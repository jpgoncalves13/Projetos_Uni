import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.util.ArrayList;

public class PointList extends ArrayList<Point> {

    public void serialize (DataOutputStream dos) throws IOException {
        dos.writeInt(this.size());
        for (Point p : this) {
            p.serialize(dos);
        }
    }

    public static PointList deserialize (DataInputStream dis) throws IOException {
        PointList pl = new PointList();
        int size = dis.readInt();
        for (int i = 0; i<size; i++) {
            pl.add(Point.deserialize(dis));
        }
        return pl;
    }

}
