package CommandsTest;

import commands.CommandException;
import commands.Define;
import org.junit.Test;
import services.InputLine;
import services.Storage;

import static org.junit.jupiter.api.Assertions.assertEquals;

public class DefineTest {
    @Test
    public void addTestOk() throws CommandException {
        Define def = new Define();
        InputLine inputLine = new InputLine();
        Storage storage = new Storage();

        inputLine.arguments.add("count");
        inputLine.arguments.add("4");
        def.execute(inputLine.arguments, storage);
        assertEquals (storage.getDefinedValue("count"), 4);

        InputLine inputLine2 = new InputLine();
        inputLine2.arguments.add("abc");
        inputLine2.arguments.add("12");
        def.execute(inputLine2.arguments, storage);
        assertEquals (storage.getDefinedValue("count"), 4);
        assertEquals (storage.getDefinedValue("abc"), 12);

    }
}


