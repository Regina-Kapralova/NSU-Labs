package ru.nsu.kapralova.factory.runnables;

import ru.nsu.kapralova.factory.Factory;
import ru.nsu.kapralova.factory.details.Storage;
import ru.nsu.kapralova.factory.details.Engine;

import java.util.concurrent.TimeUnit;

public class PutEngine implements Runnable {
    private final Storage<Engine> engineStorage;

    public PutEngine(Storage<Engine> engineStorage) {
        this.engineStorage = engineStorage;
    }

    @Override
    public void run() {
        while (true) {
            try {
                TimeUnit.SECONDS.sleep(Factory.values.ENGINE_TIME().getSeconds());
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            //System.out.println("Кладу в склад двигателей");
            engineStorage.put(new Engine((int) (Math.random() * 1000)));
        }
    }
}
