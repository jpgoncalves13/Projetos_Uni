import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class Scooter {
    private final Lock lock = new ReentrantLock();
    private Point position;
    private boolean busy;

    public Scooter(int x, int y) {
        this.position = new Point(x, y);
        this.busy = false;
    }

    public Lock getLock() {
        return this.lock;
    }

    public Point getPosition() {
        return this.position;
    }

    public boolean isBusy(){
        return this.busy;
    }

    public void setPosition(int x, int y) {
        this.position = new Point(x,y);
    }

    public void setPosition(Point p) {
        this.setPosition(p.getX(), p.getY());
    }

    public void setBusy(boolean busy) {
        this.busy = busy;
    }

    public int distanceToPosition(Point local) {
        return this.position.distance(local);
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Scooter scooter = (Scooter) o;
        return position.equals(scooter.position) &&
               busy == scooter.busy;
    }

    @Override
    public String toString() {
        return "Scooter: " + position.toString() + ", Busy=" + busy;
    }

}
