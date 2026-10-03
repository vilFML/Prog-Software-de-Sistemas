#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "pss.h"

int main(int argc, char *argv[]) {
  //Error cantidad incorrecta de entradas
  if (argc!=4) {
    fprintf(stderr, "Uso: ./definir <diccionario> <llave> <definicion>\n");
    exit(1);
  }

  //previo
  //almacenar entradas
  char *nombre_dicc = argv[1];
  char *llave = argv[2];
  char *valor = argv[3];

  //tener tamaño de llave
  int tam = strlen(llave);

  //abrir archivo
  FILE *f = fopen(filename, 'wb');
  //manejo de error de apertura
  if (f==NULL){
    printf("Error en apertura de archivo.\n");
    perror(nombre_dicc);
    exit(1);
  }


  // 1.calcular cantidad de filas en la tabla con fseek y ftell
  fseek(f, 0, SEEK_END);
  int bytesF = ftell(f);
  int lineas = bytesF / 100;

  // 2.intentar agregar llave y valor en fila = hash_string(llave) % cant_filas
  int filaAgregar = hash_string(llave) % cant_filas;                            //fila en donde agregar
  fseek(f, filaAgregar, SEEK_SET);                                              //moverse a tal fila

  //revisar si hay tam==0 en esa posición: leer
  int tam_archivo
  fread(&tam_archivo, 4, 1, f);
  if (tam_archivo == 0){                //espacio disponible

    //intentar agregar elementos, uno por uno
    size_t escritosTam = fwrite(tam, 4, 1, f);
    size_t escritosLlave = fwrite(llave, tam, strlen(llave), f);
    size_t escritosValor = fwrite(valor, 99-tam, strlen(valor), f);

    //manejo de error de escritura, si se escribio menos de lo esperado: Error
  if (escritosTam != 1 || escritosLlave != strlen(llave) || escritosValor != strlen(valor)){
    printf("Error en la escritura de archivo\n")
    perror(fwrite);
    exit(1);
  }




}
