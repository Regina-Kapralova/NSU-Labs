package ru.nsu.kapralova.factory.details;

import java.util.Queue;
import java.util.concurrent.LinkedBlockingQueue;

public class Storage<T> {
    private final int capacity;
    private final Queue<T> queue;

    public Storage(int capacity){
        this.capacity = capacity;
        queue = new LinkedBlockingQueue<>(capacity);
    }

    public synchronized void put(T obj) {
        while (queue.size() >= capacity)
            try {
                this.wait();
            } catch (InterruptedException e) { e.printStackTrace(); }
        queue.add(obj);
        this.notifyAll();
    }

    public synchronized T get() {
        while (queue.size() < 1)
            try {
                this.wait();
            } catch (InterruptedException e) {e.printStackTrace(); }
        T obj = queue.remove();
        this.notify();
        return obj;
    }



    public synchronized int getSize(){
        return queue.size();
    }
    public synchronized int getCapacity(){
        return capacity;
    }
}
