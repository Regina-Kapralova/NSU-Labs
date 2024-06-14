import org.junit.Test;
import services.Reader;

import static org.junit.jupiter.api.Assertions.assertEquals;

public class ReaderTest {
    @Test
    public void readLine(){
        String[] args = {"src/test/java/testCodeFiles/fileForReaderTest.txt"};
        System.out.println(args[0]);
        Reader reader = new Reader(args);

        assertEquals ("PUSH 5", reader.readLine());
        assertEquals ("PUSH 4", reader.readLine());
        assertEquals ("-", reader.readLine());
        assertEquals ("PRINT", reader.readLine());

    }
}