import java.math.BigInteger;
import java.util.*;
import java.io.*;
 
public class Main{
	public static void main(String[] args){
		Scanner cin = new Scanner(System.in);
		while(cin.hasNext()){
			BigInteger x = cin.nextBigInteger();
			BigInteger y = cin.nextBigInteger();
			System.out.println(x.remainder(y));
		}
    }
}
