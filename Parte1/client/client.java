import java.io.*;
import gnu.getopt.Getopt;
import java.net.* ;
import java.util.* ;
import java.lang.* ;



/********************* CLASE USUARIO ********************/


class usuario{
	String username;
	String ip;
	int port;

	public usuario (String user, String dirIP, int puerto){
		username = user;
		ip = dirIP;
		port = puerto;
	}
	public String getUserName(){
		return this.username;
	}
	public String getIP(){
		return this.ip;
	}
	public int getPuerto(){
		return this.port;
	}
}


/********************* CLIENTE ********************/

class client {

	/********************* AUXILIAR ********************/

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



	/******************* ATTRIBUTES *******************/

	private static String _server   = null;
	private static int _port = -1;
	private static int res;
	private static DataInputStream entrada;
	private static DataOutputStream salida;
	private static Socket sc;
	private static Hilo pthread;
	private static ServerSocket s;
	private static ArrayList<usuario> _usrConectados;
	private static boolean seguir = true;


	/********************* METHODS ********************/

	/**
	 * @param user - User name to register in the system
	 *
	 * @return ERROR CODE
	 */
	static int register(String user)
	{
		try{
		Socket sc = new Socket(_server, _port);  //descriptor del socket

		 entrada = new DataInputStream(sc.getInputStream());
		 salida = new DataOutputStream(sc.getOutputStream());
		 escribir(salida, "REGISTER");
		 escribir(salida, user);
		res = entrada.readByte();

			if(res==0){
				System.out.println("c > REGISTER OK");
			}else if (res==1) {
				System.out.println("c > USERNAME IN USE");
			}else if (res==2) {
				System.out.println("c > REGISTER FAIL");
			}

			sc.close();
		}catch (Exception e){
			System.err.println("excepcion " + e.toString() );
			e.printStackTrace() ;
		}


		return 0;
	}

	/**
	 * @param user - User name to unregister from the system
	 *
	 * @return ERROR CODE
	 */
	static int unregister(String user)
	{
		try{
		Socket sc = new Socket(_server, _port);  //descriptor del socket
		 	entrada = new DataInputStream(sc.getInputStream());
		 	salida = new DataOutputStream(sc.getOutputStream());
			System.out.println("UNREGISTER " + user);
			escribir(salida, "UNREGISTER");
			escribir(salida, user);
			res = entrada.readByte();

			if(res==0){
				System.out.println("c > UNREGISTER OK");
			}else if (res==1) {
				System.out.println("c > USER DOES NOT EXIST");
			}else if (res==2) {
				System.out.println("c > UNREGISTER FAIL");
			}
			sc.close();

		}catch (Exception e){
			System.err.println("excepcion " + e.toString() );
			e.printStackTrace() ;
		}
		return 0;
	}

    	/**
	 * @param user - User name to connect to the system
	 *
	 * @return ERROR CODE
	 */
	static int connect(String user)
	{
		while(seguir==true){
			try{
			Socket sc = new Socket(_server, _port);  //descriptor del socket

			entrada = new DataInputStream(sc.getInputStream());
			salida = new DataOutputStream(sc.getOutputStream());

			//creamos el servidor que atiende peticiones de otros clientes
			s = new ServerSocket(0); //obtiene el puerto al que conectarse, uno cualquiera
			int port = s.getLocalPort();
			String p= Integer.toString(port);

			//Lanzamos el hilo
			pthread = new Hilo(s);
			pthread.start();

			System.out.println("CONNECT " + user);
			escribir(salida, "CONNECT");
			escribir(salida, user);
			escribir(salida, p);

			res = entrada.readByte();
			seguir=false;

			if(res == 0){
				System.out.println("c> CONNECT OK");
			}else if(res == 1){
				System.out.println("c> CONNECT FAIL, USER DOES NOT EXIST");
			}else if(res == 2){
				System.out.println("c> USER ALREADY CONNECTED");
			}else if(res == 3){
				System.out.println("c> CONNECT FAIL");
			}

			sc.close();
		}catch (Exception e){
			System.err.println("excepcion " + e.toString() );
			e.printStackTrace() ;
		}
	}//fin while
		return 0;
	}

