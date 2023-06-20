import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.util.ArrayList;

public class RewardList extends ArrayList<Reward> {

    public void serialize (DataOutputStream dos) throws IOException {
        dos.writeInt(this.size());
        for (Reward r : this) {
            r.serialize(dos);
        }
    }

    public static RewardList deserialize (DataInputStream dis) throws IOException {
        RewardList pl = new RewardList();
        int size = dis.readInt();
        for (int i = 0; i<size; i++) {
            pl.add(Reward.deserialize(dis));
        }
        return pl;
    }

}
