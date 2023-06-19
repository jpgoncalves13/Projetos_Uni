package Simulation;

import CasaInteligente.CasaInteligenteException;
import FilesInteraction.LinhaIncorretaException;
import Fornecedores.FaturaException;
import Fornecedores.FornecedoresEnergiaException;
import SmartDevices.ResolutionException;
import SmartDevices.SmartDeviceException;

import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.time.format.DateTimeParseException;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

import static java.lang.System.out;

public class ComunityView {

    private ComunityController controller;
    private static Scanner sc = new Scanner(System.in);

    public ComunityView () {
        this.controller = new ComunityController();
    }

    public ComunityView(ComunityController controller) {
        this.controller = controller;
    }

    public static void closeScanner () {
        sc.close();
    }

    public static String readLine(String msg) {
        out.print(msg);
        return sc.next();
    }

    public static String readLn(String msg) {
        out.print(msg);
        sc.nextLine();
        return sc.nextLine();
    }

    public static void printString (String msg) {
        out.println(msg);
    }

    public static int readInt(String msg) {
        out.print(msg);
        return sc.nextInt();
    }

    public static double readDouble(String msg) {
        out.print(msg);
        return sc.nextDouble();
    }

    public static List<String> lerFicheiro(String nomeFich) throws IOException{
        List<String> lines;
        try {
            lines = Files.readAllLines(Paths.get(nomeFich), StandardCharsets.UTF_8);
        }
        catch(IOException exc) {
            lines = new ArrayList<>();
        }
        return lines;
    }

    public static List<String> lerMenu(){
        ArrayList<String> result = new ArrayList<String>();
        String line = "";
        while(!(line = sc.nextLine()).equals("fim")) {
            result.add(line);
        }
        return result;
    }


    public static void invalidOption(){
        out.println("Ops.. Opção inválida");
    }

    public Menu setMenuFoward() {
        Menu gofoward = new Menu();
        gofoward.addOption("Terminar a simulação");
        gofoward.addOption("Avançar do tempo em dias");
        gofoward.addOption("Simulação automática");
        gofoward.addOption("Guardar estado do programa");
        return gofoward;
    }

    public Menu setinit() {
        Menu init = new Menu();
        init.addOption("Carregar através do menu");
        init.addOption("Carregar através de um ficheiro de texto");
        init.addOption("Carregar através de um ficheiro com o estado do programa");
        return init;
    }

    public Menu setchanges() {
        Menu changes = new Menu();
        changes.addOption("Avançar");
        changes.addOption("Alterar fornecedor de energia de uma casa");
        changes.addOption("Ligar ou desligar dispositivo de uma casa");
        changes.addOption("Ligar ou desligar todos os dispositivos de uma casa");
        changes.addOption("Alterar valores dos fornecedores de energia");
        return changes;
    }

    public Menu setqueries() {
        Menu queries = new Menu();
        queries.addOption("Avançar");
        queries.addOption("Qual a casa que mais gastou no período decorrido");
        queries.addOption("Qual o comercializador com maior volume de faturação");
        queries.addOption("Lista das faturas emitidas por um comercializador");
        queries.addOption("Os n maiores consumidores durante um período");
        return queries;
    }



    public void run() {
        Menu gofoward = this.setMenuFoward();
        Menu init = this.setinit();
        Menu changes =  this.setchanges();
        Menu queries = this.setqueries();


        System.out.println("|---------------------------------------------------------|");
        System.out.println("|     Programa de Gestão Energética de uma Comunidade     |");
        System.out.println("|---------------------------------------------------------|");

        int opcao;
        boolean skip = false;

        out.println();
        out.println(init.toString());
        out.println();

        while (!skip) {
            out.print("Insira opção: ");
            opcao = sc.nextInt();
            try {
                skip = controller.initOption(opcao);
            } catch (LinhaIncorretaException | SmartDeviceException | ResolutionException | CasaInteligenteException e) {
                out.println("Ops " + e.getMessage());
            } catch (ClassNotFoundException | IOException e) {
                out.println(e.getMessage());
            }
            out.println();
        }

        skip = false;
        while (!skip) {
            out.println(gofoward);
            out.println();
            out.print("Insira opção: ");
            opcao = sc.nextInt();
            try {
                skip = controller.simulationFase(opcao,queries,changes);
            }
            catch (FaturaException | IOException | DateTimeParseException | CasaInteligenteException | FornecedoresEnergiaException |
                   LinhaIncorretaException e) {
                out.println("Ops " + e.getMessage());
            }
            out.println();
        }

    }


}
