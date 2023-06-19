package Changes;

import java.io.Serializable;

public class ChangeProvider implements ComunityChanges, Serializable{
    private int houseId;
    private String providerName;

    public ChangeProvider(int houseId, String providerName) {
        this.houseId = houseId;
        this.providerName = providerName;
    }

    public ChangeProvider(ChangeProvider p) {
        this.houseId = p.getHouseId();
        this.providerName = p.getProviderName();
    }

    public int getHouseId() {
        return houseId;
    }

    public void setHouseId(int houseId) {
        this.houseId = houseId;
    }

    public String getProviderName() {
        return providerName;
    }

    public void setProviderName(String providerName) {
        this.providerName = providerName;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        ChangeProvider that = (ChangeProvider) o;
        return this.providerName.equals(that.getProviderName())
               && this.houseId == that.getHouseId();
    }

    @Override
    public String toString() {
        final StringBuilder sb = new StringBuilder("Alterar fornecedor da casa ");
        sb.append(houseId);
        sb.append("para ").append(providerName);
        return sb.toString();
    }

    public ChangeProvider clone() {
        return new ChangeProvider(this);
    }

}
