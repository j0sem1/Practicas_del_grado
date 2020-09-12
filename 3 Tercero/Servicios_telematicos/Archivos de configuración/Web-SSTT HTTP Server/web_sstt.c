#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <sys/stat.h>
#include <regex.h>
#include <time.h>

#define VERSION		24
#define BUFSIZE		8096
#define ERROR		42
#define LOG			44
// Códigos de error HTTP
#define BADREQUEST	400
#define PROHIBIDO	403
#define NOENCONTRADO	404
#define NOTSUPPORTED	405
#define TOOMANYREQUEST	429
#define	EXTENSION 	440
#define	SERVERERROR	500
#define EXTENSIONS_SIZE 10
#define FIELDS_SIZE 8

struct {
	char *ext;
	char *filetype;
} extensions [] = {
	{"gif", "image/gif" },
	{"jpg", "image/jpg" },
	{"jpeg","image/jpeg"},
	{"png", "image/png" },
	{"ico", "image/ico" },
	{"zip", "image/zip" },
	{"gz",  "image/gz"  },
	{"tar", "image/tar" },
	{"htm", "text/html" },
	{"html","text/html" },
	{0,0} };

char* fields[] = { "Host:", "User-Agent:", "Accept:", "Accept-Language:", "Accept-Encoding:", "Accept-Charset:", "Keep-Alive:", "Connection:" };

void debug(int log_message_type, char *message, char *additional_info, int socket_fd)
{
	int fd ;
	char logbuffer[BUFSIZE*2];
	
	switch (log_message_type) {
		case ERROR: (void)sprintf(logbuffer,"ERROR: %s:%s Errno=%d exiting pid=%d",message, additional_info, errno,getpid());
			break;
		case PROHIBIDO:
			// Enviar como respuesta 403 Forbidden
			(void)sprintf(logbuffer,"FORBIDDEN: %s:%s",message, additional_info);
			break;
		case NOENCONTRADO:
			// Enviar como respuesta 404 Not Found
			(void)sprintf(logbuffer,"NOT FOUND: %s:%s",message, additional_info);
			break;
		case LOG: (void)sprintf(logbuffer," INFO: %s:%s:%d",message, additional_info, socket_fd); break;
	}

	if((fd = open("webserver.log", O_CREAT| O_WRONLY | O_APPEND,0644)) >= 0) {
		(void)write(fd,logbuffer,strlen(logbuffer));
		(void)write(fd,"\n",1);
		(void)close(fd);
	}
	if(log_message_type == ERROR || log_message_type == NOENCONTRADO || log_message_type == PROHIBIDO) exit(3);
}



void write_estado(int descriptorFichero, char* mensaje){
	char estado[BUFSIZE];

	if (sprintf(estado, "HTTP/1.1 %s\r\n", mensaje) < 0){
		fprintf(stderr, "Línea: %d. Error en la función 'sprintf'\n", __LINE__);
		debug(ERROR,"function", "sprintf", 0);
		return;
	}

	if (write(descriptorFichero, estado, strlen(estado)) == -1){
		fprintf(stderr, "Línea: %d. Error writting to socket: %s\n", __LINE__, strerror(errno));
		debug(ERROR,"system call", "write", 0);
		return;
	}
}

