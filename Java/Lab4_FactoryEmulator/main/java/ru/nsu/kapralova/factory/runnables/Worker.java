package ru.nsu.kapralova.factory.runnables;

import ru.nsu.kapralova.factory.Factory;
import ru.nsu.kapralova.factory.details.Storage;
import ru.nsu.kapralova.factory.details.Accessories;
import ru.nsu.kapralova.factory.details.Body;
import ru.nsu.kapralova.factory.details.Car;
import ru.nsu.kapralova.factory.details.Engine;

import java.util.concurrent.TimeUnit;

public class Worker implements Runnable {
    private final Storage<Accessories> accessoriesStorage;
    private final Storage<Body> bodyStorage;
    private final Storage<Engine> engineStorage;
    private final Storage<Car> carStorage;
    private final Object detector;


    public Worker(Storage<Accessories> accessoriesStorage, Storage<Body> bodyStorage,
                  Storage<Engine> engineStorage, Storage<Car> carStorage, Object detector) {
        this.bodyStorage = bodyStorage;
        this.carStorage = carStorage;
        this.engineStorage = engineStorage;
        this.accessoriesStorage = accessoriesStorage;
        this.detector = detector;
    }

    @Override
    public void run() {
        while (true) {
            synchronized (detector){
                try {
                    detector.wait();
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
            Body body = bodyStorage.get();
            Engine engine = engineStorage.get();
            Accessories accessories = accessoriesStorage.get();
            try {
                TimeUnit.SECONDS.sleep(Factory.values.WORKER_TIME().getSeconds());
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            Car car = new Car((int) (Math.random()*1000), body, engine, accessories);
            Factory.logger.info("Worker " + Thread.currentThread().getId() + " собрал автомобиль: " + car.CID);
            carStorage.put(car);
        }
    }
}
