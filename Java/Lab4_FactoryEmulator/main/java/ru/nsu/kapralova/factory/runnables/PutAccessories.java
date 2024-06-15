package ru.nsu.kapralova.factory.runnables;

import ru.nsu.kapralova.factory.Factory;
import ru.nsu.kapralova.factory.details.Storage;
import ru.nsu.kapralova.factory.details.Accessories;

import java.util.concurrent.TimeUnit;

public class PutAccessories implements Runnable {
    Storage<Accessories> accessoriesStorage;

    public PutAccessories(Storage<Accessories> accessoriesStorage){
        this.accessoriesStorage = accessoriesStorage;
    }

    @Override
    public void run() {
        while (true){
            try {
                TimeUnit.SECONDS.sleep(Factory.values.ACCESSORIES_TIME().getSeconds());
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            accessoriesStorage.put(new Accessories((int) (Math.random() * 1000)));
        }
    }
}
