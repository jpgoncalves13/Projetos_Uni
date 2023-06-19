package Simulation;

import CasaInteligente.CasaInteligente;
import Changes.ComunityChanges;
import Changes.DeviceOnOff;
import Changes.ChangeProvider;
import Changes.ChangeDefsValPorviders;
import Changes.AllDevicesOnOff;
import Fornecedores.ComparatorFornecedoresCustoDesc;
import Fornecedores.FornecedoresEnergia;
import Fornecedores.FornecedoresEnergiaException;
import Fornecedores.Fatura;
import Fornecedores.FaturaException;
import Fornecedores.ComparatorFaturaConsumoDesc;
import SmartDevices.SmartDevice;
import CasaInteligente.CasaInteligenteException;

import java.io.Serializable;
import java.time.LocalDate;
import java.util.List;
import java.util.Map;
import java.util.Iterator;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.HashMap;
import java.util.Set;
import java.util.TreeSet;
import java.util.stream.Collectors;

public class ComunityModel implements Serializable {

    private Map<String, FornecedoresEnergia> fornecedores;
    private Map<Integer,CasaInteligente> casas;
    private LocalDate dataAtual;
    private List<ComunityChanges> pedidos;

    public ComunityModel() {
        this.fornecedores = new HashMap<>();
        this.casas = new HashMap<>();
        this.dataAtual = LocalDate.now();
        this.pedidos = new ArrayList<>();
    }

    public ComunityModel(Map<String,FornecedoresEnergia> fornecedores, Map<Integer,CasaInteligente> casas){
        this.setFornecedores(fornecedores);
        this.setCasas(casas);
        this.dataAtual = LocalDate.now();
        this.pedidos = new ArrayList<>();
    }

    public ComunityModel(ComunityModel c) {
        this.fornecedores = c.getFornecedores();
        this.casas = c.getCasas();
        this.dataAtual = c.getDataAtual();
        this.pedidos = c.getPedidos();
    }

    public LocalDate getDataAtual() {
        return dataAtual;
    }

    public void setDataAtual(LocalDate dataAtual) {
        this.dataAtual = dataAtual;
    }

    public Map<String, FornecedoresEnergia> getFornecedores() {
        return this.fornecedores.entrySet().stream().collect(Collectors.toMap(Map.Entry::getKey,(e)->e.getValue().clone()));
    }

    public void setFornecedores(Map<String, FornecedoresEnergia> forncedores) {
        this.fornecedores = forncedores.entrySet().stream().collect(Collectors.toMap(Map.Entry::getKey, (e)->e.getValue().clone()));
    }

    public Map<Integer,CasaInteligente> getCasas() {
        return this.casas.entrySet().stream().collect(Collectors.toMap(Map.Entry::getKey,(e)->e.getValue().clone()));
    }

    public void setCasas(Map<Integer,CasaInteligente> casas) {
        this.casas = casas.entrySet().stream().collect(Collectors.toMap(Map.Entry::getKey,(e)->e.getValue().clone()));
    }

    public List<ComunityChanges> getPedidos(){
        return this.pedidos.stream().map(ComunityChanges::clone).collect(Collectors.toList());
    }

    public void setPedidos(List<ComunityChanges> pedidos) {
        this.pedidos = pedidos.stream().map(ComunityChanges::clone).collect(Collectors.toList());
    }

    public void addPedido (ComunityChanges c) {
        this.pedidos.add(c.clone());
    }

    public void execPedidos () {
        Iterator<ComunityChanges> it = this.pedidos.iterator();
        ComunityChanges p;
        AllDevicesOnOff ad;
        ChangeDefsValPorviders dp;
        ChangeProvider cp;
        DeviceOnOff d;

        while(it.hasNext()) {
            p = it.next();
            if (p instanceof AllDevicesOnOff) {
                ad = (AllDevicesOnOff) p;
                this.devicesCasaOnOff(ad.getHouseId(),ad.isOn());
            }
            else if (p instanceof ChangeDefsValPorviders) {
                dp = (ChangeDefsValPorviders) p;
                this.alteraValoresBaseFornecedores(dp.getValorBaseKw(), dp.getImposto());
            }
            else if (p instanceof ChangeProvider) {
                cp = (ChangeProvider) p;
                this.alteraFornecedorCasa(cp.getHouseId(),cp.getProviderName());
            }
            else if (p instanceof DeviceOnOff) {
                d = (DeviceOnOff) p;
                this.setDeviceOnOff(d.gethouseId(),d.getDeviceId(),d.isOn());
            }
            it.remove();
        }
    }

    public boolean containsFornecedor (String nomeFornecedor) {
        return this.fornecedores.containsKey(nomeFornecedor);
    }

    public boolean existsHouse (int houseId) {
        return this.casas.containsKey(houseId);
    }

    public boolean existsDevice (int deviceId, int houseId) {
        boolean b = false;
        Iterator<CasaInteligente> it = this.casas.values().iterator();
        while(it.hasNext()){
            CasaInteligente casa = it.next();
            if (casa.getId() == houseId){
                if(casa.getDevices().containsKey(deviceId)){
                    b = true;
                }
            }
        }
        return b;
    }

    public boolean existsDivHouse (String divName, int houseId) {
        return this.casas.get(houseId).existsDiv(divName);
    }

