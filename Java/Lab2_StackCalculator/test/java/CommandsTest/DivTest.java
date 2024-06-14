package CommandsTest;

import commands.CommandException;
import commands.Div;
import org.junit.Test;
import services.InputLine;
import services.Storage;

import java.util.EmptyStackException;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;

public class DivTest {
    @Test
    public void addTestOk() throws CommandException {
        Div div = new Div();
        InputLine inputLine = new InputLine();
        Storage storage = new Storage();

        storage.pushStack(5);
        storage.pushStack(-15);
        div.execute(null, storage);
        assertEquals (storage.peekStack(), -3);

        storage.pushStack(14);
        storage.pushStack(7);
        div.execute(null, storage);
        assertEquals (storage.peekStack(), 0.5);

        storage.pushStack(32);
        storage.pushStack(0);
        div.execute(null, storage);
        assertEquals (storage.peekStack(), 0);
    }

    @Test
    public void subTestNotGood(){
        Div div = new Div();
        InputLine inputLine = new InputLine();
        Storage storage = new Storage();
        assertThrows(EmptyStackException.class, () -> div.execute(null, storage));
        storage.pushStack(0);
        assertThrows(EmptyStackException.class, () -> div.execute(null, storage));
        storage.pushStack(26);
        assertThrows(EmptyStackException.class, () -> div.execute(null, storage));
    }
}

