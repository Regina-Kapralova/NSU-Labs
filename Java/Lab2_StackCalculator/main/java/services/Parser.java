package services;

import org.antlr.v4.runtime.misc.ParseCancellationException;
import java.io.FileInputStream;
import java.io.IOException;
import java.util.Arrays;
import java.util.Properties;

public class Parser {
    public static InputLine parse(String string) throws ParseCancellationException {
        if (string.isEmpty())
            return  null;

        String[] words = (string.split(" "));
        InputLine inputLine = new InputLine();

        Properties properties = new Properties();
        try {
            properties.load(new FileInputStream("src/main/java/commands/calc.properties"));
        } catch (IOException e) {
            e.printStackTrace();
        }
        inputLine.command = words[0];
        for (String commandShortName : properties.stringPropertyNames()){
            String[] propertiesLine = (properties.getProperty(commandShortName).split(" "));
            if (words[0].equals(propertiesLine[3]))
                inputLine.command = commandShortName;
        }
        inputLine.arguments.addAll(Arrays.asList(words).subList(1, words.length));

        return inputLine;
    }
}