	 /**
	 * @param user - User name to disconnect from the system
	 *
	 * @return ERROR CODE
	 */
	static int disconnect(String user) //4
	{
		// Write your code here
		System.out.println("DISCONNECT " + user);

		try{
		seguir = true;
		Socket sc = new Socket(_server, _port);  //descriptor del socket

		entrada = new DataInputStream(sc.getInputStream());
		salida = new DataOutputStream(sc.getOutputStream());

		pthread.terminar();

		escribir(salida, "DISCONNECT");
		escribir(salida, user);

		res = entrada.readByte();
			if(res==0){
				System.out.println("c > DISCONNECT OK");
			}else if(res==1) {
				System.out.println("c > DISCONNECT FAIL, USER DOES NOT EXIST");
			}else if(res==2) {
				System.out.println("c > DISCONNECT FAIL, USER NOT CONNECTED");
			}else if(res==3){
				System.out.println("c > PUBLISH FAIL");
			}

			sc.close();

		}catch (Exception e){
			System.err.println("excepcion " + e.toString() );
			e.printStackTrace() ;
		}
		return 0;
	}

	 /**
	 * @param user
	 * @param file_name    - file name
	 * @param description - descrition
	 *
	 * @return ERROR CODE
	 */
	static int publish(String user, String file_name, String description) //5
	{

		try{

		Socket sc = new Socket(_server, _port);  //descriptor del socket

		 entrada = new DataInputStream(sc.getInputStream());
		 salida = new DataOutputStream(sc.getOutputStream());

		System.out.println("PUBLISH " + file_name + " " + description);

			escribir(salida, "PUBLISH");
			escribir(salida, user);
			escribir(salida, file_name);
			escribir(salida, description);

			res = entrada.readByte();

			if(res==0){
				System.out.println("c > PUBLISH OK");
			}else if (res==1) {
				System.out.println("c > PUBLISH FAIL, USER DOES NOT EXIST");
			}else if (res==2) {
				System.out.println("c > PUBLISH FAIL, USER NOT CONNECTED");
			}else if (res==3){
				System.out.println("c > PUBLISH FAIL, CONTENT ALREADY PUBLISHED");
			}else if (res==4){
				System.out.println("c > PUBLISH FAIL");
			}
			sc.close();
		}catch (Exception e){
			System.err.println("excepcion " + e.toString() );
			e.printStackTrace() ;
		}
		return 0;
	}

	 /**
	 * @param file_name    - file name
	 *
	 * @return ERROR CODE
	 */
	static int delete(String user, String file_name)
	{

		try{
		// Write your code here
		Socket sc = new Socket(_server, _port);  //descriptor del socket

		 entrada = new DataInputStream(sc.getInputStream());
		 salida = new DataOutputStream(sc.getOutputStream());
			System.out.println("DELETE " + user + file_name);
			escribir(salida, "DELETE");
			escribir(salida, user);
			escribir(salida, file_name);

			res = entrada.readByte();
			if(res==0){
				System.out.println("c > DELETE OK");
			}else if (res==1) {
				System.out.println("c > DELETE FAIL, USER DOES NOT EXIST");
			}else if (res==2) {
				System.out.println("c > DELETE FAIL, USER NOT CONNECTED");
			}else if (res==3){
				System.out.println("c > DELETE FAIL, CONTENT NOT PUBLISHED");
			}else if (res==4){
				System.out.println("c > DELETE FAIL");
			}
			sc.close();

		}catch (Exception e){
			System.err.println("excepcion " + e.toString() );
			e.printStackTrace() ;
		}
		return 0;
	}

	 /**
	 * @return ERROR CODE
	 */
	static int list_users(String user)
	{
		try{
			Socket sc = new Socket(_server, _port);  //descriptor del socket
			salida = new DataOutputStream(sc.getOutputStream());
			entrada = new DataInputStream(sc.getInputStream());

			System.out.println("LIST_USERS ");
			escribir(salida, "LIST_USERS");
			escribir(salida, user);
			int i=0;
		String cod_err= leer(entrada);
		int res = Integer.parseInt(cod_err);
		if(res==0){

			String num_usuarios= leer(entrada);
			System.out.println("c > LIST_USERS OK  " + num_usuarios);

			_usrConectados = new ArrayList<usuario>(); //Instancio el ArrayList
			while(i< Integer.parseInt(num_usuarios) ){
				String usr= leer(entrada);
				String IP= leer(entrada);
				String puerto= leer(entrada);
				System.out.println(""+usr + " "+ IP +" " + puerto);
				 _usrConectados.add(new usuario(usr, IP, Integer.parseInt(puerto)));
				 i++;
			 }//FIN WHILE

		 }else if(res==1){
				System.out.println("c > LIST_USERS FAIL , USER DOES NOT EXIST ");

			}else if(res==2){
				System.out.println("c > LIST_USERS FAIL , USER NOT CONNECTED");

			}else if(res==3){
				System.out.println("c > LIST_USERS FAIL");
			}

			sc.close();

		}catch (Exception e){
			System.err.println("excepcion " + e.toString() );
			e.printStackTrace();
		}
			return 0;
	}


