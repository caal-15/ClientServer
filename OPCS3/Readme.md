Este pretende ser un software cliente servidor que presta servicios de
operaciones aritméticas sencillas, lo único que se debe tener en cuenta
a la hora de ejecutar el programa, es que se debe ejecutar primero el servidor.
El software verifica si hay trabajadores libres antes de enviarles trabajo.

Use el comando "make" para crear los archivos necesarios (para usar el make
incluido las librerías zmq y czmq deben estar en los directorios por defecto)

Para correr el servidor:
./server (no recibe parámetros)

Para correr el cliente:
./client (no recibe parámetros)

Para correr el worker:
./slave (string tipo = "add", int delay = 0)

El worker recibe una cadena con el tipo (sino se provee es add por defecto)
y un delay para simular carga (que es 0 por defecto).

Para usar el cliente:
A diferencia de lo propuesto en clase el cliente es bloqueante, espera una
respuesta para seguir (por razones de claridad para poder hacer el testing)
para usarlo simplemente introduzca el nombre de una operación seguido de
los operandos (ej: add 5 7) y presione enter.

