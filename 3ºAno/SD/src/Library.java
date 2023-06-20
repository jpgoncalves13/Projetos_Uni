import ConnectionHandler.Demultiplexer;
import ConnectionHandler.TaggedConnection;
import Exceptions.PointNotValid;
import Exceptions.ReservationNotFoundException;
import Exceptions.ScooterNotFoundException;

import java.io.*;
import java.net.Socket;

public class Library implements ClientLibrary {

    private final Demultiplexer demultiplexer;

    public Library() throws IOException {
        Socket socket = new Socket("localhost", 12345);
        TaggedConnection connection = new TaggedConnection(socket);
        this.demultiplexer = new Demultiplexer(connection);
        this.demultiplexer.start();
    }

    public PointList freeScooters(Point p) throws IOException, InterruptedException {
        ByteArrayOutputStream byteArrayOS = new ByteArrayOutputStream();
        DataOutputStream dos = new DataOutputStream(byteArrayOS);
        p.serialize(dos);
        this.demultiplexer.send(3, byteArrayOS.toByteArray());

        byte[] data = this.demultiplexer.receive(3);
        DataInputStream is = new DataInputStream(new ByteArrayInputStream(data));
        return PointList.deserialize(is);
    }

    public RewardList getRewards(Point p) throws IOException, InterruptedException {
        ByteArrayOutputStream byteArrayOS = new ByteArrayOutputStream();
        DataOutputStream dos = new DataOutputStream(byteArrayOS);
        p.serialize(dos);
        this.demultiplexer.send(4, byteArrayOS.toByteArray());

        byte[] data = this.demultiplexer.receive(4);
        DataInputStream dis = new DataInputStream(new ByteArrayInputStream(data));
        return RewardList.deserialize(dis);
    }

    public boolean registerUser(User u) throws IOException, InterruptedException {
        ByteArrayOutputStream byteArrayOS = new ByteArrayOutputStream();
        DataOutputStream dos = new DataOutputStream(byteArrayOS);
        u.serialize(dos);
        this.demultiplexer.send(2,byteArrayOS.toByteArray());

        byte[] data = this.demultiplexer.receive(2);
        return data[0] == (byte) 1;
    }

    public boolean verifyUser(User u) throws IOException, InterruptedException {
        ByteArrayOutputStream byteArrayOS = new ByteArrayOutputStream();
        DataOutputStream dos = new DataOutputStream(byteArrayOS);
        u.serialize(dos);
        this.demultiplexer.send(1,byteArrayOS.toByteArray());

        byte[] data = this.demultiplexer.receive(1);
        return data[0] == (byte) 1;
    }

    public Tuple<Integer,Point> newReservation (Point local) throws IOException, InterruptedException, ScooterNotFoundException, PointNotValid {
        ByteArrayOutputStream byteArrayOS = new ByteArrayOutputStream();
        DataOutputStream dos = new DataOutputStream(byteArrayOS);
        local.serialize(dos);
        this.demultiplexer.send(5, byteArrayOS.toByteArray());

        byte[] data = this.demultiplexer.receive(5);
        DataInputStream dis = new DataInputStream(new ByteArrayInputStream(data));
        int cod = dis.readInt();
        Point p = null;
        if (cod != 0 && cod != -1) { p = Point.deserialize(dis); }
        else if (cod == 0){
            throw new ScooterNotFoundException("No scooter available\n");
        }
        else {
            throw new PointNotValid("The Point is invalid\n");
        }
        return new Tuple<>(cod,p);
    }

    public Tuple<Boolean,Double> concludeReservation(int codRes, Point local) throws IOException, InterruptedException, ReservationNotFoundException, PointNotValid {
        ByteArrayOutputStream byteArrayOS = new ByteArrayOutputStream();
        DataOutputStream dos = new DataOutputStream(byteArrayOS);
        local.serialize(dos);
        dos.writeInt(codRes);
        this.demultiplexer.send(6, byteArrayOS.toByteArray());

        byte[] data = this.demultiplexer.receive(6);
        DataInputStream dis = new DataInputStream(new ByteArrayInputStream(data));
        int cod = dis.readInt();
        double cost = 0;
        boolean isReward = false;
        if (cod != 0 && cod != -1) {
            isReward = dis.readBoolean();
            cost = dis.readDouble();
        }
        else if (cod == 0) {
            throw new ReservationNotFoundException("No reservation available with the code " + codRes + "\n");
        }
        else {
            throw new PointNotValid("The Point is invalid\n");
        }
        return new Tuple<>(isReward,cost);
    }

    public RewardList getNotifications(User u, Point p) throws IOException, InterruptedException {
        ByteArrayOutputStream byteArrayOS = new ByteArrayOutputStream();
        DataOutputStream dos = new DataOutputStream(byteArrayOS);
        u.serialize(dos);
        p.serialize(dos);
        this.demultiplexer.send(7,byteArrayOS.toByteArray());

        byte[] data = this.demultiplexer.receive(7);
        DataInputStream dis = new DataInputStream(new ByteArrayInputStream(data));
        return RewardList.deserialize(dis);
    }

    public void cancelNotifications(User u) throws IOException {
        ByteArrayOutputStream byteArrayOS = new ByteArrayOutputStream();
        DataOutputStream dos = new DataOutputStream(byteArrayOS);
        u.serialize(dos);

        this.demultiplexer.send(8, byteArrayOS.toByteArray());
    }

    public void closeConnection(User u) throws IOException {
        ByteArrayOutputStream byteArrayOS = new ByteArrayOutputStream();
        DataOutputStream dos = new DataOutputStream(byteArrayOS);
        u.serialize(dos);

        this.demultiplexer.send(9, byteArrayOS.toByteArray());
        this.demultiplexer.close();
    }
}
