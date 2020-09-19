import java.io.*;
import gnu.getopt.Getopt;
import java.net.* ;
import java.util.* ;
import java.lang.* ;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.stream.Stream ;

class Hilo extends Thread{
  private ServerSocket serverAddr = null;
  public SocketAddress IP = null; //creo queno es necesario, con el puerto llega pero como pone IP y puerto seleccionado pues...
  private Socket sc = null; //socket del cliente
  private static DataInputStream entrada;
  private static DataOutputStream salida;
  private boolean continuar = true;

   public Hilo (ServerSocket server){
     this.serverAddr= server;
     this.IP = serverAddr.getLocalSocketAddress(); //NO SE BIEN QUE HACER CON LA IP
     System.out.println("IP: " + (this.IP).toString()); //IMPRIME IP/PUERTO, IP: 0.0.0.0/0.0.0.0:42209


   }

   public void run(){ //este seria el metodo que de descargar
     //espera a tender clientes

     System.out.println("ESTOY EN EL HILO DEL SERVIDOR" );
     try{
       while(continuar){
       sc= serverAddr.accept(); //espero a que alguien haga GET_FILE y se conecte conmigo
       System.out.println("espero a recibir clientes" );
       if(sc == null){
         salida.write('2');
         salida.write('\0');
       }

       salida = new DataOutputStream(sc.getOutputStream());
       entrada = new DataInputStream(sc.getInputStream());

       byte[] c = new byte[1];
       String operacion= new String();
       do{
         c[0] = entrada.readByte();
         if (c[0] != '\0'){
           String d = new String(c);
           operacion = operacion + d;
         }
       }while(c[0] != '\0');
       System.out.println("Operacion " + operacion ); //leo operacion

       byte[] b = new byte[1];
       String fichero= new String();
       do{
         b[0] = entrada.readByte();
         if (b[0] != '\0'){
           String d = new String(b);
           fichero = fichero + d;
         }
       }while(b[0] != '\0');

       System.out.println("Fichero " + fichero); //leo fichero remoto

       byte[] a = new byte[1];
       String user_name= new String();
       do{
         a[0] = entrada.readByte(); //leo el cod err
         if (a[0] != '\0'){
           String d = new String(a);
           user_name = user_name + d;
         }
       }while(a[0] != '\0');
       System.out.println("Nombre Usuario " + user_name); //leo fichero remoto


       String ARCHIVO;
       ARCHIVO = "./Almacenamiento/usuarios/";
       ARCHIVO = ARCHIVO.concat(user_name);
       ARCHIVO = ARCHIVO.concat("/");
       ARCHIVO = ARCHIVO.concat(fichero);

      File file = new File(ARCHIVO);
      System.out.println(new File(".").getCanonicalPath());
      System.out.println(ARCHIVO);

      if(file.exists()){
        //envia un 0 al cliente y este al recibir el 0 crea el fichero donde quiere escribir
        salida.write('0');
        salida.write('\0');
        String result = null;
        DataInputStream inputfile = new DataInputStream(new FileInputStream(ARCHIVO));
        int nBytesToRead = inputfile.available();
        if(nBytesToRead >0){
          byte [] buffer = new byte[nBytesToRead];
          inputfile.read(buffer);
          result = new String (buffer);
          System.out.println("Linea " + result);
          salida.writeBytes(result);
          salida.write('\0');
        }
      }else{
        salida.write('1');
        salida.write('\0');
      }
    }
      sc.close();
      }catch (Exception e){
       System.err.println("excepcion " + e.toString() );
       e.printStackTrace() ;
     }
   }
public void terminar(){
  continuar=false;
}

}
