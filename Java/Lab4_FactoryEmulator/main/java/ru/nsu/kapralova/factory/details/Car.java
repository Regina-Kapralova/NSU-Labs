package ru.nsu.kapralova.factory.details;

public class Car {
    public Car(int CID, Body body, Engine engine, Accessories accessories) {
        this.CID = CID;
        this.body = body;
        this.engine = engine;
        this.accessories = accessories;
    }
    public final Body body;
    public final Engine engine;
    public final Accessories accessories;
    public final int CID;
}
