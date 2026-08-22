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

Ud. debe crear el archivo comprimir.c y programar ahí la función
solicitada. Ya hay una plantilla para comprimir.c en comprimir.c.plantilla.

Debe probar su tarea bajo Debian 11 de 64 bits nativo o virtualizado.
Queda excluido WSL 1 para hacer las pruebas.  Sí puede usar WSL 2.
Estos son los requerimientos para aprobar su tarea:

+ make run-san debe felicitarlo y no debe reportar ningún problema como
  por ejemplo desplazamientos indefinidos.
+ make run-g debe felicitarlo.
+ make run debe felicitarlo por aprobar este modo de ejecución.  Esta
  prueba será rechazada si la función solicitada es 80% más lenta que
  la solución del profesor.

Cuando pruebe su tarea con make run en su computador asegúrese de que
que está configurado en modo alto rendimiento y que no estén corriendo
otros procesos intensivos en uso de CPU al mismo tiempo.  De otro modo
podría no lograr la eficiencia solicitada.

Invoque el comando make zip para ejecutar todos los tests y generar un
archivo comprimir.zip que contiene comprimir.c, con su solución,
y resultados.txt, con la salida de make run, make run-g y make run-san.

Para depurar use: make ddd

Video con ejemplos de uso de ddd: https://youtu.be/FtHZy7UkTT4
Archivos con los ejemplos: https://www.u-cursos.cl/ingenieria/2020/2/CC3301/1/novedades/r/demo-ddd.zip

-----------------------------------------------

Entrega de la tarea

Use la plataforma arbiterd.dcc.uchile.cl para probar y entregar su tarea.
En esa plataforma Ud. debe subir el archivo comprimir.c.  Su tarea será
rechazada por la plataforma si no pasa exitosamente todos los tests.

Como se trata de una plataforma experimental, además debe entregar
su tarea a través de U-cursos a modo de respaldo.  Para ello invoque
el comando make zip para ejecutar todos los tests y generar un archivo
comprimir.zip que contiene comprimir.c, con su solución, y resultados.txt,
con la salida de make run, make run-g y make run-san.

Ud. debe entregar por U-cursos el archivo comprimir.zip.

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
