package commands;

import services.Storage;

import java.util.List;

public class Sqrt implements iCommand{
    @Override
    public void execute(List<String> arguments, Storage storage) throws CommandException {
        if (storage.peekStack() < 0) throw new CommandException("<0 sqrt");
        storage.pushStack(Math.sqrt(storage.popStack()));
    }


}
