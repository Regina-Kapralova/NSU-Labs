import commands.Factory;
import services.Parser;
import services.InputLine;
import services.*;
import services.Storage;

import java.io.BufferedWriter;
import java.util.Objects;

public final class Main {
    public static void main(String[] args) {
        Reader reader = new Reader(args);
        String line;
        Storage storage = new Storage();
        while ((line = reader.readLine()) != null){
            try{
                InputLine inputLine;
                if ((inputLine = Parser.parse(line)) == null) continue;
                Validator.isValid(inputLine, storage);
                Objects.requireNonNull(Factory.commandClass(inputLine.fullCommand)).execute(inputLine.arguments, storage);
            }
            catch (Exception ex){
                System.out.println(ex.getMessage());
                if (ex.getMessage().equals("Stack is empty. Work finalized."))
                    break;
            }
        }
    }
}