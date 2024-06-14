package commands;

import services.Storage;
import java.util.List;

public class Exit implements iCommand{
    @Override
    public void execute(List<String> arguments, Storage storage) throws CommandException {
        while (storage.SizeStack() > 0)
            storage.popStack();
        throw new CommandException("Stack is empty. Work finalized.");
    }
}
