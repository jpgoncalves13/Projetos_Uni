package Fornecedores;

import net.objecthunter.exp4j.Expression;
import net.objecthunter.exp4j.ExpressionBuilder;

import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;
import java.io.Serializable;

public class FornecedoresEnergia implements Comparable<FornecedoresEnergia>, Serializable{

    private String nome;
    private static double valorBasekwh = 0.14800;
    private static double imposto = 0.23;
    private String formula;
    private List<Fatura> faturas;


    public FornecedoresEnergia(){
        this.nome = "";
        this.formula = "24*x*(c/1000)*(1+y)";
        this.faturas = new ArrayList<>();
    }


    public FornecedoresEnergia(String nome, String formula){
        this.nome = nome;
        this.formula = formula;
        this.faturas = new ArrayList<>();
    }

    public FornecedoresEnergia(String nome){
        this.nome = nome;
        this.formula = "x*(c/1000)*(1+y)";
        this.faturas = new ArrayList<>();
    }

    public FornecedoresEnergia(FornecedoresEnergia fornecedor){
        this.nome = fornecedor.getNome();
        this.formula = fornecedor.getFormula();
        this.faturas = fornecedor.getFaturas();
    }

    public String getNome() {
        return nome;
    }

    public void setNome(String nome) {
        this.nome = nome;
    }

    public static double getValorBasekwh() {
        return valorBasekwh;
    }

    public static double getImposto(){
        return imposto;
    }

    public static void setValorBasekwh(double valorBasekwh) {
        FornecedoresEnergia.valorBasekwh = valorBasekwh;
    }

    public static void setImposto(double imposto) {
        FornecedoresEnergia.imposto = imposto;
    }

    public String getFormula() {
        return formula;
    }

    public void setFormula(String formula) {
        this.formula = formula;
    }

    public List<Fatura> getFaturas() {
        return faturas.stream().map(Fatura::clone).collect(Collectors.toList());
    }

    public void setFaturas(List<Fatura> faturas) {
        this.faturas = faturas.stream().map(Fatura::clone).collect(Collectors.toList());
    }

    public void addFatura(Fatura f){
        this.faturas.add(f.clone());
    }

    public double totalCost () {
        return this.faturas.stream().mapToDouble(Fatura::getCusto).sum();
    }

    public double calculateFaturation (double consumption) {
        Expression e = new ExpressionBuilder(this.formula)
                .variables("x","y","c")
                .build()
                .setVariable("x",valorBasekwh)
                .setVariable("c",consumption)
                .setVariable("y",imposto);

        return e.evaluate();
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || this.getClass()!=o.getClass()) return false;
        FornecedoresEnergia that = (FornecedoresEnergia) o;
        return that.getNome().equals(this.nome)
               && that.getFormula().equals(this.formula)
               && that.getFaturas().equals(this.faturas);
    }

    public FornecedoresEnergia clone() {
        return new FornecedoresEnergia(this);
    }

    @Override
    public String toString() {
        final StringBuilder sb = new StringBuilder("{Provider:");
        sb.append("nome='").append(nome).append('\'');
        sb.append(", valorBasekwh=").append(valorBasekwh);
        sb.append(", imposto=").append(imposto);
        sb.append(", formula=").append(formula);
        sb.append(", faturas=").append(faturas);
        sb.append('}');
        return sb.toString();
    }

    @Override
    public int compareTo(FornecedoresEnergia o) {
        int r;
        return (int) (this.totalCost()-o.totalCost());
    }
}
