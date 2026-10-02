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

Ud. debe programar su solución en el archivo definir.c.  Use como
punto de partida la plantilla definir.c.plantilla:

cp definir.c.plantilla definir.c

Debe probar su tarea bajo Debian 13 de 64 bits nativo o virtualizado.
Queda excluido WSL 1 para hacer las pruebas.  Sí puede usar WSL 2.
Estos son los requerimientos para aprobar su tarea:

+ make run-san debe felicitarlo y no debe reportar ningún problema como
  por ejemplo memory leaks.
+ make run-g debe felicitarlo.
+ make run debe felicitarlo.

Invoque el comando make zip para ejecutar todos los tests y generar un
archivo definir.zip que contiene definir.c, con su solución,
y resultados.txt, con la salida de make run, make run-g y make run-san.

Para ejecutar la solución de referencia, ejecute estos comandos:

ARCH=$(arch)
bash test-definir.sh prof.ref-$ARCH

Para estudiar el formato del diccionario, ejecute: make revisar

Este comando compila y ejecuta revisar.c, un programa que muestra todas
las llaves y definiciones almacenadas en el diccionario dicc.ht.

Para depurar use: make ddd

Recuerde que para ejecutar el programa debe ingresar run en el panel de
comandos de ddd, especificando los parámetros.  Por ejemplo:

run dicc.ht lapiz "instrumento para escribir"

Tenga en cuenta que definir modifica el diccionario, de modo que una
segunda ejecución con la misma llave reportará que la llave ya se
encuentra en el diccionario.  Le conviene depurar con una copia:

cp dicc.ht copia.ht

y en ddd:

run copia.ht lapiz "instrumento para escribir"

Video con ejemplos de uso de ddd: https://youtu.be/FtHZy7UkTT4
Archivos con los ejemplos: https://www.u-cursos.cl/ingenieria/2020/2/CC3301/1/novedades/r/demo-ddd.zip

-----------------------------------------------

Entrega de la tarea

Ejecute: make zip

Entregue por U-cursos el archivo definir.zip

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
