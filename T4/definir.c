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
  int filaAgregar = hash_string(llave) % cant_filas;
  fseek(f, filaAgregar, SEEK_SET);

  




}
