import ConnectionHandler.TaggedConnection;
import Exceptions.PointNotValid;
import Exceptions.ReservationNotFoundException;
import Exceptions.ScooterNotFoundException;

import java.io.*;

public class Worker implements Runnable {
    private final TaggedConnection connection;
    private final Management management;

    public Worker (TaggedConnection connection, Management management) {
        this.connection = connection;
        this.management = management;
    }

    @Override
    public void run() {
            try (connection) {
                while (true) {
                    TaggedConnection.Frame frame = connection.receive();
                    DataInputStream dis = new DataInputStream(new ByteArrayInputStream(frame.data));
                    int tag = frame.tag;

                    if (tag == 1) { /* tag 1 -> Login */

                        byte[] resp = new byte[1];
                        if (management.verifyUser(User.deserialize(dis))) resp[0] = (byte) 1;
                        connection.send(tag, resp);

                    } else if (tag == 2) { /* tag 2 -> Register */

                        byte[] resp = new byte[1];
                        if (management.registerUser(User.deserialize(dis))) resp[0] = (byte) 1;
                        connection.send(tag, resp);

                    } else if (tag == 3) { /* tag 3 -> List of places where there are free scooters */

                        ByteArrayOutputStream byteArrayOS = new ByteArrayOutputStream();
                        DataOutputStream dos = new DataOutputStream(byteArrayOS);
                        management.freeScooters(Point.deserialize(dis)).serialize(dos);
                        connection.send(tag, byteArrayOS.toByteArray());

                    } else if (tag == 4) { /* tag 4 -> List of rewards */

                        ByteArrayOutputStream byteArrayOS = new ByteArrayOutputStream();
                        DataOutputStream dos = new DataOutputStream(byteArrayOS);
                        management.getRewards(Point.deserialize(dis)).serialize(dos);
                        connection.send(tag, byteArrayOS.toByteArray());

                    } else if (tag == 5) { /* tag 5 -> Book scooter */

                        int codRes; Point local = null;
                        try {
                            Tuple<Integer,Point> t = management.newReservation(Point.deserialize(dis));
                            codRes = t.getValue1();
                            local = t.getValue2();
                        } catch (ScooterNotFoundException e) {
                            codRes = 0;
                        } catch (PointNotValid e) {
                            codRes = -1;
                        }
                        ByteArrayOutputStream byteArrayOS = new ByteArrayOutputStream();
                        DataOutputStream dos = new DataOutputStream(byteArrayOS);
                        dos.writeInt(codRes); // 0 -> error
                        if (codRes != 0 && codRes != -1) {
                            local.serialize(dos);
                        }
                        connection.send(tag, byteArrayOS.toByteArray());

                    } else if (tag == 6) { /* tag 6 -> Park scooter */

                        Point p = Point.deserialize(dis);
                        int codRes = dis.readInt();
                        double cost = 0;
                        boolean isReward = false;
                        try {
                            Tuple<Boolean,Double> t = management.concludeReservation(codRes, p);
                            isReward = t.getValue1();
                            cost = t.getValue2();
                        } catch (ReservationNotFoundException e) {
                            codRes = 0; // error
                        } catch (PointNotValid e) {
                            codRes = -1;
                        }
                        ByteArrayOutputStream byteArrayOS = new ByteArrayOutputStream();
                        DataOutputStream dos = new DataOutputStream(byteArrayOS);
                        dos.writeInt(codRes);
                        if (codRes != 0 && codRes != -1) {
                            dos.writeBoolean(isReward);
                            dos.writeDouble(cost);
                        }
                        connection.send(tag, byteArrayOS.toByteArray());

                    } else if (tag == 7) { /* tag 7 -> Send notifications */
                        User u = User.deserialize(dis);
                        management.activateNotifications(u);
                        Point p = Point.deserialize(dis);

                        while (management.isNotificationsEnable(u)) {
                            ByteArrayOutputStream byteArrayOS = new ByteArrayOutputStream();
                            DataOutputStream dos = new DataOutputStream(byteArrayOS);
                            RewardList r = management.getNotifications(u,p);
                            if (r.size()>0) {
                                r.serialize(dos);
                                connection.send(tag, byteArrayOS.toByteArray());
                            }
                        }

                    } else if (tag == 8) { /* tag 8 -> Cancel Notifications */
                        management.cancelNotifications(User.deserialize(dis));
                    } else if (tag == 9) { /* tag 9 -> Close Connection */
                        management.closeConnection(User.deserialize(dis));
                    }
                }
            } catch (Exception ignored) {}
    }


}

