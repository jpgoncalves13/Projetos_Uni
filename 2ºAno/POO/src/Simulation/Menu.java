package Simulation;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

public class Menu {

    private List<String> opcoes;

    public Menu() {
        this.opcoes = new ArrayList<>();
    }
    public Menu(List<String> opcoes) {
        this.opcoes = opcoes;
    }

    public Menu(Menu m) {
        this.opcoes = m.getOpcoes();
    }

    public List<String> getOpcoes() {
        return new ArrayList<>(opcoes);
    }

    public void setOpcoes(List<String> opcoes) {
        this.opcoes = new ArrayList<>(opcoes);
    }

    public void addOption(String op) {
        this.opcoes.add(op);
    }

    @Override
    public String toString() {
        final StringBuilder sb = new StringBuilder("|---------------------------Menu--------------------------|\n");
        sb.append('\n');

        Iterator<String> it = this.opcoes.iterator();
        String op = null;
        int i = 0;
        while (it.hasNext()) {
            op = it.next();
            sb.append(i).append(". ");
            sb.append(op);
            sb.append('\n');
            i++;
        }

        sb.append('\n');
        sb.append("|---------------------------------------------------------|");
        return sb.toString();
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Menu menu = (Menu) o;
        return opcoes.equals(menu.getOpcoes());
    }

    public Menu clone() {
        return new Menu(this);
    }

}
