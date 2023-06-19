package Fornecedores;

import java.util.Comparator;

public class ComparatorFornecedoresCustoDesc implements Comparator<FornecedoresEnergia> {
    public int compare (FornecedoresEnergia f1, FornecedoresEnergia f2) {
        return (int) (f2.totalCost()- f1.totalCost());
    }
}
