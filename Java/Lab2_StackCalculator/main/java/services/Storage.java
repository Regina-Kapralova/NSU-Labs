package services;

import java.util.*;

public final class Storage  {
    public final void pushStack(double number){
        stack.push(number);
    }
    public final double popStack() throws EmptyStackException { return stack.pop(); }
    public final double peekStack() throws EmptyStackException {return stack.peek();}
    public int SizeStack() {
        return stack.size();
    }

    public final void putDefinedValue(String word, double number) {
        definesList.put(word, number);
    }
    public final double getDefinedValue(String word) { return definesList.get(word); }
    public boolean isDefined(String word) {
        return definesList.containsKey(word);
    }

    public Storage(){
        this.stack = new Stack<>();
        this.definesList = new HashMap<String, Double>();
    }
    private final Stack<Double> stack;
    private final HashMap<String, Double> definesList;
}
