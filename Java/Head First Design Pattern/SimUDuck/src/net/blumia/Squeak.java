package net.blumia;

/**
 * Created by blumia on 16-5-14.
 */
public class Squeak implements QuackBehavior{
    @Override
    public void quack() {
        System.out.println("Squeak");
    }
}
