package FilesInteraction;

import Simulation.ComunityModel;

import java.io.ObjectOutputStream;
import java.io.IOException;
import java.io.PrintWriter;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.FileInputStream;
import java.io.ObjectInputStream;
import java.io.Serializable;

public class FilesReadWrite implements Serializable {

    public static void writeTxtComunity (ComunityModel o, String fileName) throws IOException {
        PrintWriter fich = new PrintWriter(fileName);
        fich.println("|----------------Comunity---------------|");
        fich.println(o);
        fich.flush();
        fich.close();
    }

    public static void writeBinComunity (ComunityModel o, String fileName) throws FileNotFoundException,IOException {
        FileOutputStream fos = new FileOutputStream(fileName);
        ObjectOutputStream oos = new ObjectOutputStream(fos);
        oos.writeObject(o);
        oos.flush();
        oos.close();
    }

    public static ComunityModel readBinComunity (String fileName) throws IOException,FileNotFoundException,ClassNotFoundException {
        FileInputStream fis = new FileInputStream(fileName);
        ObjectInputStream ois = new ObjectInputStream(fis);
        ComunityModel c = (ComunityModel) ois.readObject();
        ois.close();
        return c;
    }

}
