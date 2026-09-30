import java.net.Socket;
import java.net.UnknownHostException;
import java.io.IOException;


public class Vapor {
    public static void main(String[] args) {
        try {
            Socket socket = new Socket("127.0.0.1", 6000);
        } catch (UnknownHostException ex) {
            System.out.println(ex);
        } catch (IOException ex) {
            System.out.println(ex);
        }
    }
}