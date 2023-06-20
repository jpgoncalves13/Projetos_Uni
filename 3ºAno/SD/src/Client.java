import Exceptions.PointNotValid;
import Exceptions.ReservationNotFoundException;
import Exceptions.ScooterNotFoundException;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

public class Client {

    private static int clientInitialMenu(Scanner sc) {
        System.out.println("=================================");
        System.out.println("    WELCOME");
        System.out.println("    1 -> Login");
        System.out.println("    2 -> Register");
        System.out.println("=================================");
        System.out.print("Option: ");
        int opt = sc.nextInt(); sc.nextLine();
        System.out.print("\n");
        return opt;
    }

    private static int clientMenu(Scanner sc) {
        System.out.println("=================================");
        System.out.println("    1 - Free Scooters");
        System.out.println("    2 - Rewards");
        System.out.println("    3 - Reserve Scooter");
        System.out.println("    4 - Reserved Scooters");
        System.out.println("    5 - Park Scooter");
        System.out.println("    6 - Get Notifications");
        System.out.println("    7 - Cancel Notifications");
        System.out.println("    0 - Exit");
        System.out.println("=================================");
        System.out.print("Option: ");
        int opt = sc.nextInt(); sc.nextLine();
        System.out.print("\n");
        return opt;
    }

    private static User getData(Scanner sc) {
        System.out.print("Username: ");
        String username = sc.nextLine();
        System.out.print("Password: ");
        String password = sc.nextLine();
        System.out.print("\n");
        return new User(username,password);
    }

    private static Point getPoint(Scanner sc) {
        System.out.println("Enter the coordinates of the location");
        System.out.print("X: ");
        int x = sc.nextInt(); sc.nextLine();
        System.out.print("Y: ");
        int y = sc.nextInt(); sc.nextLine();
        return new Point(x,y);
    }

    private static int getCode(Scanner sc) {
        System.out.print("Enter the code of the reservation: ");
        int code = sc.nextInt(); sc.nextLine();
        return code;
    }

    private static String printScooters(List<Integer> list) {
        String str = "";
        for (int num : list) {
            if (str.equals("")) str = str + num;
            else{
                str = str + ", " + num;
            }
        }

        if (str.equals("")) str = "None";

        str += "\n";

        return str;
    }


    public static void main(String[] args) throws IOException, InterruptedException {
        Scanner sc = new Scanner(System.in);
        ClientLibrary cl = new Library();

        int opt = -1;
        boolean next = false;
        User u = null;

        while (!next) {
            opt = 0;
            while (opt != 1 && opt != 2) {
                opt = clientInitialMenu(sc);
                if (opt != 1 && opt != 2) {
                    System.out.println("Invalid Option\n");
                }
            }

            u = getData(sc);
            switch (opt) {
                case 1 -> {
                    next = cl.verifyUser(u);
                    if (!next) System.out.println("Utilizador ou Password incorretos \n");
                }
                case 2 -> {
                    next = cl.registerUser(u);
                    if (!next) System.out.println("Utilizador já registado \n");
                }
            }
        }

            List<Integer> listScooters = new ArrayList<>();

            while (opt != 0) {
                opt = -1;

                while (opt < 0 || opt > 7) {
                    opt = clientMenu(sc);
                    if (opt < 0 || opt > 7) {
                        System.out.println("Invalid Option");
                    }
                }

                switch (opt) {
                    case 1:
                        System.out.println(cl.freeScooters(getPoint(sc)) + "\n");
                        break;
                    case 2:
                        System.out.println(cl.getRewards(getPoint(sc)) + "\n");
                        break;
                    case 3:
                        try {
                            Tuple<Integer, Point> t = cl.newReservation(getPoint(sc));
                            listScooters.add(t.getValue1());
                            System.out.println("Scooter in position " + t.getValue2() + " reserved with code " + t.getValue1() + "\n");
                        } catch (ScooterNotFoundException | PointNotValid e) {
                            System.out.println(e.getMessage());
                        }
                        break;
                    case 4:
                        System.out.println("Reserve Codes are " + Client.printScooters(listScooters));
                        break;
                    case 5:
                        int code = getCode(sc);
                        if (listScooters.contains(code)) {
                            try {
                                Tuple<Boolean, Double> t = cl.concludeReservation(code, getPoint(sc));
                                if (t.getValue1()) {
                                    System.out.println("Reward completed. You earned " + t.getValue2() + "$\n");
                                } else {
                                    System.out.println("Reservation concluded. The cost is " + t.getValue2() + "$\n");
                                }
                            } catch (ReservationNotFoundException | PointNotValid e) {
                                System.out.println(e.getMessage());
                            }
                        }
                        else {
                            System.out.println("Client does not have a scooter with that code\n");
                        }
                        break;
                    case 6:
                        Point p = getPoint(sc);
                        User user = u.clone();
                        new Thread(() -> {
                            try {
                                System.out.println("\n" + cl.getNotifications(user, p));
                            } catch (IOException | InterruptedException e) {
                                throw new RuntimeException(e);
                            }
                        }).start();
                        System.out.print("\n");
                        break;

                    case 7:
                        cl.cancelNotifications(u);
                        break;

                    case 0:
                        cl.closeConnection(u);
                        break;
                }
            }
    }
}
