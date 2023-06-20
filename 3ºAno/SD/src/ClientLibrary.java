import Exceptions.PointNotValid;
import Exceptions.ReservationNotFoundException;
import Exceptions.ScooterNotFoundException;

import java.io.IOException;

public interface ClientLibrary {

    PointList freeScooters(Point p) throws IOException, InterruptedException;

    RewardList getRewards(Point p) throws IOException, InterruptedException;

    boolean registerUser(User u) throws IOException, InterruptedException;

    boolean verifyUser(User u) throws IOException, InterruptedException;

    Tuple<Integer,Point> newReservation (Point local) throws IOException, InterruptedException, ScooterNotFoundException, PointNotValid;

    Tuple<Boolean,Double> concludeReservation(int codRes, Point local) throws IOException, InterruptedException, ReservationNotFoundException, PointNotValid;

    RewardList getNotifications(User u, Point p) throws IOException, InterruptedException;

    void cancelNotifications(User u) throws IOException;

    void closeConnection(User u) throws IOException;
}

