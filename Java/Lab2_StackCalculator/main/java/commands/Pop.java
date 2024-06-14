package commands;

import services.Storage;

import java.util.List;

public class Pop implements iCommand{

    @Override
    public void execute(List<String> arguments, Storage storage) {
        storage.popStack();
    }

}
