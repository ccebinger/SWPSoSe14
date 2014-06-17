public class Stack {
    private static final int STACK_SIZE = 1000;
    
    private int top = 0;
    private Object[] stack = new Object[STACK_SIZE];
    
    public void push(Object s) {
        stack[top++] = s;
    }
    
    public Object pop() {
        return stack[top--];
    }
}
