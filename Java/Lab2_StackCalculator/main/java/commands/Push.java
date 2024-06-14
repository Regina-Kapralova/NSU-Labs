package commands;

import services.Storage;

import java.util.List;

public class Push implements iCommand{
    @Override
    public void execute(List<String> arguments, Storage storage) throws CommandException {
        String arg = arguments.get(0);
            try {
                Double.parseDouble(arg);    // проверка на double
                storage.pushStack(Double.parseDouble(arg));
            } catch (NumberFormatException e1) {
                // если мы тут, значит дефайн должен быть в листе
                if (!storage.isDefined(arg)){
                    throw new CommandException("For argument \"" + arg + "\" define doesn't exist");
                }
                else
                    storage.pushStack(storage.getDefinedValue(arg));
            }

    }
}
