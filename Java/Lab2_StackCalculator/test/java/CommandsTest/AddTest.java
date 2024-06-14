package CommandsTest;

import commands.CommandException;
import commands.Add;
import org.junit.Test;
import services.InputLine;
import services.Storage;

import java.util.EmptyStackException;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;

public class AddTest {
    @Test
    public void addTestOk() throws CommandException {
        Add add = new Add();
        InputLine inputLine = new InputLine();
        Storage storage = new Storage();

        storage.pushStack(5);
        storage.pushStack(-15);
        add.execute(null, storage);
        assertEquals (storage.peekStack(), -10);

        storage.pushStack(11);
        storage.pushStack(7);
        add.execute(null, storage);
        assertEquals (storage.peekStack(), 18);

        storage.pushStack(32);
        storage.pushStack(-32);
        add.execute(null, storage);
        assertEquals (storage.peekStack(), 0);
    }

    @Test
    public void subTestNotGood(){
        Add add = new Add();
        InputLine inputLine = new InputLine();
        Storage storage = new Storage();
        assertThrows(EmptyStackException.class, () -> add.execute(null, storage));

    }
}