	 /**
	 * @param user_name    - user name
	 *
	 * @return ERROR CODE
	 */
	static int list_content(String user, String user_name)
	{
		System.out.println("LIST_CONTENT " + user_name);

		try{
			Socket sc = new Socket(_server, _port);
			salida = new DataOutputStream(sc.getOutputStream());
			entrada = new DataInputStream(sc.getInputStream());

			escribir(salida, "LIST_CONTENT");
			escribir(salida, user);
			escribir(salida, user_name);

			String cod_err= leer(entrada);
			int res = Integer.parseInt(cod_err);

			if(res==0){
				String num_ficheros= leer(entrada);
				System.out.println("c > LIST_USERS OK  " + num_ficheros);
				int i=0;
				while(i< Integer.parseInt(num_ficheros)){
					String usr= leer(entrada);
					System.out.println("" + usr);
					i++;
				 }//FIN WHILE

			 }else if(res==1){
				 System.out.println("c > LIST_CONTENT FAIL , USER DOES NOT EXIST ");

			 }else if(res==2){
				 System.out.println("c > LIST_CONTENT FAIL , USER NOT CONNECTED");

			 }else if(res==3){
				 System.out.println("c > LIST_CONTENT FAIL, REMOTE USER DOES NOT EXIST");
			 }else if (res==4){
				 System.out.println("c > LIST_CONTENT FAIL");
			 }

		 sc.close();


			}catch (Exception e){
				System.err.println("excepcion " + e.toString() );
				e.printStackTrace();
			}
		return 0;
	}

	 /**
	 * @param user_name    - user name
	 * @param remote_file_name    - remote file name
	 * @param local_file_name  - local file name
	 *
	 * @return ERROR CODE
	 */
	static int get_file(String user_name, String remote_file_name, String local_file_name)
	{
		System.out.println("GET_FILE " + user_name + " "  + remote_file_name + " " + local_file_name);
		try{

		System.out.println("Usuarios conectados;" + _usrConectados.size());
		for(int j=0; j< _usrConectados.size(); j++){
			if (_usrConectados.get(j).getUserName().equals(user_name)){
		 		 sc = new Socket(_usrConectados.get(j).getIP(), _usrConectados.get(j).getPuerto());  //descriptor del socket
				 entrada = new DataInputStream(sc.getInputStream());
				 salida = new DataOutputStream(sc.getOutputStream());
				 escribir(salida, "GET_FILE");
				 escribir(salida, remote_file_name);
				 escribir(salida, user_name);
			}
		}//FIN DE FOR

		String respuesta= leer(entrada);
		int res = Integer.parseInt(respuesta); //recibo el 0
		System.out.println("Recibo el 0: " + respuesta);

		//copiar lo que devuelve el cliente
		if(res == 0){
			FileWriter fichero = new FileWriter(local_file_name);
			String result= leer(entrada);

			System.out.println("Recibo el contenido del fichero: " + result);
			fichero.write(result);
			fichero.close();
			System.out.println("c > GET_FILE OK");
		}else if (res==1){
			System.out.println("c > GET_FILE FAIL / FILE NOT EXIST");
		}else if (res==2){
			System.out.println("c > GET_FILE FAIL");
		}

		sc.close();

	}catch (Exception e){
		System.err.println("excepcion " + e.toString());
		e.printStackTrace();
	}
		return 0;
	}

