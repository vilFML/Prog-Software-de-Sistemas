ARCH=$(arch)
DEFINIR=./$1
REF=./prof.ref-$ARCH
chmod +x prof.ref-$ARCH

# Todos los archivos que necesita este test se crean en este directorio,
# para no llenar de archivos el directorio de la tarea.
DIR=tmp-test
DICC=$DIR/dicc.ht
rm -rf $DIR
mkdir $DIR

diag() {
  echo "Si la salida parece ser igual, instale xxdiff con:"
  echo "    sudo apt-get install xxdiff"
  echo "y luego compare los caracteres invisibles con:"
  echo $*
}

terminar() {
  echo "Para depurar lance ddd con: make ddd"
  echo "En la ventana de ddd, coloque un breakpoint con: b main"
  echo "Tambien es util colocar un breakpoint en exit con: b exit"
  echo "Ejecute con: $*"
  echo "Ejecute paso a paso con el boton next"
  exit 1
}

# Construye un diccionario vacio de $2 filas en el archivo $1.
# Una fila vacia son 100 bytes en 0, de modo que un diccionario vacio
# es simplemente un archivo de 100*$2 bytes en 0.
mk_vacio() {
  head -c $((100 * $2)) /dev/zero > $1
}

# Muestra el contenido de un diccionario, una linea por fila, indicando
# el numero de la fila para poder ubicar los errores de sondeo lineal.
volcar() {
  od -An -v -tu1 -w100 $1 | awk '
    {
      tam= $1
      if (tam==0) {
        printf "fila %3d: vacia\n", NR-1
        next
      }
      llave= ""
      valor= ""
      for (i= 2; i<=100; i++) {
        c= $i
        if (c>=32 && c<127)
          ch= sprintf("%c", c)
        else
          ch= sprintf("\\%03o", c)
        if (i-1<=tam)
          llave= llave ch
        else
          valor= valor ch
      }
      printf "fila %3d: tam=%3d llave=[%s] valor=[%s]\n", NR-1, tam, llave, valor
    }'
}

# Muestra el contenido del archivo $1 destacandolo del resto de los
# mensajes.  Si el archivo esta vacio muestra en su lugar el aviso $2.
mostrar() {
  if [ -s $1 ]
  then
    sed 's/^/  | /' $1
  else
    echo "  $2"
  fi
}

# Compara la salida estandar, la salida estandar de errores, el codigo de
# retorno y el diccionario resultante de su solucion con los del binario
# del profesor.  Supone que su solucion ya se ejecuto dejando sus
# resultados en $DIR/raw-std.txt, $DIR/err.txt, RC y $DIR/mio.ht, y el
# binario del profesor en $DIR/raw-std-ref.txt, $DIR/err-ref.txt, REFRC
# y $DIR/ref.ht.  Las comparaciones son byte a byte.
comparar() {
  cmp $DIR/err.txt $DIR/err-ref.txt 2> /dev/null 1> /dev/null
  if [ $? -ne 0 ]
  then
    echo
    echo "*** Salida estandar de errores incorrecta ***"
    echo "Su solucion entrega:"
    mostrar $DIR/err.txt "(salida vacia)"
    echo "La salida esperada es:"
    mostrar $DIR/err-ref.txt "(salida vacia)"
    diag "xxdiff $DIR/err.txt $DIR/err-ref.txt"
    terminar "run $*"
  fi
  cat $DIR/err.txt

  cmp $DIR/raw-std.txt $DIR/raw-std-ref.txt 2> /dev/null 1> /dev/null
  if [ $? -ne 0 ]
  then
    echo
    echo "*** Salida estandar incorrecta ***"
    echo "Su solucion entrega:"
    mostrar $DIR/raw-std.txt "(salida vacia)"
    echo "La salida esperada es:"
    mostrar $DIR/raw-std-ref.txt "(salida vacia)"
    diag "xxdiff $DIR/raw-std.txt $DIR/raw-std-ref.txt"
    terminar "run $*"
  fi
  cat $DIR/raw-std.txt

  if [ "$RC" -ne "$REFRC" ]
  then
    echo
    echo "*** El codigo de retorno es incorrectamente $RC. Debio ser $REFRC ***"
    terminar "run $*"
  fi

  if [ -f $DIR/mio.ht ]
  then
    cmp $DIR/mio.ht $DIR/ref.ht 2> /dev/null 1> /dev/null
    if [ $? -ne 0 ]
    then
      echo
      echo "*** El diccionario resultante es incorrecto ***"
      echo "Su solucion deja este diccionario:"
      volcar $DIR/mio.ht | sed 's/^/  | /'
      echo "El diccionario esperado es:"
      volcar $DIR/ref.ht | sed 's/^/  | /' 
      echo "Recuerde que el valor se completa con espacios en blanco y que"
      echo "la llave se inserta en la primera fila desocupada a partir de"
      echo "hash_string(llave) modulo la cantidad de filas."
      terminar "run $*"
    fi
  fi

  if [ "$RC" -ne 0 ]
  then
    echo
    echo "Bien.  Se diagnostico correctamente el error."
  fi
}

