package services;

import java.io.FileInputStream;
import java.io.IOException;
import java.util.Properties;

public final class Validator {
    public static boolean isValid(InputLine inputLine, Storage storage) throws ValidatorException {
        Properties properties = new Properties();
        String[] parameters;
        try {
            properties.load(new FileInputStream("src/main/java/commands/calc.properties"));
        }catch (IOException ex){
            ex.printStackTrace();
        }
        // 1) Команда существует
        if (!properties.containsKey(inputLine.command)){
            throw new ValidatorException("Command \"" + inputLine.command + "\" doesn't exist");
        }
        parameters = properties.getProperty(inputLine.command).split(" ");
        inputLine.fullCommand = parameters[0];
        // 2) Верное число аргументов
        int expectedCountOfArgs = Integer.parseInt(parameters[1]);
        if (!inputLine.command.equals("COMMENT")){ //если не комментарий
            if (expectedCountOfArgs != inputLine.arguments.size())
                throw new ValidatorException("For command \"" + inputLine.command +
                    "\" expected " + expectedCountOfArgs + " arguments, detected " + inputLine.arguments.size());
        }
        // 3) На стеке достаточно аргументов
        int expectedCountOfStackElements = Integer.parseInt(parameters[2]);
        if (storage.SizeStack() < expectedCountOfStackElements){
            throw new ValidatorException("For command \"" + inputLine.command +
                    "\" expected " + expectedCountOfStackElements +
                    " elements on stack, but stack contains " + storage.SizeStack());
        }
        return true;
    }
}