import org.junit.Test;
import services.InputLine;
import services.*;
import services.Storage;

import java.util.LinkedList;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;

public class ValidatorTest {
    @Test
    public void isValid() throws ValidatorException {

        InputLine inputLine = new InputLine();
        inputLine.arguments = new LinkedList<>();
        Storage storage = new Storage();

        // Bad situations
        // 1) Команды не существует
        String[] badCommands = {"", "bad", "\n", "PU SH", "1", "1\n"};
        for (String badCommand : badCommands) {
            inputLine.command = badCommand;
            assertThrows(ValidatorException.class, () -> Validator.isValid(inputLine, storage));
        }

        // 2) Неверное число аргументов
        inputLine.command = "PUSH";
        try {
            Validator.isValid(inputLine, storage);
        } catch (ValidatorException ex){
            assertEquals ("For command \"PUSH\" expected 1 arguments, detected 0", ex.getMessage());
        }
        inputLine.arguments.add("1");
        inputLine.arguments.add("2");
        try {
            Validator.isValid(inputLine, storage);
        } catch (ValidatorException ex){
            assertEquals ("For command \"PUSH\" expected 1 arguments, detected 2", ex.getMessage());
        }
        inputLine.arguments.clear();
        inputLine.command = "POP";
        inputLine.arguments.add("1");
        try {
            Validator.isValid(inputLine, storage);
        } catch (ValidatorException ex){
            assertEquals ("For command \"POP\" expected 0 arguments, detected 1", ex.getMessage());
        }
        inputLine.arguments.clear();
        // 3) На стеке недостаточно аргументов
        inputLine.command = "POP";
        try {
            Validator.isValid(inputLine, storage);
        } catch (ValidatorException ex){
            assertEquals ("For command \"POP\" expected 1 elements on stack, but stack contains 0", ex.getMessage());
        }
        inputLine.arguments.clear();
        // Все в порядке
        inputLine.command = "PUSH";
        inputLine.arguments.add("4");
        assertEquals (Validator.isValid(inputLine, storage), true);
        inputLine.arguments.clear();
        inputLine.command = "POP";
        storage.pushStack(1);
        assertEquals (Validator.isValid(inputLine, storage), true);
        storage.putDefinedValue("b", 5);
        inputLine.command = "PUSH";
        inputLine.arguments.add("b");
        assertEquals (Validator.isValid(inputLine, storage), true);
    }
}
