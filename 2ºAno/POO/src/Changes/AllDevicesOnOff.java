package Changes;

import java.io.Serializable;

public class AllDevicesOnOff implements ComunityChanges, Serializable {
    private int houseId;
    private boolean on;

    public AllDevicesOnOff(int houseId, boolean on) {
        this.houseId = houseId;
        this.on = on;
    }

    public AllDevicesOnOff(AllDevicesOnOff p) {
        this.houseId = p.houseId;
        this.on = p.on;
    }

    public int getHouseId() {
        return houseId;
    }

    public void setHouseId(int houseId) {
        this.houseId = houseId;
    }

    public boolean isOn() {
        return on;
    }


    public void setOn(boolean on) {
        this.on = on;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        AllDevicesOnOff that = (AllDevicesOnOff) o;
        return this.houseId == that.getHouseId()
               && this.on == that.isOn();
    }

    @Override
    public String toString() {
        final StringBuilder sb = new StringBuilder("Ligar/Desligar todos os devices da casa");
        sb.append(houseId);
        return sb.toString();
    }

    public AllDevicesOnOff clone() {
        return new AllDevicesOnOff(this);
    }

}
