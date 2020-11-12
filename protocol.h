#ifndef protocolostpte_practicas_headerfile
#define protocolostpte_practicas_headerfile
#endif

// COMANDOS DE APLICACION
#define HELO "helo"
#define RCPT "rcpt to:"
#define MAIL "mail from:"
#define SD  "quit"

//lo quitaremos despues
#define SC "USER"  
#define PW "PASS"  
#define ECHO "ECHO"



// RESPUESTAS A COMANDOS DE APLICACION
#define OK  "OK"
#define ER  "ER"

//FIN DE RESPUESTA
#define CRLF "\r\n"

//ESTADOS
#define S_HELO 0
#define S_MAIL 1
#define S_RCPT 2
#define S_DATA 3
#define S_QUIT 4


//PUERTO DEL SERVICIO
#define TCP_SERVICE_PORT	25

// NOMBRE Y PASSWORD AUTORIZADOS
#define USER		"alumno"
#define PASSWORD	"123456"