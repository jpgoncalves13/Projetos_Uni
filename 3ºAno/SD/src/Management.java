import Exceptions.PointNotValid;
import Exceptions.ReservationNotFoundException;
import Exceptions.ScooterNotFoundException;

import java.text.DecimalFormat;
import java.time.Duration;
import java.time.LocalDateTime;
import java.util.*;
import java.util.concurrent.locks.*;

public class Management {
    private final Scooter[] scooters;

    private final List<Reward> rewards;
    private final Lock rewardsLock = new ReentrantLock();
    private final Condition waitForNewRewards = rewardsLock.newCondition();

    private Point newBusyScooterPosition;
    private Point newFreeScooterPosition;
    private final Lock positionsLock = new ReentrantLock();
    private final Condition waitForNewEvent = positionsLock.newCondition();

    private final Map<Integer,Reservation> reservations;
    private final Lock reservationsLock = new ReentrantLock();

    private final Map<String, User> users;
    private final ReadWriteLock lockUsers = new ReentrantReadWriteLock();
    private final Lock readLockUsers = lockUsers.readLock();
    private final Lock writeLockUsers = lockUsers.writeLock();

    private static final int D = 2;
    private static final int N = 20;
    private static final int S = 100;

    public Management() {
        this.scooters = this.generateScooters();
        this.rewards = new ArrayList<>();
        this.reservations = new HashMap<>();
        this.users = new HashMap<>();
        this.newFreeScooterPosition = null;
        this.newBusyScooterPosition = null;
    }

    /* Random distribution of S = 100 scooters across the map, all free, when the server starts */
    private Scooter[] generateScooters() {
        Scooter[] scootersArray = new Scooter[S];
        Random random = new Random();
        for (int i=0; i<S; i++) {
            scootersArray[i] = new Scooter(random.nextInt(1,N), random.nextInt(1,N));
        }
        return scootersArray;
    }

    /* List of places where there are free scooters, up to a fixed distance D = 2 from a given place */
    PointList freeScooters(Point local) {

        PointList points = new PointList();
        for (int i=0; i<S; i++) {
            scooters[i].getLock().lock();
        }
        for (int i=0; i<S; i++) {
            Point point = scooters[i].getPosition();
            if (!scooters[i].isBusy() && local.distance(point) <= D) {
                points.add(point);
            }
            scooters[i].getLock().unlock();
        }
        return points;
    }

    /* List of rewards originating up to a distance D = 2 from a given location, given by origin-destination pairs.*/
    RewardList getRewards (Point local) {

        RewardList rewardList = new RewardList();
        try {
            rewardsLock.lock();
            for (Reward reward : rewards) {
                if (reward.distanceToInit(local)<=D) {
                    rewardList.add(reward);
                }
            }
        } finally {
            rewardsLock.unlock();
        }
        return rewardList;
    }

    /* User registration. */
    public boolean registerUser(User u) {
        boolean ret = false;
        try {
            this.writeLockUsers.lock();
            if (!users.containsKey(u.getUsername())) {
                User n = new User(u.getUsername(),u.getPassword(),false,true);
                this.users.put(u.getUsername(),n);
                ret = true;
            }
        }
        finally {
            this.writeLockUsers.unlock();
        }
        return ret;
    }

    /* User authentication. */
    public boolean verifyUser(User u) {
        boolean ret = false;
        try {
            this.readLockUsers.lock();
            if (users.containsKey(u.getUsername())){
                ret = this.users.get(u.getUsername()).getPassword().equals(u.getPassword()) && !this.users.get(u.getUsername()).isLogged();
            }
        } finally {
            this.readLockUsers.unlock();
        }

        return ret;
    }

    public void closeConnection(User u) {
        try {
            this.writeLockUsers.lock();
            this.users.get(u.getUsername()).setLogged(false);
        } finally {
            this.writeLockUsers.unlock();
        }
    }

