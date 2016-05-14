package net.blumia;

/**
 * Created by blumia on 16-5-14.
 */
public class MuteQuack implements QuackBehavior{
    @Override
    public void quack() {
        System.out.println("<< Silence >>");
    }
}