void write_error(int descriptorFichero, char* mensaje, char* cookie_rcv, char* actual_date_rcv){
	char cabecera[BUFSIZE], cookie[BUFSIZE], actual_date[BUFSIZE], web[BUFSIZE];

	if (sprintf(web, "<html>\n<head>\n<title>ERROR</title>\n</head>\n<body>\n<h1>%s</h1>\n"
	  "<hr>\n<address>Servidor de Servicios Telem&aacuteticos</address>\n</body>\n</html>\n",
	  mensaje) < 0){
		fprintf(stderr, "Línea: %d. Error en la función 'sprintf'\n", __LINE__);
		debug(ERROR,"function", "sprintf", 0);
		return;
	}

	if (cookie_rcv != NULL){
		if (sprintf(cookie, "%s", cookie_rcv) < 0){
			fprintf(stderr, "Línea: %d. Error en la función 'sprintf'\n", __LINE__);
			debug(ERROR,"function", "sprintf", 0);
			return;
		}
	} else {
		memset(cookie, '\0', BUFSIZE);
	}

	if (actual_date_rcv != NULL){
		if (sprintf(actual_date, "Date: %s\r\n", actual_date_rcv) < 0){
			fprintf(stderr, "Línea: %d. Error en la función 'sprintf'\n", __LINE__);
			debug(ERROR,"function", "sprintf", 0);
			return;
		}
	} else {
		memset(actual_date, '\0', BUFSIZE);
	}

	if (sprintf(cabecera, "%sServer: j0sem1\r\nKeep-Alive: timeout=10, max=100\r\nConnection: Keep-Alive\r\nContent-Type: text/html\r\nContent-Length: %lu\r\n%s\r\n", actual_date, strlen(web),
	  cookie) < 0){
		fprintf(stderr, "Línea: %d. Error en la función 'sprintf'\n", __LINE__);
		debug(ERROR,"function", "sprintf", 0);
		return;
	}

	if (write(descriptorFichero, cabecera, strlen(cabecera)) == -1){
		fprintf(stderr, "Línea: %d. Error writting to socket: %s\n", __LINE__, strerror(errno));
		debug(ERROR,"system call", "write", 0);
		return;
	}

	if (write(descriptorFichero, web, strlen(web)) == -1){
		fprintf(stderr, "Línea: %d. Error writting to socket: %s\n", __LINE__, strerror(errno));
		debug(ERROR,"system call", "write", 0);
		return;
	}
}

void tratarError(int descriptorFichero, int codigo_error, char* cookie, char* actual_date){
	switch(codigo_error) {

		case BADREQUEST:
			write_estado(descriptorFichero, "400 Bad Request");
			write_error(descriptorFichero, "Error 400: Bad Request", NULL, actual_date);
			break;

		case PROHIBIDO:
			write_estado(descriptorFichero, "403 Forbidden");
			write_error(descriptorFichero, "Error 403: Acceso prohibido", NULL, actual_date);
			break;

		case NOENCONTRADO:
			write_estado(descriptorFichero, "404 Not Found");
			write_error(descriptorFichero, "Error 404: Not Found", NULL, actual_date);
			break;

		case NOTSUPPORTED:
			write_estado(descriptorFichero, "405 Method Not Supported");
			write_error(descriptorFichero, "Error 405: Method Not Supported", NULL, actual_date);
			break;
		
		case TOOMANYREQUEST:
			write_estado(descriptorFichero, "429 Too Many Requests");
			write_error(descriptorFichero, "Error 429: Too Many Request", NULL, actual_date);
			break;

		case EXTENSION:
			write_estado(descriptorFichero, "440 Extension Not Supported");
			write_error(descriptorFichero, "Error 440: Extension Not Supported", NULL, actual_date);
			break;

		case SERVERERROR:
			write_estado(descriptorFichero, "500 Internal Server Error");
			write_error(descriptorFichero, "Error 500: Internal Server Error", NULL, actual_date);
			break;
	}
}

void readwrite(int descriptorFicheroSocket, int descriptorFicheroSolicitado, char* cookie, char* actual_date){
	int nleidos;
	char archivo[BUFSIZE];
	nleidos = 1;	// Inicializado a 1 para que no de problemas en la primera ejecución del while

	// Lectura y escritura del archivo
	while(nleidos != 0){
		if ((nleidos = read(descriptorFicheroSolicitado, archivo, BUFSIZE)) == -1){
			fprintf(stderr, "Línea: %d. Error reading from socket: %s\n", __LINE__, strerror(errno));
			tratarError(descriptorFicheroSocket, SERVERERROR, NULL, actual_date);
			debug(ERROR,"system call", "read", 0);
			return;
		}

		if (write(descriptorFicheroSocket, archivo, nleidos) == -1){
			fprintf(stderr, "Línea: %d. Error writting to socket: %s\n", __LINE__, strerror(errno));
			tratarError(descriptorFicheroSocket, SERVERERROR, NULL, actual_date);
			debug(ERROR,"system call", "write", 0);
			return;
		}
	}
}

