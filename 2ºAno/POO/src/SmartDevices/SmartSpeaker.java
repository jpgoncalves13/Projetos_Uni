package SmartDevices;

import java.io.Serializable;
import java.time.LocalDate;
import java.time.Period;

/**
 *
 * Um SmartSpeaker é um SmartDevice que além de ligar e desligar permite também reproduzir som.
 * Consegue ligar-se a um canal (por simplificação uma rádio online) e permite
 * a regulação do seu nível de volume.
 *
 */
public class SmartSpeaker extends SmartDevice implements Serializable {
    public static final int MAX = 100; //volume máximo
    public static final int MIN = 0; //volume mínimo
    public static final int defaultVolume = 50;
    public static double volumeFactor = 0.02;
    private int volume;
    private String channel;
    private String marca;

    /**
     * Constructor for objects of class SmartSpeaker
     */
    public SmartSpeaker() {
        super();
        this.volume = defaultVolume;
        this.channel = "";
        this.marca = "";
    }

    public SmartSpeaker(boolean on, double consumo, double custo, int vol, String channel,String marca) throws SmartDeviceException {
        super(on,consumo,custo);
        if (vol<0 || vol>100) throw new SmartDeviceException("O volume definido para o SmartSpeaker não é válido.");
        this.volume = vol;
        this.channel = channel;
        this.marca = marca;
    }

    public SmartSpeaker(double consumo, int vol, String channel,String marca) throws SmartDeviceException {
        super(false,consumo,SmartDevice.defaultCusto);
        if (vol<0 || vol>100) throw new SmartDeviceException("O volume definido para o SmartSpeaker não é válido.");
        this.volume = vol;
        this.channel = channel;
        this.marca = marca;
    }

    public SmartSpeaker (SmartSpeaker s) {
        super(s);
        this.volume = s.getVolume();
        this.channel = s.getChannel();
        this.marca = s.getMarca();
    }

    /**
     * Getters and setters
     */

    public int getVolume() {
        return this.volume;
    }

    public void setVolume(int vol) throws SmartDeviceException {
        if (vol<0 || vol>100) throw new SmartDeviceException("O volume definido para o SmartSpeaker não é válido.");
        this.volume = vol;
    }
    
    public String getChannel() {
        return this.channel;
    }
    
    public void setChannel(String c) {
        this.channel = c;
    }

    public String getMarca() {
        return marca;
    }

    public void setMarca(String marca) {
        this.marca = marca;
    }

    public void volumeUp() {
        if (this.volume<MAX) this.volume++;
    }

    public void volumeDown() {
        if (this.volume>0) this.volume--;
    }

    public double calculateconsumption (LocalDate fromDate, LocalDate toDate) {
        double r = 0;
        if (this.getOn()) {
            Period diff = Period.between(fromDate, toDate);
            r = (this.getConsumoDiario() + volumeFactor * this.volume) * diff.getDays();
        }
        return r;
    }

    @Override
    public SmartSpeaker clone () {
        return new SmartSpeaker(this);
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        if (!super.equals(o)) return false;
        SmartSpeaker that = (SmartSpeaker) o;
        return this.volume == that.getVolume() &&
                this.channel.equals(that.getChannel()) &&
                this.marca.equals(that.getMarca());
    }

    @Override
    public String toString() {
        final StringBuilder sb = new StringBuilder("{Speaker: ");
        sb.append("id=").append(this.getID());
        sb.append(", on=").append(this.getOn());
        sb.append(", consumo diario=").append(this.getConsumoDiario());
        sb.append(", custo de instalacao=").append(this.getCustoInstalacao());
        sb.append(", volume=").append(this.volume);
        sb.append(", channel=").append(this.channel);
        sb.append(", marca=").append(this.marca);
        sb.append('}');
        return sb.toString();
    }
}
