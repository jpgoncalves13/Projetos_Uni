package SmartDevices;

import java.io.Serializable;

public class Resolution implements Serializable {

    private static final int defaultLinhas=1920;
    private static final int defaultColunas=1080;
    private int linhas;
    private int colunas;

    public Resolution() {
        this.linhas = defaultLinhas;
        this.colunas = defaultColunas;
    }

    public Resolution(int linhas, int colunas) throws ResolutionException {
        if (linhas<=0 || colunas<=0) throw new ResolutionException("A resolução definida não é válida.");
        this.linhas = linhas;
        this.colunas = colunas;
    }

    public Resolution (Resolution res) {
        this.linhas = res.getLinhas();
        this.colunas = res.getColunas();
    }

    public int getLinhas() {
        return linhas;
    }

    public void setLinhas(int linhas) {
        this.linhas = linhas;
    }

    public int getColunas() {
        return colunas;
    }

    public void setColunas(int colunas) {
        this.colunas = colunas;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Resolution that = (Resolution) o;
        return this.colunas == that.colunas
               && this.linhas == that.linhas;
    }

    @Override
    public String toString() {
        final StringBuilder sb = new StringBuilder();
        sb.append(linhas);
        sb.append("x").append(colunas);
        return sb.toString();
    }

    public Resolution clone() {
        return new Resolution(this);
    }
}
