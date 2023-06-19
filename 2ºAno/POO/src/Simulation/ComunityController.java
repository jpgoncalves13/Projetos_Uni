package Simulation;

import CasaInteligente.CasaInteligente;
import CasaInteligente.CasaInteligenteException;
import Changes.AllDevicesOnOff;
import Changes.ChangeDefsValPorviders;
import Changes.ChangeProvider;
import Changes.DeviceOnOff;
import FilesInteraction.FilesReadWrite;
import FilesInteraction.LinhaIncorretaException;
import FilesInteraction.ReadTxtFiles;
import Fornecedores.Fatura;
import Fornecedores.FaturaException;
import Fornecedores.FornecedoresEnergiaException;
import SmartDevices.ResolutionException;
import SmartDevices.SmartDeviceException;

import java.io.IOException;
import java.time.LocalDate;
import java.time.format.DateTimeFormatter;
import java.time.format.DateTimeParseException;
import java.util.List;

import static java.lang.System.out;

public class ComunityController {

    private ComunityModel model;

    public ComunityController() {
        this.model = new ComunityModel();
    }

    public ComunityController(ComunityModel model) {
        this.model = model;
    }

    public boolean initOption(int option) throws ResolutionException, CasaInteligenteException, LinhaIncorretaException, SmartDeviceException, IOException, ClassNotFoundException {
        String file;
        boolean aux = false;
        switch (option) {
            case 0: model = ReadTxtFiles.parserTxtComunityObjects(ComunityView.lerMenu());
                    aux = true;
                    break;
            case 1: file = ComunityView.readLine("Insira o nome do ficheiro a ler: ");
                    model = ReadTxtFiles.parserTxtComunityObjects(ComunityView.lerFicheiro(file));
                    aux = true;
                    break;
            case 2: file = ComunityView.readLine("Insira o nome do ficheiro a ler: ");
                    model = FilesReadWrite.readBinComunity(file);
                    aux = true;
                    break;
            default: ComunityView.invalidOption(); break;
        }
        return aux;
    }

    public void queriesFase(Menu queries) throws FaturaException, DateTimeParseException, FornecedoresEnergiaException {

        List<Fatura> faturas;
        String str;
        int aux, aux1, idcasa;
        boolean skip = false;

        while (!skip) {
            ComunityView.printString("\n"+queries+"\n");
            aux = ComunityView.readInt("Insira opção: ");

            switch (aux) {
                case 0: skip = true;
                        break;
                case 1: idcasa = model.casaQueMaisGastouUltimoPeriodo();
                        ComunityView.printString("Id da casa que mais gastou: " + idcasa);
                        break;
                case 2: ComunityView.printString(model.fornecedorMaiorFaturacao());
                        break;
                case 3: str = ComunityView.readLn("Insira o nome do comercializador: ");
                        if (!model.containsFornecedor(str)) throw new FornecedoresEnergiaException("Fornecedor " + str + " não existe");
                        faturas = model.faturasComercializador(str);
                        ComunityView.printString(Fatura.showFaturas(faturas));
                        break;
                case 4: str = ComunityView.readLine("Insira a data de inicio (DD-MM-YYYY): ");
                        DateTimeFormatter dateFormat = DateTimeFormatter.ofPattern("dd-MM-yyyy");
                        LocalDate date = LocalDate.parse(str, dateFormat);
                        str = ComunityView.readLine("Insira a data de fim (DD-MM-YYYY): ");
                        LocalDate date1 = LocalDate.parse(str, dateFormat);
                        aux1 = ComunityView.readInt("Insira o número de consumidores: ");
                        ComunityView.printString(CasaInteligente.showConsumidores(model.ordenaMaioresConsumidores(date,date1,aux1)));
                        break;
                default: ComunityView.invalidOption(); break;
            }
        }

        model.execPedidos();
    }


