package ConnectionHandler;

import java.io.*;
import java.net.Socket;
import java.util.concurrent.locks.ReentrantLock;

public class TaggedConnection implements AutoCloseable {
    private final ReentrantLock sendLock= new ReentrantLock();
    private final ReentrantLock receiveLock= new ReentrantLock();
    private final Socket sck;
    private final DataInputStream is;
    private final DataOutputStream os;

    public static class Frame {
        public final int tag;
        public final byte[] data;

        public Frame(int tag, byte[] data) {
            this.tag = tag;
            this.data = data;
        }
    }

    public TaggedConnection(Socket socket) throws IOException {
        this.sck=socket;
        this.is= new DataInputStream(new BufferedInputStream(this.sck.getInputStream()));
        this.os= new DataOutputStream(new BufferedOutputStream((this.sck.getOutputStream())));
    }

    public void send(Frame frame) throws IOException {
        this.sendLock.lock();
        try{
            this.os.writeInt(frame.tag);
            this.os.writeInt(frame.data.length);
            this.os.write(frame.data);
            this.os.flush();
        }
        finally {
            this.sendLock.unlock();
        }
    }

    public void send(int tag, byte[] data) throws IOException {
        this.sendLock.lock();
        try{
            this.os.writeInt(tag);
            this.os.writeInt(data.length);
            this.os.write(data);
            this.os.flush();
        }
        finally {
            this.sendLock.unlock();
        }
    }

    public Frame receive() throws IOException {
        this.receiveLock.lock();
        try{
            int tag=this.is.readInt();
            int size=this.is.readInt();
            byte[] data= new byte[size];
            this.is.readFully(data);
            return new Frame(tag, data);
        }
        finally {
            this.receiveLock.unlock();
        }
    }

    public void close() throws IOException {
        this.sck.close();
    }
}

