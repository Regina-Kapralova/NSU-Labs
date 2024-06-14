package commands;

public class Factory {
    public static iCommand commandClass(String fullCommand) {
        try {
            return (iCommand)Class.forName(fullCommand).getDeclaredConstructor().newInstance();
        } catch (Exception ex){
            System.out.println("Class for this command doesn't exist");
        }
        return null;
    };
}
