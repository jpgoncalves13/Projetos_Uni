package Fornecedores;

import java.util.Comparator;

public class ComparatorFaturaConsumoDesc implements Comparator<Fatura> {
    public int compare(Fatura f1, Fatura f2) {
        return (int) (f2.getConsumo()-f1.getConsumo());
    }

}
