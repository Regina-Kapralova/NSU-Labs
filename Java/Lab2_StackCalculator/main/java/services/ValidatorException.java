package services;

public class ValidatorException extends Exception{
    String message;
    public ValidatorException(String str){
        message = str;
    }
    @Override
    public String getMessage() {
        return message;
    }
}
