package ru.nsu.kapralova.factory.runnables;

import ru.nsu.kapralova.factory.details.Storage;
import ru.nsu.kapralova.factory.details.Car;

public class Controller implements Runnable {
    private final Storage<Car> carStorage;
    private final Detector detector;

    public Controller(Storage<Car> carStorage, Detector detector) {
        this.carStorage = carStorage;
        this.detector = detector;
    }

    @Override
    public void run() {
        while (true) {
            synchronized (carStorage){
                while(carStorage.getSize() >= carStorage.getCapacity()){
                    try {
                        carStorage.wait();
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
                synchronized (detector){ detector.notify(); }
            }
        }
    }
    static public class Detector{};

}
