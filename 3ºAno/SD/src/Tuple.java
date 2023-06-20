public class Tuple<V1,V2> {
    private V1 value1;
    private V2 value2;

    public Tuple(V1 value1, V2 value2) {
        this.value1 = value1;
        this.value2 = value2;
    }

    public V1 getValue1() {
        return this.value1;
    }

    public V2 getValue2() {
        return this.value2;
    }

    public void setValue1(V1 value) {
        this.value1 = value;
    }

    public void setValue2(V2 value) {
        this.value2 = value;
    }

    @Override
    public String toString() {
        return "(" + value1 + "," +  value2  + ")";
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;

        Tuple<?, ?> tuple = (Tuple<?, ?>) o;
        return this.value1.equals(tuple.value1) && this.value2.equals(tuple.value2);
    }
}
