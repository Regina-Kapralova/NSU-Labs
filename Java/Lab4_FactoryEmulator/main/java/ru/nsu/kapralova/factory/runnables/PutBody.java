package ru.nsu.kapralova.factory.runnables;

import ru.nsu.kapralova.factory.Factory;
import ru.nsu.kapralova.factory.details.Storage;
import ru.nsu.kapralova.factory.details.Body;

import java.util.concurrent.TimeUnit;

public class PutBody implements Runnable {
    private final Storage<Body> bodyStorage;

    public PutBody(Storage<Body> bodyStorage) {
        this.bodyStorage = bodyStorage;
    }

    @Override
    public void run() {
        while (true) {
            try {
                TimeUnit.SECONDS.sleep(Factory.values.BODY_TIME().getSeconds());
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            bodyStorage.put(new Body((int) (Math.random() * 1000)));
        }
    }
}