# Ejecuta un comando con su solucion y con el binario del profesor,
# partiendo ambos del mismo diccionario, y compara los resultados.
# El diccionario se llama igual en las dos ejecuciones porque su nombre
# aparece en los mensajes de diagnostico.
paso() {
  echo "Ejecutando: $DEFINIR $DICC $*"

  cp $DIR/mio.ht $DICC
  ( $DEFINIR $DICC "$@" > $DIR/raw-std.txt ) >& $DIR/err.txt
  RC=$?
  cp $DICC $DIR/mio.ht

  cp $DIR/ref.ht $DICC
  ( $REF $DICC "$@" > $DIR/raw-std-ref.txt ) >& $DIR/err-ref.txt
  REFRC=$?
  cp $DICC $DIR/ref.ht

  comparar $DICC "$@"
}

# Crea un diccionario vacio de $1 filas y ejecuta sobre el la secuencia de
# inserciones dada por los pares llave/valor que siguen.
# Uso: test <filas> <llave> <valor> [<llave> <valor> ...]
test() {
  local filas=$1
  shift
  echo "Diccionario vacio de $filas filas"
  mk_vacio $DIR/mio.ht $filas
  mk_vacio $DIR/ref.ht $filas
  while [ $# -gt 0 ]
  do
    paso "$1" "$2"
    shift 2
  done
}

# Ejecuta un comando sin crear ningun diccionario y compara los resultados.
# Sirve para los errores en la cantidad de parametros y para el caso en que
# el diccionario no existe.
test_sin_dicc() {
  echo "Ejecutando: $DEFINIR $*"
  rm -f $DICC $DIR/mio.ht $DIR/ref.ht
  ( $DEFINIR "$@" > $DIR/raw-std.txt ) >& $DIR/err.txt
  RC=$?
  ( $REF "$@" > $DIR/raw-std-ref.txt ) >& $DIR/err-ref.txt
  REFRC=$?
  comparar "$@"
}

# Ejecuta un comando sobre un diccionario con los permisos dados en $1.
# Uso: test_permisos <modo> <llave> <valor>
test_permisos() {
  local modo=$1
  shift
  echo "Ejecutando: $DEFINIR $DICC $* con permisos $modo"
  rm -f $DIR/mio.ht $DIR/ref.ht

  mk_vacio $DICC 5
  chmod $modo $DICC
  ( $DEFINIR $DICC "$@" > $DIR/raw-std.txt ) >& $DIR/err.txt
  RC=$?

  chmod 600 $DICC
  mk_vacio $DICC 5
  chmod $modo $DICC
  ( $REF $DICC "$@" > $DIR/raw-std-ref.txt ) >& $DIR/err-ref.txt
  REFRC=$?
  chmod 600 $DICC

  comparar $DICC "$@"
}

echo "-----------------------------------------------------------"
echo "Test de una insercion en un diccionario vacio"
test 7 casa "edificacion en donde vive una familia"

echo "-----------------------------------------------------------"
echo "Test de inserciones en filas distintas"
test 7 casa "edificacion en donde vive una familia" \
        alimento "sustancia que se ingiere para nutrirse"

echo "-----------------------------------------------------------"
echo "Test de una colision: ambas llaves van a la fila 5"
test 7 gato "mamifero domestico de habitos nocturnos" \
        celular "telefono que cabe en el bolsillo"

echo "-----------------------------------------------------------"
echo "Test de un sondeo lineal sobre varias filas ocupadas"
test 7 gato "mamifero domestico de habitos nocturnos" \
        celular "telefono que cabe en el bolsillo" \
        bolsillo "bolsa pequena cosida en la ropa" \
        byte "unidad de informacion de ocho bits"

echo "-----------------------------------------------------------"
echo "Test de un sondeo que da la vuelta al final del archivo"
test 7 perro "mamifero domestico con olfato muy fino" \
        lluvia "agua que cae de las nubes"

echo "-----------------------------------------------------------"
echo "Test de un sondeo que da la vuelta desde la ultima fila"
test 8 canario "ave amarilla que canta" \
        puntero "flecha que apunta a donde Ud. cree"

echo "-----------------------------------------------------------"
echo "Test que ocupa la ultima fila libre de la tabla"
test 3 techo "parte superior de una construccion" \
        gato "mamifero domestico de habitos nocturnos" \
        perro "mamifero domestico con olfato muy fino"

echo "-----------------------------------------------------------"
echo "Test de insercion en un diccionario completamente lleno"
test 3 techo "parte superior de una construccion" \
        gato "mamifero domestico de habitos nocturnos" \
        perro "mamifero domestico con olfato muy fino" \
        casa "edificacion en donde vive una familia"

echo "-----------------------------------------------------------"
echo "Test de un diccionario de una sola fila"
test 1 byte "unidad de informacion de ocho bits" \
        bit "byte que todavia esta en formacion"

echo "-----------------------------------------------------------"
echo "Test de una llave que ya se encuentra en su propia fila"
test 7 casa "edificacion en donde vive una familia" \
        casa "otra definicion cualquiera"

echo "-----------------------------------------------------------"
echo "Test de una llave que ya se encuentra, detectada al sondear"
test 7 gato "mamifero domestico de habitos nocturnos" \
        celular "telefono que cabe en el bolsillo" \
        celular "otra definicion cualquiera"

echo "-----------------------------------------------------------"
echo "Test de una llave mas una definicion que ocupan exactamente 99 bytes"
test 7 byte "$(head -c 95 /dev/zero | tr '\0' x)"

echo "-----------------------------------------------------------"
echo "Test de una llave mas una definicion que ocupan 100 bytes"
test 7 byte "$(head -c 96 /dev/zero | tr '\0' x)"

echo "-----------------------------------------------------------"
echo "Test de una llave mas una definicion muy largas"
test 7 "$(head -c 60 /dev/zero | tr '\0' k)" "$(head -c 60 /dev/zero | tr '\0' v)"

echo "-----------------------------------------------------------"
echo "Test de una definicion vacia"
test 7 byte ""

echo "-----------------------------------------------------------"
echo "Test de una llave de un solo byte"
test 7 a "primera letra del abecedario"

echo "-----------------------------------------------------------"
echo "Test de una llave y una definicion con espacios en blanco"
test 7 "byte nulo" "el que nunca esta cuando uno lo busca"

echo "-----------------------------------------------------------"
echo "Test de un diccionario que se va llenando"
mk_vacio $DIR/mio.ht 17
mk_vacio $DIR/ref.ht 17
for i in $(seq 1 15)
do
  paso "llave$i" "definicion numero $i"
done

echo "-----------------------------------------------------------"
echo "Test de uso incorrecto de parametros"
test_sin_dicc
test_sin_dicc $DICC
test_sin_dicc $DICC byte
test_sin_dicc $DICC byte "unidad de informacion" sobra

echo "-----------------------------------------------------------"
echo "Test con un diccionario inexistente"
test_sin_dicc $DIR/nodicc.ht byte "unidad de informacion de ocho bits"

echo "-----------------------------------------------------------"
echo "Test con un diccionario sin permiso de lectura"
test_permisos 222 byte "unidad de informacion de ocho bits"

echo "-----------------------------------------------------------"
echo "Test con un diccionario sin permiso de escritura"
test_permisos 444 byte "unidad de informacion de ocho bits"

echo
echo "Felicitaciones: aprobo todos los tests unitarios"
