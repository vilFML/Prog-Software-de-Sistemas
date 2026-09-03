==========================================================
Esta es la documentación para compilar y ejecutar su tarea
==========================================================

Se está ejecutando el comando: less README.txt

***************************
*** Para salir: tecla q ***
***************************

Para avanzar a una nueva página: tecla <page down>
Para retroceder a la página anterior: tecla <page up>
Para avanzar una sola línea: tecla <enter>
Para buscar un texto: tecla / seguido del texto (/...texto...)
         por ejemplo: /ddd

-----------------------------------------------

Instrucciones para la tarea 2

Ud. debe crear el archivo desescapar.c y programar ahí las funciones
solicitadas:

    void  desescapar(char *str);
    char *desescapado(const char *str);

En el archivo desescapar.c.plantilla encontrará el esqueleto para
empezar.  Cópielo con:

    cp desescapar.c.plantilla desescapar.c

Debe probar su tarea bajo Debian 13 nativo o virtualizado.
Queda excluido WSL 1 para hacer las pruebas.  Sí puede usar WSL 2.
Estos son los requerimientos para aprobar su tarea:

+ make run-san debe felicitarlo y no debe reportar ningún problema como
  por ejemplo goteras de memoria o accesos invalidos a memoria.
+ make run-g debe felicitarlo.
+ make run debe felicitarlo por aprobar este modo de ejecución.  Esta
  prueba será rechazada si su solución es 80% más lenta que la
  solución del profesor.

Cuando pruebe su tarea con make run en su computador asegúrese de que
que está configurado en modo alto rendimiento y que no estén corriendo
otros procesos intensivos en uso de CPU al mismo tiempo.  De otro modo
podría no lograr la eficiencia solicitada.

Los tests verifican, entre otras cosas:

+ Que las secuencias de escape se reemplacen por el byte correcto.
+ Que las secuencias de escape incompletas se traten como indica el
  enunciado: se elimina la barra invertida y lo que sigue se mantiene
  tal cual.
+ Que el byte producido por una secuencia de escape no se vuelva a
  interpretar como parte de otra secuencia.
+ Que \x consuma exactamente dos dígitos hexadecimales, ni más ni menos.
+ Que el resultado pueda contener bytes nulos, producidos por \x00.
+ Que desescapado no modifique el string que recibe.
+ Que desescapado pida exactamente los bytes que ocupa el resultado.
+ Que desescapar y desescapado entreguen siempre el mismo resultado.

Para depurar use: make ddd

Video con ejemplos de uso de ddd: https://youtu.be/FtHZy7UkTT4
Archivos con los ejemplos: https://www.u-cursos.cl/ingenieria/2020/2/CC3301/1/novedades/r/demo-ddd.zip

-----------------------------------------------

Entrega de la tarea

Entregue su archivo desescapar.c a través de https://arbiterd.dcc.uchile.cl

A modo de respaldo debe además ejecutar:

    make zip

que ejecuta todos los tests y genera el archivo desescapar.zip, con su
solución en desescapar.c y la salida de make run, make run-g y make
run-san en resultados.txt.  Entregue ese archivo por U-cursos.

A continuación es muy importante que descargue de U-cursos el mismo
archivo que subió, luego descargue nuevamente los archivos adjuntos y
vuelva a probar la tarea tal cual como la entregó.  Esto es para
evitar que Ud. reciba un 1.0 en su tarea porque entregó los archivos
equivocados.  Créame, sucede a menudo por ahorrarse esta verificación.

-----------------------------------------------

Limpieza de archivos

make clean

Hace limpieza borrando todos los archivos que se pueden volver
a reconstruir a partir de los fuentes: *.o binarios etc.

-----------------------------------------------

Acerca del comando make

El comando make sirve para automatizar el proceso de compilación asegurando
recompilar el archivo binario ejecutable cuando cambió uno de los archivos
fuentes de los cuales depende.

A veces es útil usar make con la opción -n para que solo muestre
exactamente qué comandos va a ejecutar, sin ejecutarlos de verdad.
Por ejemplo:

   make -n ddd

También es útil usar make con la opción -B para forzar la recompilación
de los fuentes a pesar de que no han cambiado desde la última compilación.
Por ejemplo:

   make -B run