	/**
	 * @brief Command interpreter for the client. It calls the protocol functions.
	 */
	static void shell()
	{
		boolean exit = false;
		String input;
		String [] line;
		BufferedReader in = new BufferedReader(new InputStreamReader(System.in));

		while (!exit) {
			try {
				System.out.print("c> ");
				input = in.readLine();
				line = input.split("\\s");
				System.out.println("Leyendo linea");

				if (line.length > 0) {
					/*********** REGISTER *************/
					if (line[0].equals("REGISTER")) {
						if  (line.length == 2) {
							register(line[1]); // userName = line[1]
						} else {
							System.out.println("Syntax error. Usage: REGISTER <userName>");
						}
					}

					/********** UNREGISTER ************/
					else if (line[0].equals("UNREGISTER")) {
						if  (line.length == 2) {
							unregister(line[1]); // userName = line[1]
						} else {
							System.out.println("Syntax error. Usage: UNREGISTER <userName>");
						}
                    			}

                    			/************ CONNECT *************/
                    			else if (line[0].equals("CONNECT")) {
						if  (line.length == 2) {
							connect(line[1]); // userName = line[1]
						} else {
							System.out.println("Syntax error. Usage: CONNECT <userName>");
                    				}
                    			}

                    			/********** DISCONNECT ************/
                    			else if (line[0].equals("DISCONNECT")) {
						if  (line.length == 2) {
							disconnect(line[1]); // userName = line[1]
						} else {
							System.out.println("Syntax error. Usage: DISCONNECT <userName>");
                    				}
                    			}

                    			/************** PUBLISH **************/
                    			else if (line[0].equals("PUBLISH")) {
						if  (line.length >= 3) {
							// Remove first two words
							String description = input.substring(input.indexOf(' ')+1).substring(input.indexOf(' ')+1);
						//	String description = input.substring(input.indexOf(' ')+1);
							description = description.substring(description.indexOf(' ')+1);
							publish(line[1],line[2],description); // file_name = line[2]
						} else {
							System.out.println("Syntax error. Usage: PUBLISH <userName> <file_name> <description>");
                    				}
                    			}

                    			/************ DELETE *************/
                    			else if (line[0].equals("DELETE")) {
						if  (line.length == 3) {
							delete(line[1], line[2]); // userName = line[1]
						} else {
							System.out.println("Syntax error. Usage: DELETE <userName> <file name>");
                    				}
                    			}

                    			/************** LIST_USERS **************/
                    			else if (line[0].equals("LIST_USERS")) {
						if  (line.length == 2) {
							// Remove first two words
							list_users(line[1]);
						} else {
							System.out.println("Syntax error. Usage: LIST_USERS <userName>");
                    				}
                    			}

                    			/************ LIST_CONTENT *************/
													else if (line[0].equals("LIST_CONTENT")) {
																			if  (line.length == 3) {
																				list_content(line[1], line[2]);
																			} else {
																				System.out.println("Syntax error. Usage: LIST_CONTENT <your_name> <user_name>");
													            }
												}

                    			/************** GET_FILE **************/
                    			else if (line[0].equals("GET_FILE")) {
						if  (line.length == 4) {
							get_file(line[1], line[2], line[3]);
						} else {
							System.out.println("Syntax error. Usage: GET_FILE <user> <remote_file_name> <local_file_name>");
                    				}
                    			}


                    			/************** QUIT **************/
                    			else if (line[0].equals("QUIT")){
						if (line.length == 1) {
							exit = true;
						} else {
							System.out.println("Syntax error. Use: QUIT");
						}
					}

					/************* UNKNOWN ************/
					else {
						System.out.println("Error: command '" + line[0] + "' not valid.");
					}
				}
			} catch (java.io.IOException e) {
				System.out.println("Exception: " + e);
				e.printStackTrace();
			}
		}//fin while
	}//fin shell

	/**
	 * @brief Prints program usage
	 */
	static void usage()
	{
		System.out.println("Usage: java -cp . client -s <server> -p <port>");
	}

	/**
	 * @brief Parses program execution arguments
	 */
	static boolean parseArguments(String [] argv)
	{
		Getopt g = new Getopt("client", argv, "ds:p:");

		int c;
		String arg;

		while ((c = g.getopt()) != -1) {
			switch(c) {
				//case 'd':
				//	_debug = true;
				//	break;
				case 's':
					_server = g.getOptarg();
					break;
				case 'p':
					arg = g.getOptarg();
					_port = Integer.parseInt(arg);
					break;
				case '?':
					System.out.print("getopt() returned " + c + "\n");
					break; // getopt() already printed an error
				default:
					System.out.print("getopt() returned " + c + "\n");
			}
		}

		if (_server == null)
			return false;

		if ((_port < 1024) || (_port > 65535)) {
			System.out.println("Error: Port must be in the range 1024 <= port <= 65535");
			return false;
		}

		return true;
	}



	/********************* MAIN **********************/

	public static void main(String[] argv)
	{
		if(!parseArguments(argv)) {
			usage();
			return;
		}

		shell();
		System.out.println("+++ FINISHED +++");
	}
}
