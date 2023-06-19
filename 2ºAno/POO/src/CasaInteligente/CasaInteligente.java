package CasaInteligente;

import Fornecedores.Fatura;
import SmartDevices.SmartDevice;

import java.io.Serializable;
import java.time.LocalDate;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.stream.Collectors;

/**
 * A CasaInteligente.CasaInteligente faz a gestão dos SmartDevices que existem e dos
 * espaços (as salas) que existem na casa.
 *
 */
public class CasaInteligente implements Serializable {

    private static int nextid = 1;
    private String owner;
    private int nif;
    private int id;
    private Map<Integer, SmartDevice> devices; // Devices da casa identificados pelo identificador único
    private Map<String, List<Integer>> locations; // Espaço -> Lista codigo dos devices
    private String fornecedor;
    private Map<String,List<Fatura>> faturas;

    /**
     * Constructor for objects of class CasaInteligente.CasaInteligente
     */
    public CasaInteligente() {
        // initialise instance variables
        this.owner = "";
        this.nif = 0;
        this.id = nextid;
        nextid++;
        this.devices = new HashMap();
        this.locations = new HashMap();
        this.fornecedor = "";
        this.faturas = new HashMap<>();
    }

    public CasaInteligente(String nome, int NIF, String fornecedor) throws CasaInteligenteException {
        this.owner = nome;
        if (NIF<=0) throw new CasaInteligenteException("O nif é um valor positivo.");
        this.nif = NIF;
        this.id = nextid;
        nextid++;
        this.devices = new HashMap();
        this.locations = new HashMap();
        this.fornecedor = fornecedor;
        this.faturas = new HashMap<>();
    }

    public CasaInteligente(CasaInteligente c) {
        this.owner = c.getOwner();
        this.nif = c.getNif();
        this.id= c.getId();
        this.fornecedor = c.getFornecedor();
        this.devices = c.getDevices();
        this.locations = c.getLocations();
        this.faturas = c.getFaturas();
    }

    public int getId() {
        return id;
    }

    public Map<String, List<Fatura>> getFaturas() {
        return faturas.entrySet().stream().collect(Collectors.toMap(Map.Entry::getKey,
                                                                    (e)->e.getValue().stream().map(Fatura::clone).collect(Collectors.toList())));
    }

    public void setDevices(Map<Integer, SmartDevice> devices) {
        this.devices = devices.entrySet().stream().collect(Collectors.toMap(Map.Entry::getKey,(e)->e.getValue().clone()));
    }

    public void setLocations(Map<String, List<Integer>> locations) {
        this.locations = locations.entrySet().stream().collect(Collectors.toMap(Map.Entry::getKey,(e)-> new ArrayList<>(e.getValue())));
    }

    public void setFaturas(Map<String, List<Fatura>> faturas) {
        this.faturas = faturas.entrySet().stream().collect(Collectors.toMap(Map.Entry::getKey,(e)-> e.getValue().stream().map(Fatura::clone).collect(Collectors.toList())));
    }

    public Map<Integer, SmartDevice> getDevices() {
        return devices.entrySet().stream().collect(Collectors.toMap(Map.Entry::getKey,(e)->e.getValue().clone()));
    }

    public Map<String, List<Integer>> getLocations() {
        return locations.entrySet().stream().collect(Collectors.toMap(Map.Entry::getKey,(e)->e.getValue().stream().collect(Collectors.toList())));
    }

    public String getOwner() {
        return this.owner;
    }

    public int getNif() {
        return this.nif;
    }

    public void setOwner(String owner) {
        this.owner = owner;
    }

    public void setNif(int nif) {
        this.nif = nif;
    }

    public String getFornecedor() {
        return this.fornecedor;
    }

    public void setFornecedor(String fornecedor) {
        this.fornecedor = fornecedor;
    }

    public int getNumDevices() {
        return this.getDevices().size();
    }

    public void setDeviceOn(int devCode) {
        this.devices.get(devCode).turnOn();
    }

    public void setDeviceOff(int devCode) {
        this.devices.get(devCode).turnOff();
    }

    public boolean existsDevice(int id) {
        return this.devices.containsKey(id);
    }
    
    public void addDevice(SmartDevice s) {
        this.devices.put(s.getID(),s.clone());
    }
    
    public SmartDevice getDevice(int s) {
        return this.devices.get(s).clone();
    }
    
    public void setOn(int s, boolean b) {
        this.devices.get(s).setOn(b);
    }
    
    public void setAllOn(boolean b) {
        this.devices.values().forEach(s->s.setOn(b));
    }
    
    public void addRoom(String s) {
        this.locations.put(s,new ArrayList<>());
    }
    
    public boolean hasRoom(String s) {
        return this.locations.containsKey(s);
    }
    
    public void addToRoom (String s1, int s2) {
        if (this.hasRoom(s1)) this.locations.get(s1).add(s2);
    }
    
    public boolean roomHasDevice (String s1, int s2) {
        return this.locations.get(s1).contains(s2);
    }

    public boolean existsFornecedor(String nomeFornecedor) {
        return this.faturas.containsKey(nomeFornecedor);
    }

    public void addFornecedorToFaturas(String nomeFornecedor) {
        this.faturas.put(nomeFornecedor,new ArrayList<>());
    }

    public void addFatura (String nomeFornecedor, Fatura f) {
        this.faturas.get(nomeFornecedor).add(f.clone());
    }

    public double calculateConsumptionCasa (LocalDate fromDate, LocalDate toDate) {
        return this.devices.values().stream().mapToDouble(dev -> dev.calculateconsumption(fromDate,toDate)).sum();
    }

    public double custoInstalcaoAllDevices () {
        return this.devices.values().stream().mapToDouble(SmartDevice::getCustoInstalacao).sum();
    }

    public boolean existsDiv (String divName) {
        return this.locations.containsKey(divName);
    }

    public List<Integer> devicesOfRoom (String div) {
        return new ArrayList<>(this.locations.get(div));
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || this.getClass()!=o.getClass()) return false;
        CasaInteligente that = (CasaInteligente) o;
        return nif == that.getNif()
               && this.id == that.getId()
               && this.owner.equals(that.getOwner())
               && this.devices.equals(that.getDevices())
               && this.locations.equals(that.getLocations())
               && this.fornecedor.equals(that.getFornecedor())
               && this.faturas.equals(that.getFaturas());
    }

    public CasaInteligente clone () {
        return new CasaInteligente(this);
    }

    public static String showConsumidores (List<CasaInteligente> casas) {
        StringBuilder sb = new StringBuilder("\nTop Consumidores:\n\n");
        Iterator<CasaInteligente> it = casas.iterator();
        int i=1;
        while (it.hasNext()) {
            sb.append(i).append(". ");
            sb.append(it.next().getOwner());
            sb.append('\n');
            i++;
        }
        return sb.toString();
    }

    @Override
    public String toString() {
        final StringBuilder sb = new StringBuilder("CasaInteligente :\n");
        sb.append("\tOwner = '").append(owner).append("'\n");
        sb.append("\tNif = ").append(nif).append('\n');
        sb.append("\tNúmero da porta = ").append(id).append('\n');
        sb.append("\tDevices = ").append(devices.toString()).append('\n');
        sb.append("\tLocations = ").append(locations.toString()).append('\n');
        sb.append("\tFornecedor = ").append(fornecedor).append('\n');
        sb.append("\tFaturas = ").append(faturas.toString()).append('\n');
        return sb.toString();
    }
}
