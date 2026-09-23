# _CC3301 Programación de Software de Sistemas – Primavera 2026 – Tarea 3 – Profs. Mateu/Ibarra/Lehmann_



En un _recorrido en orden_ de un árbol binario, se visita recursivamente
primero el subárbol izquierdo, luego se visita la raíz y finalmente se
visita recursivamente el subárbol derecho.  Considere que se está
visitando un nodo T al recorrer un árbol binario en orden. Se define
como _previo_ a T el nodo que se visitó anteriormente, y como _próximo_ el
nodo que se visitará a continuación. Estudie el lado derecho de la figura
de ejemplo. Programe en el archivo _prev.c_ la función _asignarPrev_ que
asigna los campos _prev_ y _prox_ agregados a la estructura de los nodos de
un árbol _t_ .  El encabezado de la función se muestra a la derecha. El parámetro _*pprev_ es de entrada y salida.
```
typedef struct nodo {
   int x;
   struct nodo *izq, *der;
   struct nodo *prev, prox;
} Nodo;

void asignarPrev(Nodo *t, Nodo **prev);
```
El nodo previo del primer nodo visitado (el nodo 1 en el ejemplo) debe ser el nodo apuntado inicialmente por _*pprev_ (nodo 0) y el nodo próximo del último nodo en
ser visitado (nodo 5) debe ser NULL.  En _*pprev_ debe quedar finalmente la dirección del último nodo visitado (nodo 5). En el siguiente ejemplo de uso las variables _t_ y _prev_ son de tipo Nodo *.


_**Restricción**_ : Su solución debe tomar tiempo linealmente proporcional al
número de nodos en el árbol _t_ .

_Ayuda_ : Cuando visite el nodo T, su nodo previo es _*pprev_ . Asigne
NULL a su nodo próximo por ahora. Si el nodo previo a T no es NULL,
T es el nodo próximo del nodo previo a T. Antes de continuar el
recorrido, asigne T a _*pprev_ .

# **_Instrucciones_**

Descargue _t3.zip_ de U-cursos y descomprímalo.  El directorio _T3_
contiene los archivos (a) _test-prev.c_ que prueba si su tarea funciona y
compara su eficiencia con la solución del profesor, (b) _prof.ref-x86_64 y_
_prof.ref-aarch64_ con los binarios ejecutables de la solución del profesor,
(c) _prev.h_ que incluye los encabezados de las funcion pedidas, (d)
_Makefile_ que le servirá para compilar y ejecutar su tarea, y (e) _prev.cbf_



para que pueda probar su tarea con _codeblocks_ . **Ejecute en un terminal**
**el comando** _**make**_ para recibir instrucciones adicionales. Estos son los
requerimientos para aprobar su tarea.

   - _make run_ debe felicitarlo por aprobar este modo de ejecución.
Su solución no debe ser 80% más lenta que la solución del
profesor.

   - _make run-g_ debe felicitarlo.

   - _make run-san_ debe felicitarlo y no reportar ningún problema
como por ejemplo _heap-buffer-overflow_ .


Cuando pruebe su tarea con _make run_ asegúrese que su computador esté
configurado en modo alto rendimiento y que no estén corriendo otros
procesos intensivos en uso de CPU al mismo tiempo. De otro modo
podría no lograr la eficiencia solicitada.

# **_Entrega_**


Ud. solo debe entregar por medio de U-cursos el archivo _prev.zip_
generado por el comando _make zip_ . **A continuación es muy**
**importante que descargue de U-cursos el mismo archivo que subió,**
**luego descargue nuevamente los archivos adjuntos y vuelva a**
**probar la tarea tal cual como la entregó** . Esto es para evitar que Ud.
reciba un 1.0 en su tarea porque entregó los archivos equivocados.
Créame, sucede a menudo por ahorrarse esta verificación. Se descontará
medio punto por día de atraso. No se consideran los días de receso,
sábados, domingos o festivos.


