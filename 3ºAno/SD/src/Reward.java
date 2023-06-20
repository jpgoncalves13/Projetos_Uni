import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;

public class Reward {
    private final Point initPoint;
    private final Point finalPoint;

    public Reward(Point initPoint, Point finalPoint) {
        this.initPoint = initPoint;
        this.finalPoint = finalPoint;
    }

    public Point getInitPoint() {
        return initPoint;
    }

    public Point getFinalPoint() {
        return finalPoint;
    }

    public int distanceToInit(Point point) {
        return this.initPoint.distance(point);
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Reward reward = (Reward) o;
        return initPoint.equals(reward.initPoint) &&
               finalPoint.equals(reward.finalPoint);
    }

    public void serialize (DataOutputStream dos) throws IOException {
        this.initPoint.serialize(dos);
        this.finalPoint.serialize(dos);
    }

    public static Reward deserialize (DataInputStream dis) throws IOException {
        Point p1 = Point.deserialize(dis);
        Point p2 = Point.deserialize(dis);
        return new Reward(p1,p2);
    }

    public boolean comparePoints (Point initP, Point finalP) {
        return initP.equals(initPoint) && finalP.equals(finalPoint);
    }

    @Override
    public Reward clone() {
        return new Reward(this.getInitPoint(), this.getFinalPoint());
    }

    @Override
    public String toString() {
        return "Reward: " + initPoint.toString() + " -> " + finalPoint.toString();
    }

}
