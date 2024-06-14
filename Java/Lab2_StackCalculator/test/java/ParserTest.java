
import org.junit.Test;
import services.Parser;
import services.InputLine;

import static org.junit.jupiter.api.Assertions.assertEquals;


public class ParserTest {
    @Test
    public void parse(){
        // correct strings
        InputLine inputLine = Parser.parse("PUSH 10");
        assertEquals ("PUSH", inputLine.command);
        assertEquals (inputLine.arguments.size(), 1);
        assertEquals ("10", inputLine.arguments.get(0));

        inputLine = Parser.parse("PRINT");
        assertEquals ("PRINT", inputLine.command);
        assertEquals (inputLine.arguments.size(), 0);

        inputLine = Parser.parse("DEFINE count 4");
        assertEquals ("DEFINE", inputLine.command);
        assertEquals (inputLine.arguments.size(), 2);
        assertEquals ("count", inputLine.arguments.get(0));
        assertEquals ("4", inputLine.arguments.get(1));

        inputLine = Parser.parse("* 25");
        assertEquals ("MUL", inputLine.command);
        assertEquals (inputLine.arguments.size(),  1);
        assertEquals ("25", inputLine.arguments.get(0));

        inputLine = Parser.parse("# abc");
        assertEquals ("COMMENT", inputLine.command);
        assertEquals (inputLine.arguments.size(),  1);
        assertEquals ("abc", inputLine.arguments.get(0));
    }

}
