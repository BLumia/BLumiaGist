import java.io.IOException;
import java.net.*;
import java.util.Scanner;

// Runnable impl for multi-thread.
public class MulticastSocketTest implements Runnable {

	// Multicast IP Scale: 224.0.0.0 ~ 239.255.255.255
	private static final String BROADCAST_IP = "224.61.61.61";
	// pickup a random port
	public static final int BROADCAST_PORT = 30000;
	private static final int DATA_LEN = 4096;

	private MulticastSocket socket = null;
	private InetAddress broadcastAddress = null;
	private Scanner scan = null;

	byte[] inBuff = new byte[DATA_LEN];
	private DatagramPacket inPacket = new DatagramPacket(inBuff , inBuff.length);
	private DatagramPacket outPacket = null;

	public void init() throws IOException {
		try {
			socket = new MulticastSocket(BROADCAST_PORT);
			broadcastAddress = InetAddress.getByName(BROADCAST_IP);
			socket.joinGroup(broadcastAddress);
			socket.setLoopbackMode(false);
			outPacket = new DatagramPacket(new byte[0], 0, broadcastAddress, BROADCAST_PORT);

			// Run new thread to listen and receive UDP Multicast pack.
			new Thread(this).start();

			scan = new Scanner(System.in);
			while(scan.hasNextLine()) {
				byte[] buff = scan.nextLine().getBytes();
				outPacket.setData(buff);
				socket.send(outPacket);
			}
		}
		finally
		{
			socket.close();
		}
	}
	public void run() {
		try {
			while(true) {
				socket.receive(inPacket);
				System.out.println(inPacket.getAddress().toString() + " Says:" + new String(inBuff , 0 ,
						inPacket.getLength()));
			}
		}
		catch (IOException ex) {
			ex.printStackTrace();
			try {
				if (socket != null) {
					socket.leaveGroup(broadcastAddress);
					socket.close();
				}
				System.exit(1);
			}
			catch (IOException e) {
				e.printStackTrace();
			}
		}
	}


	public static void main(String[] args) throws IOException {
		new MulticastSocketTest().init();
	}
}