void process_web_request(int descriptorFichero)
{
	debug(LOG,"request","Ha llegado una peticion",descriptorFichero);


	/**************************
	*** BUFFERS Y VARIABLES ***
	**************************/
	// Declaración de variables
	char peticion[BUFSIZE], buf[BUFSIZE], estado[BUFSIZE], cabecera[BUFSIZE], nombre_archivo[BUFSIZE], extension[BUFSIZE], cookie[BUFSIZE], actual_date[BUFSIZE], date[BUFSIZE], ext[BUFSIZE], nombre_archivo_aux[BUFSIZE], linea_actual[BUFSIZE];
	char *token, *date_aux;
	int nleidos_peticion, status_ER, size, hayCookie, codigo_error, i;
	regex_t regex;
    regmatch_t matches[1];
    time_t hora_actual;
	struct tm * timeinfo;



	/**********************************
	*** LECTURA DE LA PETICIÓN HTTP ***
	**********************************/
	if ((nleidos_peticion = read(descriptorFichero, peticion, BUFSIZE)) == -1){
		fprintf(stderr, "Línea: %d. Error reading from socket: %s\n", __LINE__, strerror(errno));

		tratarError(descriptorFichero, SERVERERROR, NULL, NULL);
		debug(ERROR,"system call", "read", 0);
		return;
	} else if (nleidos_peticion == 0){
		return;
	}



	/************
	*** FECHA ***
	************/
	// Obtener la hora actual en una variable time
	if (time(&hora_actual) == -1){
		fprintf(stderr, "Línea: %d. Error getting time: %s\n", __LINE__, strerror(errno));
		tratarError(descriptorFichero, SERVERERROR, NULL, NULL);
		debug(ERROR,"system call", "time", 0);
		return;
	}
	// Convertimos la variable time_t en una estructura tm
	if ((timeinfo = localtime(&hora_actual)) == NULL){
		fprintf(stderr, "Línea: %d. Error en la función 'localtime': %s\n", __LINE__, strerror(errno));
		tratarError(descriptorFichero, SERVERERROR, NULL, NULL);
		debug(ERROR,"function", "localtime", 0);
		return;
	}
	// Le restamos una hora para ajustarlo a la hora GMT
	timeinfo->tm_hour = timeinfo->tm_hour - 1;
	if (strftime(actual_date, BUFSIZE, "%a, %d %b %Y %T GMT", timeinfo) == 0){
		fprintf(stderr, "Línea: %d. Error en la función 'strftime'\n", __LINE__);
		debug(ERROR,"function", "strftime", 0);
		// No hacemos return porque la ejecución puede continuar aunque no haya fecha
	}



	/********************************
	*** TRATAMIENTO DE LA LECTURA ***
	********************************/
	// Terminación buffer en el que se ha leído la petición con un '\0'
	peticion[nleidos_peticion] = '\0';


	/*
		Se sustituyen los caracteres '\r\n' por '**'' con expresiones regulares
		para poder usar de manera cómoda la función 'strtok' y manejar
		las cookies
	*/
	// Compilar la expresión regular '\r\n'
	;
	if ((status_ER = regcomp(&regex, "\r\n", REG_EXTENDED))) {
		fprintf(stderr, "Línea: %d. 'regcomp' no pudo compilar\n", __LINE__);
		tratarError(descriptorFichero, SERVERERROR, NULL, actual_date);
		debug(ERROR,"function", "regcomp", 0);
		return;
	}
	// Encontrar las coincidencias y hacer la sustitución
	while(1){
		status_ER = regexec(&regex, peticion, 1, matches, 0);
		if ((!status_ER) && (matches[0].rm_so != -1)) {
		    // Match
		    peticion[(int) matches[0].rm_so] = '~';
		    peticion[(int) matches[0].rm_so + 1] = '~';
		} else if (status_ER == REG_NOMATCH) {
		    //No match
		    break;
		} else {
		    regerror(status_ER, &regex, peticion, sizeof(peticion));
		    fprintf(stderr, "Línea: %d. Regex match falló\n", __LINE__);
			tratarError(descriptorFichero, SERVERERROR, NULL, actual_date);
		    debug(ERROR,"function", "regexec", 0);
		    return;
		}
	}
	// Liberar la memoria reservada por la función 'regcomp'
	regfree(&regex);



	/*********************************
	*** TRATAMIENTO DE LA PETICIÓN ***
	*********************************/
	/*
		Se va a utilizar 'strtok' para ir analizando la petición HTTP
		del cliente, y así poder tratarla según sea necesario en cada caso.
	*/
	// Comprobamos de que se trata de una petición GET
	if((token = strtok(peticion, " ")) == NULL){
		fprintf(stderr, "Línea: %d. Error en la función 'strtok'\n", __LINE__);
		tratarError(descriptorFichero, SERVERERROR, NULL, actual_date);
		debug(ERROR,"function", "strtok", 0);
		return;
	} else if (strcmp(token, "POST") == 0){
		// Solo se soportan peticiones GET
		tratarError(descriptorFichero, NOTSUPPORTED, NULL, actual_date);
		return;
	} else if(strcmp(token, "GET") != 0){
		// Si la petición GET es incorrecta
		tratarError(descriptorFichero, BADREQUEST, NULL, actual_date);
		return;
	}

	// Leemos el archivo que se solicita
	if((token = strtok(NULL, " ")) == NULL){
		tratarError(descriptorFichero, BADREQUEST, NULL, actual_date);
		return;
	} else if (strcmp(token, "/") == 0){
		if (sprintf(nombre_archivo, "index.html") < 0){
			fprintf(stderr, "Línea: %d. Error en la función 'sprintf'\n", __LINE__);
			tratarError(descriptorFichero, SERVERERROR, NULL, actual_date);
			debug(ERROR,"function", "sprintf", 0);
			return;
		}
	} else if ((strncmp(token, "..", 2) == 0) || (strncmp(token, "/..", 3) == 0)){
		// No se puede acceder -> Acceso ilegal
		tratarError(descriptorFichero, PROHIBIDO, NULL, actual_date);
		return;
	} else {
		if (sprintf(nombre_archivo, "%s", &token[1]) < 0){
			fprintf(stderr, "Línea: %d. Error en la función 'sprintf'\n", __LINE__);
			tratarError(descriptorFichero, SERVERERROR, NULL, actual_date);
			debug(ERROR,"function", "sprintf", 0);
			return;
		}

	}

	// Comprobamos que la versión de HTTP usada es correcta
	if((token = strtok(NULL, "~~")) == NULL){
		tratarError(descriptorFichero, BADREQUEST, NULL, actual_date);
		return;
	} else if(strcmp(token, "HTTP/1.1") != 0){
		tratarError(descriptorFichero, BADREQUEST, NULL, actual_date);
		return;
	}

	// Compilar la expresión regular '\r\n'
	if ((status_ER = regcomp(&regex, ".+:", REG_EXTENDED))) {
		fprintf(stderr, "Línea: %d. 'regcomp' no pudo compilar\n", __LINE__);
		tratarError(descriptorFichero, SERVERERROR, NULL, actual_date);
		debug(ERROR,"function", "regcomp", 0);
		return;
	}

	// Ahora vamos a avanzar en la petición hasta encontrar una cookie (o no)
	memset(cookie, '\0', BUFSIZE);
	hayCookie = 0;
	token = strtok(NULL, "~~");
	while(token != NULL){
		if (strncmp(token, "Cookie:", 7) == 0){
			if (sprintf(cookie, "%s", &token[17]) < 0){
				fprintf(stderr, "Línea: %d. Error en la función 'sprintf'\n", __LINE__);
				tratarError(descriptorFichero, SERVERERROR, NULL, actual_date);
				debug(ERROR,"function", "sprintf", 0);
				return;
			}
			hayCookie = 1;
			break;
		} else {
			// Comprobar campos de la cabecera
			sprintf(linea_actual, "%s", token);
			// Encontrar las coincidencias
			status_ER = regexec(&regex, linea_actual, 1, matches, 0);
			if ((!status_ER) && (matches[0].rm_so != -1)) {
			    // Match
			} else if (status_ER == REG_NOMATCH) {
			    //No match
			    tratarError(descriptorFichero, BADREQUEST, NULL, actual_date);
				return;
			} else {
			    regerror(status_ER, &regex, peticion, sizeof(peticion));
			    fprintf(stderr, "Línea: %d. Regex match falló\n", __LINE__);
				tratarError(descriptorFichero, SERVERERROR, NULL, actual_date);
			    debug(ERROR,"function", "regexec", 0);
			    return;
			}
		}
		token = strtok(NULL, "~~");
	}

	// Liberar la memoria reservada por la función 'regcomp'
	regfree(&regex);



	/*************
	*** COOKIE ***
	*************/
	// Le sumamos dos minutos a la hora actual para que la 
	// cookie expire en ese tiempo
	timeinfo->tm_min = timeinfo->tm_min + 2;
	if (strftime(date, BUFSIZE, "%a, %d %b %Y %T GMT", timeinfo) == 0){
		fprintf(stderr, "Línea: %d. Error en la función 'strftime'\n", __LINE__);
		tratarError(descriptorFichero, SERVERERROR, NULL, actual_date);
		debug(ERROR,"function", "strftime", 0);
		return;
	}

	/*
		Si no contenía una cookie, debemos enviarle una al cliente indicándole que
		es su primer acceso al servidor, y que en dos minutos debe expirar dicha cookie.
		Para ello, además de utilizar el campo 'expires', se envía al cliente la
		hora en la que la cookie debe expirar. De esta manera, el cliente podrá
		informar al servidor en un futuro de la hora a la que tiene que expirar la
		cookie, y que el servidor pueda volver a rellenar el campo 'expires' con el
		valor de la hora anterior
	*/
	if (!hayCookie){

		if (sprintf(cookie, "Set-Cookie: contador=1; Expires=%s\r\n", date) < 0){
			fprintf(stderr, "Línea: %d. Error en la función 'sprintf'\n", __LINE__);
			tratarError(descriptorFichero, SERVERERROR, NULL, actual_date);
			debug(ERROR,"function", "sprintf", 0);
			return;
		}
	}
	/*
		Si el cliente nos ha enviado una cookie que tenía almacenada, el valor
		de dicha cookie es inferior a 10, y la petición del cliente es la página
		'index.html', debemos incrementar la cookie y enviarle el nuevo valor
		al cliente, conservando la fecha de expiración anterior
	*/
	else if (atoi(cookie) < 10){
		if (sprintf(cookie, "Set-Cookie: contador=%d; Expires=%s\r\n", atoi(cookie) + 1, date) < 0){
			fprintf(stderr, "Línea: %d. Error en la función 'sprintf'\n", __LINE__);
			tratarError(descriptorFichero, SERVERERROR, NULL, actual_date);
			debug(ERROR,"function", "sprintf", 0);
			return;
		}
	}
	/*
		Si el cliente nos ha enviado una cookie que tenía almacenada, y el valor
		de dicha cookie es 10, hay que denegar el acceso al contenido al cliente
	*/
	else {
		if (sprintf(cookie, "Set-Cookie: contador=10; Expires=%s\r\n", date) < 0){
			fprintf(stderr, "Línea: %d. Error en la función 'sprintf'\n", __LINE__);
			tratarError(descriptorFichero, SERVERERROR, NULL, actual_date);
			debug(ERROR,"function", "sprintf", 0);
			return;
		}
		tratarError(descriptorFichero, TOOMANYREQUEST, NULL, actual_date);
		return;
	}



	/*********************************************
	*** TRATAR LA EXTENSIÓN DEL ARCHIVO PEDIDO ***
	*********************************************/
	if (sprintf(nombre_archivo_aux, "%s", nombre_archivo) < 0){
		fprintf(stderr, "Línea: %d. Error en la función 'sprintf'\n", __LINE__);
		tratarError(descriptorFichero, SERVERERROR, NULL, actual_date);
		debug(ERROR,"function", "sprintf", 0);
		return;
	}
	// Comenzamos a obtener el nombre del archivo
	if((token = strtok(nombre_archivo_aux, ".")) == NULL){
		fprintf(stderr, "Línea: %d. Error en la función 'strtok'\n", __LINE__);
		tratarError(descriptorFichero, SERVERERROR, NULL, actual_date);
		debug(ERROR,"function", "strtok", 0);
		return;
	}
	// Si el archivo no tiene extensión indicamos que la petición es incorrecta
	if((token = strtok(NULL, ".")) == NULL){
		tratarError(descriptorFichero, BADREQUEST, NULL, actual_date);
		return;
	}
	// Seguimos avanzando por si lo que hemos obtenido era parte del
	// nombre del archivo y no era la extensión
	while(token != NULL){
		if (sprintf(extension, "%s", token) < 0){
			fprintf(stderr, "Línea: %d. Error en la función 'sprintf'\n", __LINE__);
			tratarError(descriptorFichero, SERVERERROR, NULL, actual_date);
			debug(ERROR,"function", "sprintf", 0);
			return;
		}
		token = strtok(NULL, ".");
	}
	for (i = 0; i < EXTENSIONS_SIZE; i++){
		if (strcmp(extension, extensions[i].ext) == 0){
			if (sprintf(ext, "%s", extensions[i].filetype) < 0){
				fprintf(stderr, "Línea: %d. Error en la función 'sprintf'\n", __LINE__);
				tratarError(descriptorFichero, SERVERERROR, NULL, actual_date);
				debug(ERROR,"function", "sprintf", 0);
				return;
			}
			i = EXTENSIONS_SIZE + 1;
		}
	}
	if (i == EXTENSIONS_SIZE){
		// Extensión no soportada
		tratarError(descriptorFichero, EXTENSION, NULL, actual_date);
		return;
	}



	//
	//	Como se trata el caso excepcional de la URL que no apunta a ningún fichero
	//	html
	//
	int fd;
	if ((fd = open(nombre_archivo, O_RDONLY)) == -1){
		if (errno == 2)
			tratarError(descriptorFichero, NOENCONTRADO, NULL, actual_date);
		else
			tratarError(descriptorFichero, SERVERERROR, NULL, actual_date);
		fprintf(stderr, "Línea: %d. Error opening file: %s\n", __LINE__, strerror(errno));
		debug(ERROR,"system call", "open", 0);
		return;
	}



	//
	//	En caso de que el fichero sea soportado, exista, etc. se envia el fichero con la cabecera
	//	correspondiente, y el envio del fichero se hace en blockes de un máximo de  8kB
	//
	if (sprintf(estado, "HTTP/1.1 200 OK\r\n") < 0){
		fprintf(stderr, "Línea: %d. Error en la función 'sprintf'\n", __LINE__);
		tratarError(descriptorFichero, SERVERERROR, NULL, actual_date);
		debug(ERROR,"function", "sprintf", 0);
		return;
	}
	if (write(descriptorFichero, estado, strlen(estado)) == -1){
		fprintf(stderr, "Línea: %d. Error writting to socket: %s\n", __LINE__, strerror(errno));
		tratarError(descriptorFichero, SERVERERROR, NULL, actual_date);
		debug(ERROR,"system call", "write", 0);
		return;
	}

	if ((size = lseek(fd, 0, SEEK_END)) == -1){
		fprintf(stderr, "Línea: %d. Error in 'lseek' system call: %s\n", __LINE__, strerror(errno));
		tratarError(descriptorFichero, SERVERERROR, NULL, actual_date);
		debug(ERROR,"system call", "lseek", 0);
		return;
	}
	if (lseek(fd, 0, SEEK_SET) == -1){
		fprintf(stderr, "Línea: %d. Error in 'lseek' system call: %s\n", __LINE__, strerror(errno));
		tratarError(descriptorFichero, SERVERERROR, NULL, actual_date);
		debug(ERROR,"system call", "lseek", 0);
		return;
	}

	
	if (sprintf(cabecera, "Date: %s\r\nServer: j0sem1\r\nKeep-Alive: timeout=10, max=100\r\nConnection: Keep-Alive\r\nContent-Type: %s\r\nContent-Length: %d\r\n%s\r\n", actual_date, ext, size, cookie) < 0){
		fprintf(stderr, "Línea: %d. Error en la función 'sprintf'\n", __LINE__);
		tratarError(descriptorFichero, SERVERERROR, NULL, actual_date);
		debug(ERROR,"function", "sprintf", 0);
		return;
	}
	if (write(descriptorFichero, cabecera, strlen(cabecera)) == -1){
		fprintf(stderr, "Línea: %d. Error writting to socket: %s\n", __LINE__, strerror(errno));
		tratarError(descriptorFichero, SERVERERROR, NULL, actual_date);
		debug(ERROR,"system call", "write", 0);
		return;
	}
	

	readwrite(descriptorFichero, fd, cookie, actual_date);

	if (close(fd) == -1){
		fprintf(stderr, "Línea: %d. Error closing file: %s\n", __LINE__, strerror(errno));
		tratarError(descriptorFichero, SERVERERROR, NULL, actual_date);
		debug(ERROR,"system call", "close", 0);
		return;
	}

	return;
}

