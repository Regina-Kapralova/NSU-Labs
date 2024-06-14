package services;

import java.util.LinkedList;

public class InputLine{
    public InputLine(){
        arguments = new LinkedList<>();
        command = null;
        fullCommand = null;
    }
    public String command;
    public String fullCommand;
    public LinkedList<String> arguments;
}
