package ConnectionHandler;

import java.io.IOException;
import java.util.*;
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class Demultiplexer implements AutoCloseable {
    private final TaggedConnection conn;
    private final Map<Integer, Entry> buf;
    private final Lock lock;
    private IOException exception;

    private class Entry {
        private final Condition cond = lock.newCondition();
        private final ArrayDeque<byte []> queue = new ArrayDeque<>();
    }
    
    private Entry get(int tag) {
        Entry e = buf.get(tag);
        if (e==null) {
            buf.put(tag,new Entry());
            e = buf.get(tag);
        }
        return e;
    }

    public Demultiplexer(TaggedConnection conn) {
        this.conn = conn;
        this.exception = null;
        this.lock = new ReentrantLock();
        this.buf = new HashMap<>();
    }

    // inicia theard para ser produtor ( ler frames da conexão e encaminhar para o buffer correspondente)
    public void start() {
        new Thread(()-> {
            try {
                while (true) {
                    TaggedConnection.Frame frame = conn.receive();
                    lock.lock();
                    try {
                        Entry e = get(frame.tag);
                        e.queue.add(frame.data);
                        e.cond.signal();
                    } finally {
                        lock.unlock();
                    }
                }
            } catch (IOException ex) {
                lock.lock();
                try {
                    exception = ex;
                    buf.forEach((k,v) -> v.cond.signalAll());
                } finally {
                    lock.unlock();
                }
            }
        }).start();
    }

    public void send(TaggedConnection.Frame frame) throws IOException {
        send(frame.tag, frame.data);
    }

    public void send(int tag, byte[] data) throws IOException {
        conn.send(tag,data);
    }

    public byte[] receive(int tag) throws IOException, InterruptedException {
        lock.lock();
        try {
            Entry e = get(tag);
            while (e.queue.isEmpty() && exception == null) {
                e.cond.await();
            }
            
            if (!e.queue.isEmpty()) {
                return e.queue.poll();
            } else {
                throw exception;
            }
            
        } finally {
            this.lock.unlock();
        }
    }

    public void close() throws IOException {
        conn.close();
    }
}
