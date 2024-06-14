package CommandsTest;

import commands.CommandException;
import commands.Sqrt;
import org.junit.Test;
import services.InputLine;
import services.Storage;

import java.util.EmptyStackException;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;

public class SqrtTest {
    @Test
    public void sqrtTestOk() throws CommandException {
        Sqrt sqrt = new Sqrt();
        InputLine inputLine = new InputLine();
        Storage storage = new Storage();

        storage.pushStack(25);
        sqrt.execute(null, storage);
        assertEquals (storage.peekStack(), 5);

        storage.pushStack(2.25);
        sqrt.execute(null, storage);
        assertEquals (storage.peekStack(), 1.5);
    }

    @Test
    public void sqrtTestNotGood(){
        Sqrt sqrt = new Sqrt();
        InputLine inputLine = new InputLine();
        Storage storage = new Storage();
        assertThrows(EmptyStackException.class, () -> sqrt.execute(null, storage));

    }
}