    public void avancarTempo(LocalDate nextDate) throws FaturaException {
        Iterator<CasaInteligente> it = casas.values().iterator();
        CasaInteligente casa = null;
        Fatura fatura = null;
        String nomeFornecedor;
        double consumo, custo;

        while (it.hasNext()) {
            casa = it.next();
            consumo = casa.calculateConsumptionCasa(this.dataAtual,nextDate);
            custo = this.fornecedores.get(casa.getFornecedor()).calculateFaturation(consumo);
            nomeFornecedor = casa.getFornecedor();
            fatura = new Fatura(casa.getOwner(),casa.getNif(),casa.getId(),nomeFornecedor,this.dataAtual,nextDate,consumo,custo);
            if (casa.existsFornecedor(nomeFornecedor)) {
                casa.addFatura(nomeFornecedor,fatura);
                this.fornecedores.get(casa.getFornecedor()).addFatura(fatura);
            }
            else {
                casa.addFornecedorToFaturas(nomeFornecedor);
                fatura.setCusto(custo+casa.custoInstalcaoAllDevices());
                casa.addFatura(nomeFornecedor,fatura);
                this.fornecedores.get(casa.getFornecedor()).addFatura(fatura);
            }
        }

        this.setDataAtual(nextDate);
    }

    public void devicesCasaOnOff (int numPorta, boolean on) {
        this.casas.get(numPorta).setAllOn(on);
    }

    public void alteraValoresBaseFornecedores (double valorBaseKw, double imposto) {
        FornecedoresEnergia.setValorBasekwh(valorBaseKw);
        FornecedoresEnergia.setImposto(imposto);
    }

    public String devicesOfRoom (int numPorta, String div) throws CasaInteligenteException{
        StringBuilder sb = new StringBuilder("Devices da " + div + ": ");
        Map<Integer,SmartDevice> devices = this.casas.get(numPorta).getDevices();
        List<Integer> list = this.casas.get(numPorta).devicesOfRoom(div);

        if (list == null) {
            throw new CasaInteligenteException("Divisão " + div + " não existe");
        }
        Iterator<Integer> it = list.iterator();
        int id;

        while (it.hasNext()) {
            id = it.next();
            sb.append(id).append("-").append(devices.get(id).getClass().getName());
            sb.append("\n\t\t\t\t    ");
        }

        return sb.toString();
    }

    public void setDeviceOnOff (int numPorta, int id, boolean on) {
        if (on) this.casas.get(numPorta).setDeviceOn(id);
        else this.casas.get(numPorta).setDeviceOff(id);
    }

    public int casaQueMaisGastouUltimoPeriodo() throws FaturaException {
        Iterator<FornecedoresEnergia> it = this.fornecedores.values().iterator();
        Set<Fatura> setFat = new TreeSet<>(new ComparatorFaturaConsumoDesc());

        Iterator<Fatura> itfat;
        FornecedoresEnergia fornecedor;
        Fatura fat;

        while (it.hasNext()) {
            fornecedor=it.next();
            itfat = fornecedor.getFaturas().iterator();
            while (itfat.hasNext()) {
                fat = itfat.next();
                if (fat.getTo().isEqual(this.dataAtual)) {
                    setFat.add(fat);
                }
            }
        }

        fat = setFat.stream().findFirst().orElse(null);
        if (fat==null) throw new FaturaException("Não existem faturas");
        return fat.getHouseId();
    }

    public void alteraFornecedorCasa (int numPorta, String nome) {
        this.casas.get(numPorta).setFornecedor(this.fornecedores.get(nome).getNome());
    }

    public List<Fatura> faturasComercializador (String nome) {
        return this.fornecedores.get(nome).getFaturas();
    }

    public String fornecedorMaiorFaturacao() throws FornecedoresEnergiaException {
        Set<FornecedoresEnergia> setFornecedores = new TreeSet<>(new ComparatorFornecedoresCustoDesc());
        setFornecedores.addAll(this.fornecedores.values());

        FornecedoresEnergia f = setFornecedores.stream().findFirst().orElse(null);
        if (f==null) throw new FornecedoresEnergiaException("Não existem fornecedores");
        return f.getNome();
    }

    public List<CasaInteligente> ordenaMaioresConsumidores(LocalDate inicio, LocalDate fim, int n) {
        Iterator<CasaInteligente> it = this.casas.values().iterator();

        Comparator<CasaInteligente> c = (casa1,casa2) -> {
            double calculateconsumption1 = casa1.calculateConsumptionCasa(inicio, fim);
            double calculateconsumption2 = casa2.calculateConsumptionCasa(inicio,fim);
            if (calculateconsumption1 == calculateconsumption2) return casa2.getNumDevices() - casa1.getNumDevices();
            else return (int) (calculateconsumption2 - calculateconsumption1);
        };

        TreeSet<CasaInteligente> set = new TreeSet<>(c);
        while(it.hasNext()) {
            CasaInteligente casa = it.next();
            set.add(casa);
        }
        ArrayList<CasaInteligente> list = new ArrayList<>();
        int ind = 0;
        Iterator<CasaInteligente> it2 = set.iterator();
        while(it2.hasNext() && ind < n) {
            list.add(it2.next());
            ind++;
        }
        return list;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        ComunityModel that = (ComunityModel) o;
        return fornecedores.equals(that.getFornecedores())
               && casas.equals(that.getCasas())
               && dataAtual.isEqual(that.getDataAtual())
               && pedidos.equals(that.getPedidos());
    }

    @Override
    public String toString() {
        final StringBuilder sb = new StringBuilder("Simulation.Comunidade{ ");
        sb.append("Forncedores=").append(fornecedores);
        sb.append("\nCasas=").append(casas);
        sb.append("\nData Atual=").append(dataAtual);
        sb.append("\nPedidos=").append(pedidos);
        sb.append('}');
        return sb.toString();
    }

    public ComunityModel clone() {
        return new ComunityModel(this);
    }


}
