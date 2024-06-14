package commands;

public class CommandException extends Exception{
    String message;
    public CommandException(String str){
        message = str;
    }
    @Override
    public String getMessage() {
        return message;
    }
}