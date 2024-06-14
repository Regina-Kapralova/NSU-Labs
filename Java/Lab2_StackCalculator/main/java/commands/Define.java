package commands;

import services.Storage;

import java.util.List;

public class Define implements iCommand{
    @Override
    public void execute(List<String> arguments, Storage storage) throws CommandException {
        String arg0 = arguments.get(0); // слово
        String arg1 = arguments.get(1); // число

        // проверка arg0 на число
        try {
            Double.parseDouble(arg1);    // проверка на число
        } catch (NumberFormatException e1) {
            throw new CommandException("Argument \"" + arg1 + "\" has to be a number");
        }
        // проверка arg1 на слово
        try {
            Double.parseDouble(arg0);    // проверка на число
            throw new CommandException("Argument \"" + arg0 + "\" has to be a word");
        } catch (NumberFormatException e1) {
            // если мы тут, значит это слово => все в порядке
        }
        if (storage.isDefined(arg0))
            System.out.println("Redefined " + arg0);
        storage.putDefinedValue(arg0, Double.parseDouble(arguments.get(1)));
    }
}
