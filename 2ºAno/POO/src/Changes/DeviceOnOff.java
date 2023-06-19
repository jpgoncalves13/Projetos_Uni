package Changes;

import java.io.Serializable;

public class DeviceOnOff implements ComunityChanges, Serializable {
    private int deviceId;
    private int houseId;

    private boolean on;

    public DeviceOnOff(int deviceId, int houseId, boolean on) {
        this.deviceId = deviceId;
        this.houseId = houseId;
        this.on = on;
    }

    public DeviceOnOff(DeviceOnOff p) {
        this.deviceId = p.deviceId;
        this.houseId = p.houseId;
        this.on = p.on;
    }

    public int getDeviceId() {
        return deviceId;
    }

    public void setDeviceId(int deviceId) {
        this.deviceId = deviceId;
    }

    public int gethouseId() {
        return houseId;
    }

    public void sethouseId(int houseId) {
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
        DeviceOnOff that = (DeviceOnOff) o;
        return this.deviceId == that.getDeviceId()
               && this.houseId == that.gethouseId()
               && this.on == that.isOn();
    }

    @Override
    public String toString() {
        final StringBuilder sb = new StringBuilder("Ligar/desligar dispositivo com id ");
        sb.append(deviceId);
        sb.append(" da casa com número de porta ").append(houseId);
        return sb.toString();
    }

    public DeviceOnOff clone() {
        return new DeviceOnOff(this);
    }

}
