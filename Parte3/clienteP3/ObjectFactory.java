
package clienteP3;

import javax.xml.bind.JAXBElement;
import javax.xml.bind.annotation.XmlElementDecl;
import javax.xml.bind.annotation.XmlRegistry;
import javax.xml.namespace.QName;


/**
 * This object contains factory methods for each 
 * Java content interface and Java element interface 
 * generated in the clienteP3 package. 
 * <p>An ObjectFactory allows you to programatically 
 * construct new instances of the Java representation 
 * for XML content. The Java representation of XML 
 * content can consist of schema derived interfaces 
 * and classes representing the binding of schema 
 * type definitions, element declarations and model 
 * groups.  Factory methods for each of these are 
 * provided in this class.
 * 
 */
@XmlRegistry
public class ObjectFactory {

    private final static QName _ToUpper_QNAME = new QName("http://upperCase/", "ToUpper");
    private final static QName _Upper_QNAME = new QName("http://upperCase/", "Upper");
    private final static QName _UpperResponse_QNAME = new QName("http://upperCase/", "UpperResponse");
    private final static QName _ToUpperResponse_QNAME = new QName("http://upperCase/", "ToUpperResponse");

    /**
     * Create a new ObjectFactory that can be used to create new instances of schema derived classes for package: clienteP3
     * 
     */
    public ObjectFactory() {
    }

    /**
     * Create an instance of {@link ToUpper_Type }
     * 
     */
    public ToUpper_Type createToUpper_Type() {
        return new ToUpper_Type();
    }

    /**
     * Create an instance of {@link Upper }
     * 
     */
    public Upper createUpper() {
        return new Upper();
    }

    /**
     * Create an instance of {@link ToUpperResponse }
     * 
     */
    public ToUpperResponse createToUpperResponse() {
        return new ToUpperResponse();
    }

    /**
     * Create an instance of {@link UpperResponse }
     * 
     */
    public UpperResponse createUpperResponse() {
        return new UpperResponse();
    }

    /**
     * Create an instance of {@link JAXBElement }{@code <}{@link ToUpper_Type }{@code >}}
     * 
     */
    @XmlElementDecl(namespace = "http://upperCase/", name = "ToUpper")
    public JAXBElement<ToUpper_Type> createToUpper(ToUpper_Type value) {
        return new JAXBElement<ToUpper_Type>(_ToUpper_QNAME, ToUpper_Type.class, null, value);
    }

    /**
     * Create an instance of {@link JAXBElement }{@code <}{@link Upper }{@code >}}
     * 
     */
    @XmlElementDecl(namespace = "http://upperCase/", name = "Upper")
    public JAXBElement<Upper> createUpper(Upper value) {
        return new JAXBElement<Upper>(_Upper_QNAME, Upper.class, null, value);
    }

    /**
     * Create an instance of {@link JAXBElement }{@code <}{@link UpperResponse }{@code >}}
     * 
     */
    @XmlElementDecl(namespace = "http://upperCase/", name = "UpperResponse")
    public JAXBElement<UpperResponse> createUpperResponse(UpperResponse value) {
        return new JAXBElement<UpperResponse>(_UpperResponse_QNAME, UpperResponse.class, null, value);
    }

    /**
     * Create an instance of {@link JAXBElement }{@code <}{@link ToUpperResponse }{@code >}}
     * 
     */
    @XmlElementDecl(namespace = "http://upperCase/", name = "ToUpperResponse")
    public JAXBElement<ToUpperResponse> createToUpperResponse(ToUpperResponse value) {
        return new JAXBElement<ToUpperResponse>(_ToUpperResponse_QNAME, ToUpperResponse.class, null, value);
    }

}
