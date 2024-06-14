package commands;

import services.Storage;

import java.util.List;

public class Mul implements iCommand{
    @Override
    public void execute(List<String> arguments, Storage storage) throws CommandException {
        storage.pushStack (storage.popStack() * storage.popStack());
    }
}