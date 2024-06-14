package commands;

import services.Storage;

import java.util.List;

public interface iCommand {
    void execute(List<String> arguments, Storage storage) throws CommandException;
}