package upperCase;
import javax.jws.WebService;
import javax.jws.WebMethod;

@WebService
public class ToUpper {
    public void ToUpper() {}
    @WebMethod
    public String Upper(String name) {
         return name.toUpperCase();
    }
}
