package CommandsTest;

import commands.CommandException;
import commands.Mul;
import org.junit.Test;
import services.InputLine;
import services.Storage;

import java.util.EmptyStackException;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;

public class MulTest {
    @Test
    public void mulTestOk() throws CommandException {
        Mul mul = new Mul();
        InputLine inputLine = new InputLine();
        Storage storage = new Storage();

        storage.pushStack(5);
        storage.pushStack(15);
        mul.execute(null, storage);
        assertEquals (storage.peekStack(), 75);

        storage.pushStack(-11);
        storage.pushStack(4);
        mul.execute(null, storage);
        assertEquals (storage.peekStack(), -44);

        storage.pushStack(3.2);
        storage.pushStack(0.32);
        mul.execute(null, storage);
        assertEquals (storage.peekStack(), 1.024);
    }

    @Test
    public void mulTestNotGood(){
        Mul mul = new Mul();
        InputLine inputLine = new InputLine();
        Storage storage = new Storage();
        assertThrows(EmptyStackException.class, () -> mul.execute(null, storage));
    }
}

