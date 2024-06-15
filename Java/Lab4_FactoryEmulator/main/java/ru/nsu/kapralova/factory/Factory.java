package ru.nsu.kapralova.factory;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import ru.nsu.kapralova.factory.statuses.Values;
import ru.nsu.kapralova.factory.statuses.Window;
import ru.nsu.kapralova.factory.statuses.WindowUpdate;
import ru.nsu.kapralova.factory.details.*;
import ru.nsu.kapralova.factory.runnables.*;
import ru.nsu.kapralova.pool.ThreadPool;
import java.io.FileInputStream;
import java.io.IOException;
import java.util.Properties;

public class Factory {
    public static Values values = new Values();
    public static Logger logger = LoggerFactory.getLogger(Factory.class);

    public static void main(String[] args) {
        Properties property = new Properties();
        try {
            property.load(new FileInputStream("src/main/resources/config.properties"));
        } catch (IOException e) {
            e.printStackTrace();
        }

        Storage<Accessories> accessoriesStorage = new Storage<>(Integer.parseInt(property.getProperty("StorageAccessorySize")));
        Storage<Body> bodyStorage = new Storage<>(Integer.parseInt(property.getProperty("StorageBodySize")));
        Storage<Engine> engineStorage = new Storage<>(Integer.parseInt(property.getProperty("StorageMotorSize")));
        Storage<Car> carStorage = new Storage<>(Integer.parseInt(property.getProperty("StorageAutoSize")));

        Controller.Detector detector = new Controller.Detector();
        ThreadPool controllersThreadPool = new ThreadPool(Integer.parseInt(property.getProperty("Controllers")));

        Window window = new Window();
        WindowUpdate.update(window, values, engineStorage, bodyStorage, carStorage, accessoriesStorage);

        ThreadPool accessoriesThreadPool = new ThreadPool(Integer.parseInt(property.getProperty("AccessorySuppliers")));
        for (int i = 0; i < Integer.parseInt(property.getProperty("AccessorySuppliers")); i++)
            accessoriesThreadPool.execute(new PutAccessories(accessoriesStorage));

        ThreadPool bodyThreadPool = new ThreadPool(Integer.parseInt(property.getProperty("EngineSuppliers")));
        bodyThreadPool.execute(new PutBody(bodyStorage));

        ThreadPool engineThreadPool = new ThreadPool(Integer.parseInt(property.getProperty("BodySuppliers")));
        engineThreadPool.execute(new PutEngine(engineStorage));

        ThreadPool workersThreadPool = new ThreadPool(Integer.parseInt(property.getProperty("Workers")));
        for (int i = 0; i < Integer.parseInt(property.getProperty("Workers")); i++)
            workersThreadPool.execute(new Worker(accessoriesStorage, bodyStorage, engineStorage, carStorage, detector));

        ThreadPool dealersThreadPool = new ThreadPool(Integer.parseInt(property.getProperty("Dealers")));
        for (int i = 0; i < Integer.parseInt(property.getProperty("Dealers")); i++)
            dealersThreadPool.execute(new GetCar(carStorage));

        controllersThreadPool.execute(new Controller(carStorage, detector));

//        accessoriesThreadPool.shutdown();
//        engineThreadPool.shutdown();
//        bodyThreadPool.shutdown();
//        workersThreadPool.shutdown();
//        dealersThreadPool.shutdown();
//        controllersThreadPool.shutdown();

    }
}
