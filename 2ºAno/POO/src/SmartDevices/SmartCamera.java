package SmartDevices;

import java.io.Serializable;
import java.time.LocalDate;
import java.time.Period;

public class SmartCamera extends SmartDevice implements Serializable {

        private Resolution res;
        public double fileSize;

        /**
         * Constructor for objects of class com.company.SmartDevices.SmartCamera
         */
        public SmartCamera() {
            super();
            this.res = new Resolution();
            this.fileSize = 0;
        }

        public SmartCamera(boolean on, double consumo, double custo, int linhas, int colunas, double fileSize) throws SmartDeviceException, ResolutionException {
            super(on,consumo,custo);
            this.res = new Resolution(linhas,colunas);
            if (fileSize<=0) throw new SmartDeviceException("O tamanho do ficherio da SmartCamera é um valor positivo.");
            this.fileSize = fileSize;
        }

        public SmartCamera(double consumo, int linhas, int colunas, double fileSize) throws SmartDeviceException, ResolutionException {
            super(false,consumo,SmartDevice.defaultCusto);
            this.res = new Resolution(linhas,colunas);
            if (fileSize<=0) throw new SmartDeviceException("O tamanho do ficherio da SmartCamera é um valor positivo.");
            this.fileSize = fileSize;
        }

        public SmartCamera (SmartCamera s) {
            super(s);
            this.res = s.getResolution();
            this.fileSize = s.getFileSize();
        }

        /**
         * Getters and Setters
         */
        public double getFileSize() {
            return fileSize;
        }

        public void setFileSize(double fileSize) throws SmartDeviceException {
            if (fileSize<=0) throw new SmartDeviceException("O tamanho do ficherio da SmartCamera é um valor positivo.");
            this.fileSize = fileSize;
        }

        public Resolution getResolution() {
            return this.res.clone();
        }

        public void setResolution(Resolution resolution) {
            this.res = resolution.clone();
        }

        public double calculateconsumption (LocalDate fromDate, LocalDate toDate) {
            double r = 0;
            if (this.getOn()) {
                Period diff = Period.between(fromDate,toDate);
                r = this.getConsumoDiario()*diff.getDays();
            }
            return r;
        }

        public void setConsumoCam() throws SmartDeviceException {
            this.setConsumoDiario(this.fileSize*this.res.getLinhas()*this.res.getLinhas()/(1024*1024));
        }

        public SmartCamera clone () {
            return new SmartCamera(this);
        }

        @Override
        public boolean equals(Object o) {
            if (this == o) return true;
            if (o == null || getClass() != o.getClass()) return false;
            if (!super.equals(o)) return false;
            SmartCamera sc = (SmartCamera) o;
            return this.res.equals(sc.getResolution())
                   && this.fileSize == sc.getFileSize();
        }

        @Override
        public String toString() {
            StringBuilder sb = new StringBuilder("Camera:");
            sb.append("id=").append(this.getID());
            sb.append("; on=").append(this.getOn());
            sb.append("; consumo diario=").append(this.getConsumoDiario());
            sb.append("; custo de instalacao=").append(this.getCustoInstalacao());
            sb.append("; resolucao=").append(this.res);
            sb.append("; tamanho do ficheiro=").append(this.fileSize);
            return sb.toString();
        }
}



