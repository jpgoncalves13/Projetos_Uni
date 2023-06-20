import java.time.LocalDateTime;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class Reservation {
    private static int nextCod = 1;
    private final static Lock codLock = new ReentrantLock();

    private final int cod;
    private final LocalDateTime time;
    private final Scooter scooter;

    private static int getAndIncrement() {
        try {
            codLock.lock();
            return nextCod;
        } finally {
            nextCod++;
            codLock.unlock();
        }
    }

    public Reservation(Scooter scooter) {
        this.cod = getAndIncrement();
        this.scooter = scooter;
        this.time = LocalDateTime.now();
    }

    public int getCod() {
        return this.cod;
    }

    public Scooter getScooter(){
        return this.scooter;
    }

    public LocalDateTime getTime() {
        return this.time;
    }

}
