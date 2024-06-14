import commands.Factory;
import commands.iCommand;
import org.junit.Test;
import static org.junit.jupiter.api.Assertions.*;

public class FactoryTest {
    @Test
    public void commandClass() {
        iCommand command = Factory.commandClass("commands.Push");
        assertEquals("class commands.Push", command.getClass().toString());
        command = Factory.commandClass("commands.Add");
        assertEquals("class commands.Add", command.getClass().toString());
        command = Factory.commandClass("commands.Div");
        assertEquals("class commands.Div", command.getClass().toString());
    }
    @Test
    public void failed(){
            assertNull(Factory.commandClass("commands.Pow"));
            assertNull(Factory.commandClass("commands.1245"));
    }
}
