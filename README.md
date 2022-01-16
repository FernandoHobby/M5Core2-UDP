# M5Core2-UDP
Comunicación UDP entre M5Core2 y M5StampC3

El M5Cores2 actua como cliente y envia periodicamente un mensaje UDP al servidor (M5StampC3), en el mensaje le envia unos valores RGB aleatorios para que modifique el color del led del server.
El periodo entre mensajes UDP se puede variar mediante los botones del M5Core2. El periodo minimo esta fijado en 25 milisegundos ( 40 mensajes por segundo ), este periodo esta limitado por el tiempo de proceso del M5Cores2 aunque, reduciendo algunas instrucciones de pantalla, se puede reducir unos milisengos mas.
El cliente envia los mensajes UDP sin esperar a recibir el 'eco' del server. 
El server, en cuanto recibe un mensaje UDP del cliente, envia un 'eco' copiando los datos recibidos. Si el periodo entre mensajes es muy bajo (25 mseg) es posible que el cliente envie varios mensajes UDP y el server no envie el 'eco' inmediatamente. 
