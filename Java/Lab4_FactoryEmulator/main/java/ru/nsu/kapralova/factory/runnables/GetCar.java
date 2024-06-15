package ru.nsu.kapralova.factory.runnables;

import ru.nsu.kapralova.factory.Factory;
import ru.nsu.kapralova.factory.details.Storage;
import ru.nsu.kapralova.factory.details.Car;

import java.util.concurrent.TimeUnit;

public class GetCar implements Runnable {
    private final Storage<Car> carStorage;

    public GetCar(Storage<Car> carStorage) {
        this.carStorage = carStorage;
    }

    @Override
    public void run() {
        while (true) {
            try {
                TimeUnit.SECONDS.sleep(Factory.values.DEALER_TIME().getSeconds());
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            synchronized (carStorage) {
                Car car = carStorage.get();
                Factory.logger.info("Получение: Dealer {}: Auto {} (Body: {} Motor: {} Accessory: {}) ",
                        Thread.currentThread().getId(), car.CID, car.body.BID, car.engine.EID, car.accessories.AID);
                carStorage.notify();
            }
        }
    }

}
