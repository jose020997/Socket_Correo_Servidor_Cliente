#ifndef protocolostpte_practicas_headerfile
#define protocolostpte_practicas_headerfile
#endif

// COMANDOS DE APLICACION
#define SC "USER"  
#define PW "PASS"  

#define SD  "QUIT"  
#define ECHO "ECHO"
#define SUM "SUM " //comando para definir


// RESPUESTAS A COMANDOS DE APLICACION
#define OK  "OK"
#define ER  "ER"

//FIN DE RESPUESTA
#define CRLF "\r\n"

//ESTADOS
#define S_INIT 0
#define S_USER 1
#define S_PASS 2
#define S_DATA 3
#define S_QUIT 4


//PUERTO DEL SERVICIO
#define TCP_SERVICE_PORT	6000

// NOMBRE Y PASSWORD AUTORIZADOS
#define USER		"alumno"
#define PASSWORD	"123456"