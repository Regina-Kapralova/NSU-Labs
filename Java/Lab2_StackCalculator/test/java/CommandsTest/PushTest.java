package CommandsTest;

import commands.CommandException;
import commands.Push;
import org.junit.Test;
import services.InputLine;
import services.Storage;

import java.util.EmptyStackException;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;

public class PushTest {
    @Test
    public void pushTestOk() throws CommandException {
        Push push = new Push();
        InputLine inputLine = new InputLine();
        Storage storage = new Storage();

        inputLine.arguments.add("59");
        push.execute(inputLine.arguments, storage);
        assertEquals (storage.peekStack(), 59);

        InputLine inputLine2 = new InputLine();
        inputLine2.arguments.add("18");
        push.execute(inputLine2.arguments, storage);
        assertEquals (storage.peekStack(), 18);
    }
    public void pushTestNotGood(){
        Push push = new Push();
        InputLine inputLine = new InputLine();
        Storage storage = new Storage();
        inputLine.arguments.add("count");
        assertThrows(EmptyStackException.class, () -> push.execute(inputLine.arguments, storage));

    }

}