    /* Reservation of a free scooter, as close as possible to a certain place, limited to a distance D = 2. */
    public Tuple<Integer,Point> newReservation (Point local) throws ScooterNotFoundException, PointNotValid {
        if ((local.getX() < 1 || local.getX() > 20) || (local.getY() < 1 || local.getY() > 20)) throw new PointNotValid("The Point is not valid");

        Scooter scooter = null;
        int distance, i, minDistance = D;
        Point ret;
        try {
            positionsLock.lock();
            for (i = 0; i<S; i++) {
                scooters[i].getLock().lock();
            }

            for (i = 0; i<S; i++) {
                if (!scooters[i].isBusy()) {
                    distance = scooters[i].distanceToPosition(local);
                    if (distance <= minDistance) {
                        scooter = scooters[i];
                        minDistance = distance;
                    }
                }
            }

            reservationsLock.lock();
            for (i = 0; i<S; i++) {
                if (scooters[i]!=scooter) scooters[i].getLock().unlock();
            }

            if (scooter == null) {
                positionsLock.unlock();
                throw new ScooterNotFoundException("No scooter available at a distance less than D = " + D + ".");
            }

            newBusyScooterPosition = scooter.getPosition();
            ret = scooter.getPosition();
            scooter.setBusy(true);
            scooter.getLock().unlock();

            waitForNewEvent.signal();
            positionsLock.unlock();

            Reservation reservation = new Reservation(scooter);
            int cod = reservation.getCod();
            reservations.put(cod,reservation);

            return new Tuple<>(cod,ret);
        } finally {
            reservationsLock.unlock();
        }
    }

    /* Parking a scooter giving the reservation code and location, returns true if reward else false, and the value */
    public Tuple<Boolean,Double> concludeReservation(int codRes, Point local) throws ReservationNotFoundException, PointNotValid {
        if ((local.getX() < 1 || local.getX() > 20) || (local.getY() < 1 || local.getY() > 20)) throw new PointNotValid("The Point is not valid");

        Reservation res;
        double value;

        reservationsLock.lock();
        if (!reservations.containsKey(codRes)) {
            reservationsLock.unlock();
            throw new ReservationNotFoundException("No reservation available with the code " + codRes + ".");
        }
        res = reservations.remove(codRes);
        Scooter scooter = res.getScooter();
        positionsLock.lock();
        scooter.getLock().lock();
        scooter.setPosition(local);
        reservationsLock.unlock();


        boolean isReward = isReward(scooter.getPosition(),local);

        newFreeScooterPosition = scooter.getPosition();
        waitForNewEvent.signal();
        positionsLock.unlock();

        scooter.setBusy(false);
        Duration duration = Duration.between(res.getTime(), LocalDateTime.now());
        if (isReward) {
            value = scooter.distanceToPosition(local) * 0.15;
        }
        else {
            value = scooter.distanceToPosition(local) * 0.15 + duration.toMinutes() * 0.15;
        }

        value = (double) Math.round(value * 100) / 100;
        scooter.setPosition(local);
        scooter.getLock().unlock();

        return new Tuple<>(isReward,value);
    }

    public boolean isReward (Point initPoint, Point finalPoint) {

        boolean found = false;

        try {
            rewardsLock.lock();

            for (int i=0; i<rewards.size() && !found; i++) {
                if (rewards.get(i).comparePoints(initPoint,finalPoint)) {
                    found = true;
                }
            }

            if (found) {
                Reward r;
                Iterator<Reward> it = rewards.iterator();
                while (it.hasNext()) {
                    r = it.next();
                    if (r.getFinalPoint().equals(finalPoint)) {
                        it.remove();
                    }
                }
                waitForNewRewards.signalAll();
            }

        } finally {
            rewardsLock.unlock();
        }
        return found;
    }

