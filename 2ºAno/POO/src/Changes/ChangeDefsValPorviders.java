package Changes;

import java.io.Serializable;

public class    ChangeDefsValPorviders implements ComunityChanges, Serializable {
    private double valorBaseKw;
    private double imposto;

    public ChangeDefsValPorviders(double valorBaseKw, double imposto) {
        this.valorBaseKw = valorBaseKw;
        this.imposto = imposto;
    }

    public ChangeDefsValPorviders(ChangeDefsValPorviders p) {
        this.valorBaseKw = p.valorBaseKw;
        this.imposto = p.imposto;
    }

    public double getValorBaseKw() {
        return valorBaseKw;
    }

    public void setValorBaseKw(double valorBaseKw) {
        this.valorBaseKw = valorBaseKw;
    }

    public double getImposto() {
        return imposto;
    }

    public void setImposto(double imposto) {
        this.imposto = imposto;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        ChangeDefsValPorviders that = (ChangeDefsValPorviders) o;
        return this.valorBaseKw == that.getValorBaseKw()
               && this.imposto == that.getImposto();
    }

    @Override
    public String toString() {
        final StringBuilder sb = new StringBuilder("Alterar valores dos Forncedores:");
        sb.append("valorBaseKw=").append(valorBaseKw);
        sb.append("e imposto=").append(imposto);
        return sb.toString();
    }

    public ChangeDefsValPorviders clone() {
        return new ChangeDefsValPorviders(this);
    }

}
