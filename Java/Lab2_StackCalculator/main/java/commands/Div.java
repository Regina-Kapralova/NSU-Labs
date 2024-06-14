package commands;

import services.Storage;

import java.util.List;

public class Div implements iCommand{
    @Override
    public void execute(List<String> arguments, Storage storage) throws CommandException {
        double temp = storage.popStack();
        if (storage.peekStack() == 0){
            storage.pushStack(temp);
            throw new CommandException("Can't be divided by zero.");
        }
        storage.pushStack(temp);
        storage.pushStack (storage.popStack() / storage.popStack());
    }
}
