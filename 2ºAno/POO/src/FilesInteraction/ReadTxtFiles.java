package FilesInteraction;

import CasaInteligente.CasaInteligente;
import Fornecedores.FornecedoresEnergia;
import Simulation.ComunityModel;
import SmartDevices.SmartDeviceException;
import SmartDevices.ResolutionException;
import SmartDevices.SmartDevice;
import SmartDevices.SmartBulb;
import SmartDevices.SmartSpeaker;
import SmartDevices.SmartCamera;
import CasaInteligente.CasaInteligenteException;

import java.util.List;
import java.util.Map;
import java.util.HashMap;

public class ReadTxtFiles {
    public static ComunityModel parserTxtComunityObjects(List<String> linhas) throws LinhaIncorretaException, SmartDeviceException, ResolutionException, CasaInteligenteException {
            Map<Integer,CasaInteligente> casas = new HashMap<>();
            Map<String, FornecedoresEnergia> fornecedores = new HashMap<>();

            String[] linhaPartida;
            String Divisao = null;
            CasaInteligente ci = null;
            SmartDevice sd = null;

            for (String linha : linhas) {
                    linhaPartida = linha.split(":",2);
                    switch (linhaPartida[0]) {
                        case "Fornecedor":
                            FornecedoresEnergia com = ReadTxtFiles.readFornecedor(linhaPartida[1]);
                            fornecedores.put(com.getNome(),com);
                            break;
                        case "Casa":
                            ci = ReadTxtFiles.readHouse(linhaPartida[1],fornecedores);
                            casas.put(ci.getId(),ci);
                            Divisao = null;
                            break;
                        case "Divisao":
                            if (ci == null) throw new LinhaIncorretaException("Casa não foi criada");
                            Divisao = linhaPartida[1];
                            ci.addRoom(Divisao);
                            break;
                        case "SmartBulb":
                            if (Divisao == null) throw new LinhaIncorretaException("Divisão não existe na casa");
                            sd = ReadTxtFiles.readBulb(linhaPartida[1]);
                            ci.addDevice(sd);
                            ci.addToRoom(Divisao,sd.getID());
                            break;
                        case "SmartCamera":
                            if (Divisao == null) throw new LinhaIncorretaException("Divisão não existe na casa");
                            sd = ReadTxtFiles.readCam(linhaPartida[1]);
                            ci.addDevice(sd);
                            ci.addToRoom(Divisao,sd.getID());
                            break;
                        case "SmartSpeaker":
                            if (Divisao == null) throw new LinhaIncorretaException("Divisão não existe na casa");
                            sd = ReadTxtFiles.readSpeaker(linhaPartida[1]);
                            ci.addDevice(sd);
                            ci.addToRoom(Divisao,sd.getID());
                            break;
                        default:
                            System.out.println("Linha inválida");
                            break;
                    }
            }

            return new ComunityModel(fornecedores,casas);
    }

    public static CasaInteligente readHouse(String string, Map<String,FornecedoresEnergia> fornecedores) throws CasaInteligenteException, LinhaIncorretaException {
        String[] strings = string.split(",");
        CasaInteligente house = null;
        if (strings.length == 3) {
            if(fornecedores.containsKey(strings[2])){
                house = new CasaInteligente(strings[0],Integer.parseInt(strings[1]),strings[2]);
            }
            else throw new LinhaIncorretaException("Fornecedor não existe");
        }
        else throw new LinhaIncorretaException("Casa não possui o número de argumentos necessários");
        return house;
    }

    public static SmartBulb readBulb(String string) throws LinhaIncorretaException, SmartDeviceException {
        String[] strings = string.split(",");
        SmartBulb bulb = null;
        int  tone;
        if (strings.length == 5) {
            switch (strings[2]) {
                case "Cold": tone = 0; break;
                case "Neutral": tone = 1; break;
                case "Warm": tone = 2; break;
                default: throw new LinhaIncorretaException("O tom da SmartBulb passado na linha \"" + string + "\" não existe.");
            }
            bulb = new SmartBulb(Boolean.parseBoolean(strings[0]),Double.parseDouble(strings[4]),Double.parseDouble(strings[1]),tone,Double.parseDouble(strings[3]));
        }
        else if (strings.length == 3) {
            switch (strings[0]) {
                case "Cold": tone = 0; break;
                case "Neutral": tone = 1; break;
                case "Warm": tone = 2; break;
                default: throw new LinhaIncorretaException("O tom da SmartBulb passado na linha \"" + string + "\" não existe.");
            }
            bulb = new SmartBulb(Double.parseDouble(strings[2]),tone,Double.parseDouble(strings[1]));
        }
        else throw new LinhaIncorretaException("SmartBulb não possui o número de argumentos necessários");
        return bulb;
    }

    public static SmartCamera readCam(String string) throws SmartDeviceException, ResolutionException, LinhaIncorretaException {
        String[] strings = string.split(",");
        SmartCamera cam = null;
        if (strings.length == 5){
            String[] strings1 = strings[2].split("x");
            cam = new SmartCamera(Boolean.parseBoolean(strings[0]),Double.parseDouble(strings[4]),Double.parseDouble(strings[4]),Integer.parseInt(strings1[0]),Integer.parseInt(strings1[1]),Double.parseDouble(strings[3]));
        }
        else if (strings.length == 3) {
            String[] strings1 = strings[0].split("x");
            cam = new SmartCamera(Double.parseDouble(strings[2]),Integer.parseInt(strings1[0]),Integer.parseInt(strings1[1]),Double.parseDouble(strings[1]));
        }
        else throw new LinhaIncorretaException("SmartCamera não possui o número de argumentos necessários");
        return cam;
    }

    public static SmartSpeaker readSpeaker(String string) throws SmartDeviceException, LinhaIncorretaException {
        String[] strings = string.split(",");
        SmartSpeaker speaker = null;
        if (strings.length == 6){
            speaker = new SmartSpeaker(Boolean.parseBoolean(strings[0]),Double.parseDouble(strings[5]),Double.parseDouble(strings[1]),Integer.parseInt(strings[2]),strings[3],strings[3]);
        }
        else if (strings.length == 4){
            speaker = new SmartSpeaker(Double.parseDouble(strings[3]),Integer.parseInt(strings[0]),strings[1],strings[2]);
        }
        else throw new LinhaIncorretaException("SmartSpeaker não possui o número de argumentos necessários");
        return speaker;
    }

    public static FornecedoresEnergia readFornecedor(String string) throws LinhaIncorretaException {
        String[] strings = string.split(",");
        FornecedoresEnergia com = null;
        if (strings.length == 2) {
            com = new FornecedoresEnergia(strings[0],strings[1]);
        }
        else if (strings.length == 1){
            com = new FornecedoresEnergia(strings[0]);
        }
        else throw new LinhaIncorretaException("Fornecedor não possui o número de argumentos necessários");
        return com;
    }

}
