package net.blumia;

/**
 * Created by blumia on 16-5-21.
 */
public class ModuleDuck extends Duck {
    public ModuleDuck() {
        flyBehavior = new FlyNoWay();
        quackBehavior = new Quack();
    }

    public void display() {
        System.out.println("I'm a model duck");
    }
}
