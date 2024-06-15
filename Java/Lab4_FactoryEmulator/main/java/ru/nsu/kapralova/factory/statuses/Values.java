package ru.nsu.kapralova.factory.statuses;

import java.time.Duration;

public class Values {
    private Duration BODY_TIME;
    private Duration ENGINE_TIME;
    private Duration ACCESSORIES_TIME;
    private Duration DEALER_TIME;
    private Duration WORKER_TIME;

    public Values(){
        BODY_TIME = Duration.ofSeconds(5);
        ENGINE_TIME = Duration.ofSeconds(5);
        ACCESSORIES_TIME = Duration.ofSeconds(5);
        DEALER_TIME = Duration.ofSeconds(15);
        WORKER_TIME = Duration.ofSeconds(15);
    }
    public Duration BODY_TIME() {return BODY_TIME; }
    public void setBODY_TIME(Duration BODY_DEALER_TIME){
        this.BODY_TIME = BODY_DEALER_TIME;
    }
    public Duration ENGINE_TIME() {return ENGINE_TIME; }
    public void setENGINE_TIME(Duration ENGINE_DEALER_TIME){
        this.ENGINE_TIME = ENGINE_DEALER_TIME;
    }
    public Duration ACCESSORIES_TIME(){return ACCESSORIES_TIME; }
    public void setACCESSORIES_TIME(Duration ACCESSORIES_DEALER_TIME){
        this.ACCESSORIES_TIME = ACCESSORIES_DEALER_TIME;
    }
    public Duration DEALER_TIME() {return DEALER_TIME; }
    public void setDEALER_TIME(Duration CAR_DEALER_TIME){
        this.DEALER_TIME = CAR_DEALER_TIME;
    }
    public Duration WORKER_TIME() {return WORKER_TIME; }
    public void setWORKER_TIME(Duration WORKER_TIME){
        this.WORKER_TIME = WORKER_TIME;
    }
}

