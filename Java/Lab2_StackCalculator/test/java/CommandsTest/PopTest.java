package CommandsTest;

import commands.CommandException;
import commands.Pop;
import org.junit.Test;
import services.InputLine;
import services.Storage;

import java.util.EmptyStackException;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;

public class PopTest {
    @Test
    public void PopTestOk() throws CommandException {
        Pop pop = new Pop();
        InputLine inputLine = new InputLine();
        Storage storage = new Storage();

        storage.pushStack(14);
        storage.pushStack(59);
        storage.pushStack(25);
        pop.execute(null, storage);
        assertEquals (storage.peekStack(), 59);
        pop.execute(null, storage);
        assertEquals (storage.peekStack(), 14);
    }
    @Test
    public void popTestNotGood(){
        Pop pop = new Pop();
        InputLine inputLine = new InputLine();
        Storage storage = new Storage();
        assertThrows(EmptyStackException.class, () -> pop.execute(null, storage));

    }
}


