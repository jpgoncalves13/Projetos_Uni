import ConnectionHandler.TaggedConnection;
import java.net.ServerSocket;
import java.net.Socket;

public class Server {
    final static int WORKERS_PER_CONNECTION = 3;

    public static void main(String[] args) throws Exception {
        ServerSocket ss = new ServerSocket(12345);
        Management man = new Management();
        man.start();

        while(true) {
            Socket s = ss.accept();
            TaggedConnection c = new TaggedConnection(s);
            for (int i = 0; i < WORKERS_PER_CONNECTION; ++i) {
                new Thread(new Worker(c, man)).start();
            }
        }

    }
}



