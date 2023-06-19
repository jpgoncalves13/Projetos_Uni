package Simulation;

public class ComunityProgram {
    public static void main(String[] args) {
        ComunityModel model = new ComunityModel();
        ComunityController controller = new ComunityController(model);
        ComunityView view = new ComunityView(controller);

        view.run();

        ComunityView.closeScanner();
    }

}
