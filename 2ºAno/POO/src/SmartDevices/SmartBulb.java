package SmartDevices;

import java.io.Serializable;
import java.time.Period;
import java.time.LocalDate;

/**
 * Uma SmartBulb é uma lâmpada inteligente que além de ligar e desligar (já que
 * é subclasse de SmartDevice) também permite escolher a intensidade da iluminação
 * (a cor da mesma).
 *
 * @author (your name)
 * @version (a version number or a date)
 */
public class SmartBulb extends SmartDevice implements Serializable {
    public static final int WARM = 2;
    public static final int NEUTRAL = 1;
    public static final int COLD = 0;
    public static final double defaultDim = 0.5;
    private int tone;
    public double dimensao; // em cm

    /**
     * Constructor for objects of class SmartBulb
     */
    public SmartBulb() {
        super();
        this.tone = NEUTRAL;
        this.dimensao = defaultDim;
    }

    public SmartBulb(boolean on, double consumo, double custo, int tone, double dim) throws SmartDeviceException {
        super(on,consumo,custo);
        if (tone>WARM || tone<COLD) throw new SmartDeviceException("O tom definido para criar a SmartBulb não é valido.");
        this.tone = tone;
        if (dim<=0) throw new SmartDeviceException("A dimensão de uma lâmpada é um valor positivo.");
        this.dimensao = dim;
    }

    public SmartBulb(double consumo, int tone, double dim) throws SmartDeviceException {
        super(false,consumo,SmartDevice.defaultCusto);
        if (tone>WARM || tone<COLD) throw new SmartDeviceException("O tom definido para criar a SmartBulb não é valido.");
        this.tone = tone;
        if (dim<=0) throw new SmartDeviceException("A dimensão de uma lâmpada é um valor positivo.");
        this.dimensao = dim;
    }


    public SmartBulb (SmartBulb s) {
        super(s);
        this.tone = s.tone;
        this.dimensao = s.dimensao;
    }

    /**
     * Getters and Setters
     */
    public void setTone(int tone) throws SmartDeviceException {
        if(tone>WARM || tone<COLD) throw new SmartDeviceException("O tom definido para a SmartBulb não é valido.");
        this.tone = tone;
    }

    public int getTone() {
        return this.tone;
    }

    public double getDimensao() {
        return dimensao;
    }

    public void setDimensao(double dimensao) throws SmartDeviceException {
        if (dimensao<=0) throw new SmartDeviceException("A dimensão de uma lâmpada é um valor positivo.");
        this.dimensao = dimensao;
    }

    public double calculateconsumption (LocalDate fromDate, LocalDate toDate) {
        double r = 0;
        if (this.getOn()) {
            Period diff = Period.between(fromDate, toDate);
            double factor;
            if (this.tone == NEUTRAL) factor = 1;
            else if (this.tone == WARM) factor = 1.1;
            else factor = 0.9;
            r = diff.getDays()*this.getConsumoDiario()*factor;
        }
        return r;
    }

    public SmartBulb clone () {
        return new SmartBulb(this);
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        if (!super.equals(o)) return false;
        SmartBulb smartBulb = (SmartBulb) o;
        return this.tone == smartBulb.getTone()
                && this.dimensao == smartBulb.dimensao;
    }

    @Override
    public String toString() {
        final StringBuilder sb = new StringBuilder("{Bulb: ");
        sb.append("on=").append(getOn());
        sb.append(", id=").append(getID());
        sb.append(", tone=").append(tone);
        sb.append(", dimensao=").append(dimensao);
        sb.append(", consumoDiario=").append(getConsumoDiario());
        sb.append(", custoInstalacao=").append(getCustoInstalacao());
        sb.append('}');
        return sb.toString();
    }
}

