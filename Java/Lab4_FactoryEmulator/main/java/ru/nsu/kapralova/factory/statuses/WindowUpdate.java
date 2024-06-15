package ru.nsu.kapralova.factory.statuses;

import ru.nsu.kapralova.factory.details.Storage;
import ru.nsu.kapralova.factory.details.Accessories;
import ru.nsu.kapralova.factory.details.Body;
import ru.nsu.kapralova.factory.details.Car;
import ru.nsu.kapralova.factory.details.Engine;

public class WindowUpdate {
    static public void update(Window window, Values values, final Storage<Engine> engineStorage, final Storage<Body> bodyStorage,
                             final Storage<Car> carStorage, final Storage<Accessories> accessoriesStorage){
        Thread update = new Thread(() -> {
            while (true){
                try {
                    Thread.sleep(500);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                values.setACCESSORIES_TIME(window.getACCESSORIES_TIME());
                values.setBODY_TIME(window.getBODY_TIME());
                values.setWORKER_TIME(window.getWORKER_TIME());
                values.setENGINE_TIME(window.getENGINE_TIME());
                values.setDEALER_TIME(window.getDEALER_TIME());

                window.showCountEngine(engineStorage.getSize(), engineStorage.getCapacity());
                window.showCountBody(bodyStorage.getSize(), bodyStorage.getCapacity());
                window.showCountCar(carStorage.getSize(), carStorage.getCapacity());
                window.showCountAccessories(accessoriesStorage.getSize(), accessoriesStorage.getCapacity());
            }
        });
        update.start();
    }
}
