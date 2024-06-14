package CommandsTest;

import commands.CommandException;
import commands.Sub;
import org.junit.Test;
import services.InputLine;
import services.Storage;

import java.util.EmptyStackException;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;

public class SubTest {
    @Test
    public void subTestOk() throws CommandException {
        Sub sub = new Sub();
        InputLine inputLine = new InputLine();
        Storage storage = new Storage();

        storage.pushStack(5);
        storage.pushStack(15);
        sub.execute(null, storage);
        assertEquals (storage.peekStack(), 10);

        storage.pushStack(11);
        storage.pushStack(7);
        sub.execute(null, storage);
        assertEquals (storage.peekStack(), -4);

        storage.pushStack(32);
        storage.pushStack(32);
        sub.execute(null, storage);
        assertEquals (storage.peekStack(), 0);
    }

    @Test
    public void subTestNotGood(){
        Sub sub = new Sub();
        InputLine inputLine = new InputLine();
        Storage storage = new Storage();
        assertThrows(EmptyStackException.class, () -> sub.execute(null, storage));

    }
}

