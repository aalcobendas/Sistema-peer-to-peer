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
  public SocketAddress IP = null;
  private Socket sc = null; //socket del cliente
  private static DataInputStream entrada;
  private static DataOutputStream salida;
  private boolean continuar = true;

   public Hilo (ServerSocket server){
     this.serverAddr= server;
     this.IP = serverAddr.getLocalSocketAddress();
     //System.out.println("IP: " + (this.IP).toString());
   }

   public static String leer(DataInputStream input){
       byte[] ch = new byte[1];
       String output = new String();
       try {
           do {
               ch[0] = input.readByte();
               if (ch[0] != '\0'){
                   String d = new String(ch);
                   output = output + d;
               }
           }while(ch[0] != '\0');
         }
         catch (Exception e) {
           System.err.println("excepcion " + e.toString() );
         }
         return output;
  }
   public static void escribir(DataOutputStream output, String mensaje){
         try{
           output.writeBytes(mensaje);
           output.write('\0');
         }
         catch(Exception e){
           System.err.println("excepcion " + e.toString());
         }
   }

   public void run(){ //este seria el metodo que de descargar
     System.out.println("ESTOY EN EL HILO DEL SERVIDOR" );
     try{
       while(continuar){
       sc= serverAddr.accept(); //espero a que alguien haga GET_FILE y se conecte conmigo
      // System.out.println("espero a recibir clientes" );
       if(sc == null){
         salida.write('2');
         salida.write('\0');
       }

       salida = new DataOutputStream(sc.getOutputStream());
       entrada = new DataInputStream(sc.getInputStream());
       String operacion= leer(entrada);
       //System.out.println("Operacion " + operacion ); //leo operacion
       String fichero= leer(entrada);
       //System.out.println("Fichero " + fichero); //leo fichero remoto
       String user_name= leer(entrada);
       //System.out.println("Nombre Usuario " + user_name); //leo fichero remoto

       String ARCHIVO;
       ARCHIVO = "../";
       ARCHIVO = ARCHIVO.concat(user_name);
       ARCHIVO = ARCHIVO.concat("/");
       ARCHIVO = ARCHIVO.concat(fichero); //nombre + extension introducidos por terminal

      File file = new File(ARCHIVO);
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
      sc.close();
    }//FIN WHILE
      }catch (Exception e){
       System.err.println("excepcion " + e.toString() );
       e.printStackTrace() ;
     }
  }

  public void terminar(){
    continuar=false;
  }

}
