package upperCase;
import javax.xml.ws.Endpoint;

public class ToUpperPublisher {
  public static void main(String[] args) {

     String bindingURI = "http://localhost:8080/upperCase/ToUpper";
     ToUpper webService = new ToUpper();
     Endpoint.publish(bindingURI, webService);
     System.out.println("Server started at: " + bindingURI);
  }
}