int main(int argc, char **argv)
{
	int i, port, pid, listenfd, socketfd;
	socklen_t length;
	static struct sockaddr_in cli_addr;		// static = Inicializado con ceros
	static struct sockaddr_in serv_addr;	// static = Inicializado con ceros
	
	//  Argumentos que se esperan:
	//
	//	argv[1]
	//	En el primer argumento del programa se espera el puerto en el que el servidor escuchara
	//
	//  argv[2]
	//  En el segundo argumento del programa se espera el directorio en el que se encuentran los ficheros del servidor
	//
	//  Verificar que los argumentos que se pasan al iniciar el programa son los esperados
	//

	//
	//  Verficiar que el directorio escogido es apto. Que no es un directorio del sistema y que se tienen
	//  permisos para ser usado
	//

	if(chdir(argv[2]) == -1){ 
		(void)printf("ERROR: No se puede cambiar de directorio %s\n",argv[2]);
		exit(4);
	}

	// Hacemos que el proceso sea un demonio sin hijos zombies
	if(fork() != 0)
		return 0; // El proceso padre devuelve un OK al shell

	(void)signal(SIGCHLD, SIG_IGN); // Ignoramos a los hijos
	(void)signal(SIGHUP, SIG_IGN); // Ignoramos cuelgues
	
	debug(LOG,"web server starting...", argv[1] ,getpid());
	
	/* setup the network socket */
	if((listenfd = socket(AF_INET, SOCK_STREAM,0)) <0)
		debug(ERROR, "system call","socket",0);
	
	port = atoi(argv[1]);
	
	if(port < 0 || port >60000)
		debug(ERROR,"Puerto invalido, prueba un puerto de 1 a 60000",argv[1],0);
	
	/*Se crea una estructura para la información IP y puerto donde escucha el servidor*/
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); /*Escucha en cualquier IP disponible*/
	serv_addr.sin_port = htons(port); /*... en el puerto port especificado como parámetro*/
	
	if(bind(listenfd, (struct sockaddr *)&serv_addr,sizeof(serv_addr)) <0)
		debug(ERROR,"system call","bind",0);
	
	if( listen(listenfd,64) <0)
		debug(ERROR,"system call","listen",0);
	
	while(1){
		length = sizeof(cli_addr);
		if((socketfd = accept(listenfd, (struct sockaddr *)&cli_addr, &length)) < 0)
			debug(ERROR,"system call","accept",0);
		if((pid = fork()) < 0) {
			debug(ERROR,"system call","fork",0);
		}
		else {
			if(pid == 0) { 	// Proceso hijo
				(void)close(listenfd);

				/*******************
				*** PERSISTENCIA ***
				*******************/
				fd_set rfds;
				struct timeval tv;
				int exit_select;
				// Esperar hasta 10 segundos
				tv.tv_sec = 10;
				tv.tv_usec = 0;
				// Observar el descriptor 'socketfd' para ver cuando llega una nueva petición
				FD_ZERO(&rfds);
				FD_SET(socketfd, &rfds);
				while((exit_select = select(socketfd + 1, &rfds, NULL, NULL, &tv)) > 0){
					process_web_request(socketfd);
					tv.tv_sec = 10;
					tv.tv_usec = 0;
				}
				// Comprobamos si select ha tenido algún error
				if (exit_select == -1){
					debug(ERROR,"system call","select",0);
					fprintf(stderr, "Línea: %d. Error in 'select' system call: %s\n", __LINE__, strerror(errno));
					exit(EXIT_FAILURE);
				}
				// Cerramos el socket
				if (close(socketfd) == -1){
					debug(ERROR,"system call","close",0);
					fprintf(stderr, "Línea: %d. Error in 'close' system call: %s\n", __LINE__, strerror(errno));
					exit(EXIT_FAILURE);
				}

				// El hijo termina correctamente
				exit(EXIT_SUCCESS);
			} else { 	// Proceso padre
				(void)close(socketfd);
			}
		}
	}
}
