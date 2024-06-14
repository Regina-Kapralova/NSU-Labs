package services;

import java.io.BufferedReader;
import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.Scanner;

public class Reader {
    private ReadType readType;
    private BufferedReader br;
    public Reader(String[] args){
        if (args.length == 0)
            readType = ReadType.CMD;
        else {
            Path path = Paths.get(args[0]);
            if (!Files.exists(path) || !Files.isReadable(path)){
                System.out.println("File in path \"" + path + "\" doesn't exist or non-readable");
                System.out.println("Ignore this path and waiting input from cmd:");
                readType = ReadType.CMD;
            }
            else {
                try {
                    this.br = Files.newBufferedReader(path, StandardCharsets.UTF_8);
                    readType = ReadType.FILE;
                }
                catch (IOException exception){
                    System.err.println("Can't open file: " + exception.getMessage());
                }
            }
        }
    }
    public String readLine(){
        if (readType == ReadType.FILE){
            try{
                String str;
                if ((str = br.readLine()) != null)
                    return str;
                else
                    return null;
            } catch (IOException exception){
                System.err.println("Can't read file: " + exception.getMessage());
                return null;
            }
        }
        else {
            Scanner in = new Scanner(System.in);
            return in.nextLine();
        }
    }
    private enum ReadType {CMD, FILE};
}