    public void start() {
        new Thread(() -> {
            try {
                positionsLock.lock();

                while (true) {
                        /* Enquanto não existir nova reserva ou reserva existente ser concluida */
                        while (newBusyScooterPosition == null && newFreeScooterPosition == null) {
                            waitForNewEvent.await();
                        }

                        if (newBusyScooterPosition != null) {
                            boolean someoneFree = false; /* Assumir que não existe uma trotinete livre num raio D da posição da nova trotinete ocupada */

                            for (int i = 0; i<S; i++) {
                                scooters[i].getLock().lock();
                            }

                            for (int i=0; i<S && !someoneFree; i++) {
                                if (!scooters[i].isBusy() && scooters[i].distanceToPosition(newBusyScooterPosition)<=D) {
                                    someoneFree = true;  /* Encontrou uma trotinete livre num raio D da posição da nova trotinete ocupada */
                                }
                            }

                            if (!someoneFree) {  /* Como não existe uma trotinete livre num raio D da posição da trotinete ocupada criamos recompensas */
                                rewardsLock.lock();
                                for (int i=0; i<S; i++) {
                                    if (!scooters[i].isBusy()) {
                                        Point pos = scooters[i].getPosition();
                                        if (pos.distance(newBusyScooterPosition)>D) {
                                            rewards.add(new Reward(pos, newBusyScooterPosition));
                                        }
                                    }
                                }
                                waitForNewRewards.signalAll(); /* Acordar quem espera por novas reservas para enviar notificações */
                                rewardsLock.unlock();
                            }

                            for (int i = 0; i<S; i++) {
                                scooters[i].getLock().unlock();
                            }

                            newBusyScooterPosition = null;

                        } else { /* Reserva concluida */
                            boolean newRewards = false;

                            for(int k=0; k<S; k++) {
                                scooters[k].getLock().lock();
                            }
                            rewardsLock.lock();
                            for(int i=1; i<=N;i++) {
                                for (int j=1; j<=N; j++) { /* Percorrer todas as posições do mapa */

                                    /* Verificar se existe alguma trotinete livre no raio D desta posição */
                                    boolean someoneFree = false;

                                    for(int k=0; k<S; k++) {
                                        if (!scooters[k].isBusy() && scooters[k].distanceToPosition(new Point(i,j)) <= D) {
                                            someoneFree = true;
                                        }
                                    }

                                    if (!someoneFree) {
                                        rewards.add(new Reward(newFreeScooterPosition, new Point(i,j)));
                                        newRewards = true;
                                    }
                                }
                            }
                            if (newRewards) { waitForNewRewards.signalAll(); }
                            rewardsLock.unlock();

                            for(int k=0; k<S; k++) {
                                scooters[k].getLock().unlock();
                            }

                            newFreeScooterPosition = null;
                        }
                }

            } catch (InterruptedException e) {
                throw new RuntimeException(e);
            } finally {
                positionsLock.unlock();
            }
        }).start();
    }

    public void cancelNotifications(User u) {
        try {
            writeLockUsers.lock();
            users.get(u.getUsername()).setNotifications(false);
            rewardsLock.lock();
            waitForNewRewards.signalAll();
            rewardsLock.unlock();
        } finally {
            writeLockUsers.unlock();
        }
    }

    public void activateNotifications(User u) {
        try {
            writeLockUsers.lock();
            users.get(u.getUsername()).setNotifications(true);
        } finally {
            writeLockUsers.unlock();
        }
    }

    public boolean isNotificationsEnable(User u) {
        try {
            readLockUsers.lock();
            return users.get(u.getUsername()).getNotification();
        } finally {
            readLockUsers.unlock();
        }
    }

    public RewardList getNotifications(User u, Point p) throws PointNotValid {
        RewardList rewardList = new RewardList();

        try {
            rewardsLock.lock();
            int rewardsSize = rewards.size();

            while (rewardsSize == rewards.size() && isNotificationsEnable(u)) {
                waitForNewRewards.await();
                if (rewards.size() < rewardsSize) {
                    rewardsSize = rewards.size();
                }
            }

            if (isNotificationsEnable(u)) {
                for (int i = rewardsSize; i < rewards.size(); i++) {
                    if (rewards.get(i).getInitPoint().distance(p) <= D) {
                        rewardList.add(rewards.get(i));
                    }
                }
            }

        } catch (InterruptedException e) {
            throw new RuntimeException(e);
        } finally {
            rewardsLock.unlock();
        }
        return rewardList;
    }


}
