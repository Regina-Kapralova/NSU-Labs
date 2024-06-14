package commands;

import services.Storage;

import java.util.List;

public class Print implements iCommand{
    @Override
    public void execute(List<String> arguments, Storage storage) {
        System.out.println(storage.peekStack());
    }
}
