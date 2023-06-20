import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.util.List;

public class User {
    private final String username;
    private final String password;
    private boolean notifications;

    private boolean logged;

    private List<Scooter> scooters;

    public User(String username, String password){
        this.username = username;
        this.password = password;
        this.notifications = false;
        this.logged = false;
    }

    public User(String username, String password, boolean notifications, boolean logged){
        this.username = username;
        this.password = password;
        this.notifications = notifications;
        this.logged = logged;
    }

    public boolean isLogged() {
        return logged;
    }

    public void setLogged(boolean logged) {
        this.logged = logged;
    }

    public String getUsername() {
        return this.username;
    }

    public String getPassword() {
        return this.password;
    }

    public boolean getNotification() { return this.notifications; }

    public void setNotifications(boolean option){
        this.notifications=option;
    }

    public void serialize (DataOutputStream dos) throws IOException {
        dos.writeUTF(username);
        dos.writeUTF(password);
    }

    public static User deserialize (DataInputStream dis) throws IOException {
        String un = dis.readUTF();
        String ps = dis.readUTF();
        return new User(un,ps);
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (!(o instanceof User user)) return false;
        return  getUsername().equals(user.getUsername()) &&
                getPassword().equals(user.getPassword()) &&
                getNotification() == user.getNotification() &&
                isLogged() == user.isLogged();
    }

    @Override
    public User clone() {
        return new User(this.getUsername(), this.getPassword(), this.getNotification(), this.isLogged());
    }
}
