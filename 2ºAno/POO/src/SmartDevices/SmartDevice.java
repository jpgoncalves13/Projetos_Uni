package SmartDevices;

import java.io.Serializable;
import java.time.LocalDate;

/**
 * A classe SmartDevice é uma classe abstrata.
 * Permite a interação com dispositivos.
 *
 */
public abstract class SmartDevice implements Serializable {

    public static final double defaultConsumo = 9;
    public static final double defaultCusto = 10;

    private static int nextId = 1; // para auto incrementar o campo id
    private final int id;
    private boolean on;
    private double consumoDiario;  // em watts
    private double custoInstalacao; // em euros

    /**
     * Constructor for objects of class com.company.SmartDevices.SmartDevice
     */
    public SmartDevice() {
        this.id = nextId;
        nextId +=1;
        this.on = false;
        this.consumoDiario = defaultConsumo;
        this.custoInstalacao = defaultCusto;
    }

    public SmartDevice(boolean on, double consumo, double custo) throws SmartDeviceException {
        this.id = nextId;
        nextId+=1;
        this.on = on;
        if (consumo<=0) throw new SmartDeviceException("O consumo diário do SmartDevice é um valor positivo.");
        this.consumoDiario = consumo;
        if (custo<=0) throw new SmartDeviceException("O custo de instalação do SmartDevice é um valor positivo.");
        this.custoInstalacao = custo;
    }

    public SmartDevice (SmartDevice s) {
        this.on = s.getOn();
        this.id = s.getID();
        this.consumoDiario = s.getConsumoDiario();
        this.custoInstalacao = s.getCustoInstalacao();
    }

    /**
     * Getters and Setters
     */
    public void turnOn() {
        this.on = true;
    }
    
    public void turnOff() {
        this.on = false;
    }
    
    public boolean getOn() {
        return this.on;
    }

    public void setOn(boolean on) {
        this.on = on;
    }

    public double getConsumoDiario() {
        return consumoDiario;
    }

    public void setConsumoDiario(double consumoDiario) throws SmartDeviceException {
        if (consumoDiario<=0) throw new SmartDeviceException("O consumo diário do SmartDevice é um valor positivo.");
        this.consumoDiario = consumoDiario;
    }

    public int getID() {
        return this.id;
    }

    public double getCustoInstalacao() {
        return custoInstalacao;
    }

    public void setCustoInstalacao(double custoInstalacao) throws SmartDeviceException {
        if (custoInstalacao<=0) throw new SmartDeviceException("O custo de instalação do SmartDevice é um valor positivo.");
        this.custoInstalacao = custoInstalacao;
    }

    public abstract SmartDevice clone();
    public abstract double calculateconsumption (LocalDate fromDate, LocalDate toDate);

    @Override
    public boolean equals (Object o) {
        if (this == o) return true;
        if (o == null || this.getClass() != o.getClass()) return false;

        SmartDevice sd = (SmartDevice) o;
        return this.on == sd.getOn() &&
                this.id == sd.getID() &&
                this.consumoDiario == sd.getConsumoDiario() &&
                this.custoInstalacao == sd.getCustoInstalacao();
    }

    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder("{Device:");
        sb.append("id=").append(id);
        sb.append(", on=").append(on);
        sb.append(", consumo diario=").append(consumoDiario);
        sb.append(", custo de instalacao=").append(custoInstalacao);
        sb.append('}');
        return sb.toString();
    }
}
