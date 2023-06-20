import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;

public class Point {
    private final int x;
    private final int y;

    public Point(int x, int y) {
        this.x = x;
        this.y = y;
    }

    public int getX() {
        return x;
    }

    public int getY() {
        return y;
    }

    public int distance(Point p2) {
        int dx = this.x - p2.x;
        int dy = this.y - p2.y;
        return Math.abs(dx) + Math.abs(dy);
    }

    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;

        Point p2 = (Point) o;
        return this.x == p2.x &&
               this.y == p2.y;
    }

    public void serialize (DataOutputStream dos) throws IOException {
        dos.writeInt(this.x);
        dos.writeInt(this.y);
    }

    public static Point deserialize (DataInputStream dis) throws IOException {
        int x = dis.readInt();
        int y = dis.readInt();
        return new Point(x,y);
    }

    @Override
    public String toString() {
        return "(" + this.x + "," + this.y + ")";
    }

    @Override
    public Point clone() {
        return new Point(this.x,this.y);
    }

}