package Fornecedores;

import java.time.LocalDate;
import java.io.Serializable;
import java.util.Iterator;
import java.util.List;

public class Fatura implements Comparable<Fatura>,Serializable{

    private int houseId;
    private String nome;
    private int nif;
    private String fornecedor;
    private LocalDate from;
    private LocalDate to;
    private double consumo;
    private double custo;

    /**
     * Constructors
     */

    public Fatura() {
        this.nome = "";
        this.nif = 0;
        this.houseId = 0;
        this.fornecedor = "";
        this.from = LocalDate.now();
        this.to = LocalDate.now();
        this.consumo = 0;
        this.custo = 0;
    }

    public Fatura(String nome, int nif, int houseId, String fornecedor, LocalDate from, LocalDate to, double consumo, double custo) throws FaturaException {
        this.nome = nome;
        this.nif = nif;
        this.houseId = houseId;
        this.fornecedor = fornecedor;
        this.from = from;
        this.to = to;
        if (consumo<0) throw new FaturaException("O consumo da fatura tem de ser um valor positivo.");
        this.consumo = consumo;
        if (custo<0) throw new FaturaException("O custo da fatura tem de ser um valor positivo.");
        this.custo = custo;
    }

    public Fatura(Fatura f) {
        this.nome = f.getNome();
        this.nif = f.getNif();
        this.houseId = f.getHouseId();
        this.fornecedor = f.getFornecedor();
        this.from = f.getFrom();
        this.to = f.getTo();
        this.consumo = f.getConsumo();
        this.custo = f.getCusto();
    }

    /**
     *  Getters and Setters
     */

    public String getNome() {
        return nome;
    }

    public void setNome(String nome) {
        this.nome = nome;
    }

    public int getNif() {
        return nif;
    }

    public void setNif(int nif) {
        this.nif = nif;
    }

    public int getHouseId() {
        return houseId;
    }

    public void setHouseId(int houseId) {
        this.houseId = houseId;
    }

    public String getFornecedor() {
        return fornecedor;
    }

    public void setFornecedor(String fornecedor) {
        this.fornecedor = fornecedor;
    }

    public LocalDate getFrom() {
        return from;
    }

    public void setFrom(LocalDate from) {
        this.from = from;
    }

    public LocalDate getTo() {
        return to;
    }

    public void setTo(LocalDate to) {
        this.to = to;
    }

    public double getConsumo() {
        return consumo;
    }

    public void setConsumo(double consumo) {
        this.consumo = consumo;
    }

    public double getCusto() {
        return custo;
    }

    public void setCusto(double custo) {
        this.custo = custo;
    }

    @Override
    public Fatura clone() {
        return  new Fatura(this);
    }

    public int compareTo (Fatura f) {
        int r;
        if (this.getConsumo()>f.getConsumo()) r = 1;
        else if (this.getConsumo()==f.getConsumo()) r=0;
        else r=-1;
        return r;
    }

    public static String showFaturas (List<Fatura> faturas) {
        StringBuilder sb = new StringBuilder("\nFatura: Nome, Nif, Id da Casa, Fornecedor, Data Inicial, Data Final, Consumo, Total.\n\n");
        Iterator<Fatura> it = faturas.iterator();
        while (it.hasNext()) {
            sb.append(it.next().showFatura());
            sb.append("\n");
        }
        return sb.toString();
    }

    public String showFatura() {
        StringBuilder sb = new StringBuilder("Fatura: ");
        sb.append(nome);
        sb.append(", ").append(nif);
        sb.append(", ").append(houseId);
        sb.append(", ").append(fornecedor);
        sb.append(", ").append(from);
        sb.append(", ").append(to);
        sb.append(", ").append(consumo);
        sb.append(", ").append(custo);
        sb.append('.');
        return sb.toString();
    }


    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Fatura fatura = (Fatura) o;
        return fatura.getConsumo() == this.consumo
               && fatura.getNome().equals(this.nome)
               && fatura.getNif() == this.nif
               && fatura.getHouseId() == this.houseId
               && fatura.getFornecedor().equals(this.fornecedor)
               && fatura.getCusto() == this.custo
               && fatura.getFrom().isEqual(this.from)
               && fatura.getTo().isEqual(this.to);
    }

    @Override
    public String toString() {
        final StringBuilder sb = new StringBuilder("{Fatura: ");
        sb.append("nome= ").append(nome);
        sb.append(", nif= ").append(nif);
        sb.append(", número de porta= ").append(houseId);
        sb.append(", forncedor= ").append(fornecedor);
        sb.append(", from= ").append(from);
        sb.append(", to= ").append(to);
        sb.append(", consumo= ").append(consumo);
        sb.append(", custo= ").append(custo);
        sb.append('}');
        return sb.toString();
    }
}
