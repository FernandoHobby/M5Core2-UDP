# M5Core2-UDP
Comunicación UDP entre M5Core2 y M5StampC3

El M5Cores2 actua como cliente y envia periodicamente un mensaje UDP al servidor (M5StampC3), en el mensaje le envía unos valores RGB aleatorios para que modifique el color del led del server.
El periódo entre mensajes UDP se puede variar mediante los botones del M5Core2. El periódo mínimo esta fijado en 25 milisegundos ( 40 mensajes por segundo ), este periódo está limitado por el tiempo de proceso del M5Cores2 aunque, reduciendo algunas instrucciones de pantalla, se puede reducir unos milisegundos mas.
El cliente envía los mensajes UDP sin esperar a recibir el 'eco' del server. 
El server, en cuanto recibe un mensaje UDP del cliente, envía un 'eco' copiando los datos recibidos. Si el periódo entre mensajes es muy bajo (25 mseg) es posible que el cliente envíe varios mensajes UDP y el server no envíe el 'eco' inmediatamente. 