    public void changesFase(Menu changes) throws CasaInteligenteException, FornecedoresEnergiaException {

        ChangeProvider cp = null; DeviceOnOff d = null; AllDevicesOnOff ad = null; ChangeDefsValPorviders cdvp = null;
        String str;
        int aux, aux1, aux4;
        double aux2, aux3;
        boolean skip = false;

        while (!skip) {
            out.println();
            out.println(changes);
            out.println();
            aux4 = ComunityView.readInt("Insira opção: ");
            switch (aux4) {
                case 0: skip = true; break;
                case 1: aux = ComunityView.readInt("Insira o id da casa: ");
                        if (!model.existsHouse(aux)) throw new CasaInteligenteException("Casa com id " + aux + " não existe.");
                        str = ComunityView.readLn("Insira o nome do novo fornecedor da casa: ");
                        if (!model.containsFornecedor(str)) throw new FornecedoresEnergiaException("Fornecedor " + str + " não existe");
                        cp = new ChangeProvider(aux,str);
                        model.addPedido(cp);
                        break;
                case 2: aux = ComunityView.readInt("Insira o id da casa: ");
                        if (!model.existsHouse(aux)) throw new CasaInteligenteException("Casa com id " + aux + " não existe.");
                        str = ComunityView.readLn("Insira o nome da divisão onde se encontra o dispositivo: ");
                        if (!model.existsDivHouse(str,aux)) throw new CasaInteligenteException("A " + str + " não existe na casa com id " + aux);
                        ComunityView.printString("\n" + model.devicesOfRoom(aux,str));
                        aux1 = ComunityView.readInt("Insira o id do device a ligar/desligar: ");
                        if (!model.existsDevice(aux1,aux)) throw new CasaInteligenteException("O device com Id" + aux1 + "nao existe na casa");
                        aux4 = -1;
                        while (aux4!= 1 && aux4!= 2) {
                            ComunityView.printString("\n1. Ligar\n2. Desligar\n");
                            aux4 = ComunityView.readInt("Escolha uma opção: ");
                            switch (aux4) {
                                case 1: d = new DeviceOnOff(aux1,aux,true); break;
                                case 2: d = new DeviceOnOff(aux1,aux,false); break;
                                default: ComunityView.invalidOption(); break;
                            }
                        }
                        model.addPedido(d);
                        break;
                case 3: aux = ComunityView.readInt("Insira o id da casa: ");
                        if (!model.existsHouse(aux)) throw new CasaInteligenteException("Casa com id " + aux + " não existe.");
                        aux1 = -1;
                        while (aux1!= 1 && aux1 != 2) {
                            ComunityView.printString("\n1. Ligar\n2. Desligar\n");
                            aux1 = ComunityView.readInt("Escolha uma opção: ");
                            switch (aux1) {
                                case 1: ad = new AllDevicesOnOff(aux,true); break;
                                case 2: ad = new AllDevicesOnOff(aux,false); break;
                                default: ComunityView.invalidOption(); break;
                            }
                        }
                        model.addPedido(ad);
                        break;
                case 4: aux2 = ComunityView.readDouble("Insira o novo valor base do kw: ");
                        aux3 = ComunityView.readDouble("Insira o novo valor do imposto: ");
                        cdvp = new ChangeDefsValPorviders(aux2,aux3);
                        model.addPedido(cdvp);
                        break;
                default: ComunityView.invalidOption(); break;
                }
        }

    }

    public void parserTxtAutoSimulation(List<String> linhas) throws LinhaIncorretaException, FaturaException {
        String[] strings = null;
        DateTimeFormatter formatter = DateTimeFormatter.ofPattern("yyyy.MM.dd");
        LocalDate localDate;
        LocalDate nextdate = null;
        boolean inSimulation = false;
        AllDevicesOnOff ad;ChangeDefsValPorviders cd;ChangeProvider cp;DeviceOnOff dof=null;
        int idCasa, idDispositivo;

        for(String line : linhas) {
            strings = line.split(", ");

            if (strings.length<3) throw new LinhaIncorretaException("Linha inválida");

            if (strings[0].equals("Avançar")) {
                localDate = LocalDate.parse(strings[1], formatter);
                model.setDataAtual(localDate);
                nextdate = localDate.plusDays(Integer.parseInt(strings[2]));
                model.avancarTempo(nextdate);
                inSimulation = true;
            }
            else {
                localDate = LocalDate.parse(strings[0], formatter);

                if (inSimulation && localDate.isEqual(nextdate)) {
                    inSimulation = false;
                    model.execPedidos();
                }

                if (strings[1].chars().allMatch(Character::isDigit)) {
                    idCasa = Integer.parseInt(strings[1]);
                    if (strings[2].chars().allMatch(Character::isDigit)) {
                        idDispositivo = Integer.parseInt(strings[2]);
                        if (strings[3].equals("ligar")) {
                            dof = new DeviceOnOff(idDispositivo, idCasa, true);
                            model.addPedido(dof);
                        }
                        else if (strings[3].equals("desligar")) {
                            dof = new DeviceOnOff(idDispositivo, idCasa, false);
                            model.addPedido(dof);
                        }
                    }
                    else if (strings[2].equals("ligar")) {
                        ad = new AllDevicesOnOff(idCasa,true);
                        model.addPedido(ad);
                    }
                    else if (strings[2].equals("desligar")) {
                        ad = new AllDevicesOnOff(idCasa,false);
                        model.addPedido(ad);
                    }
                    else {
                        cp = new ChangeProvider(idCasa,strings[2]);
                        model.addPedido(cp);
                    }
                }
                else {
                    cd = new ChangeDefsValPorviders(Double.parseDouble(strings[2]),Double.parseDouble(strings[3]));
                    model.addPedido(cd);
                }
            }

        }
    }


    public boolean simulationFase(int option, Menu queries,Menu changes) throws FaturaException, DateTimeParseException, IOException, CasaInteligenteException, FornecedoresEnergiaException, LinhaIncorretaException {

       LocalDate nextDate;
       String file;
       int aux;
       boolean skip = false;

       switch (option) {
            case 0: skip = true;
                break;
            case 1: aux = ComunityView.readInt("Introduza o número de dias a avançar no tempo: ");
                nextDate = model.getDataAtual().plusDays(aux);
                model.avancarTempo(nextDate);
                model.execPedidos();
                queriesFase(queries);
                changesFase(changes);
                break;
            case 2: file = ComunityView.readLine("Introduza o nome do ficheiro: ");
                    parserTxtAutoSimulation(ComunityView.lerFicheiro(file));
                    break;
            case 3: file = ComunityView.readLine("Insira o nome do ficheiro a escrever: ");
                    FilesReadWrite.writeBinComunity(model,file);
                    break;
            default: ComunityView.invalidOption();
                    break;
        }
       return skip;
    }







}
