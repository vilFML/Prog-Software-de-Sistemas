#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "pss.h"

int main(int argc, char *argv[]) {
  //Error cantidad incorrecta de entradas
  if (argc!=4) {
    fprintf(stderr, "Uso: ./definir <diccionario> <key> <definicion>\n");
    exit(1);
  }

  //preprocess
  //store input
  char *filename = argv[1];
  char *key = argv[2];
  char *val = argv[3];

  //get sizes
  size_t tam = strlen(key);
  size_t tamval = strlen(val);
  
                                        //int tam = strlen(key);

  //manage info exceeds maximum 100B
  if (tam + tamval > 99){
    fprintf(stderr, "Tamanno de la llave mas tamanno de la definicion (%d) "
            "exceden maximo permitido (%d)\n", (int)(tam + tamval), 99);
    exit(1);
  }

  //open file
  FILE *f = fopen(filename, "rb+");
  //manage open error
  if (f==NULL){
    printf("Error en apertura de archivo.\n");
    perror(filename);
    exit(1);
  }


  /* I. Get amount of rows in the table */
  int seekres = fseek(f, 0, SEEK_END);
  if (seekres != 0){
    printf("Error de fseek\n");
    perror(filename);
    exit(1);
  }
  int bytesF = ftell(f);
  int rows = bytesF / 100;


  /* II. Try writing key and val at row = hash_string(key) % rows */
  int row = hash_string(key) % rows;                                            //row to add at

  //start file cycle
  for (int i = 0; i < rows; i++){
    
    int j = (row + i) % rows;                                                   //for cycle

    seekres = fseek(f, j*100, SEEK_SET);                                        //move to row
    if (seekres != 0){
      printf("Error de fseek\n");
      perror(filename);
      exit(1);
    }

    unsigned char t;
    int readres = fread(&t, 1, 1, f);
    if (readres != 1){                  //manage fread error
      perror(filename);
      exit(1);
    }

    //empty row found
    if (t == 0) {
      char fila[100];
      fila[0] = (unsigned char)tam;
      for (size_t k = 0; k < tam; k++)       // key
        fila[1 + k] = key[k];
      for (size_t k = 0; k < tamval; k++)    // definition
        fila[1 + tam + k] = val[k];
      for (size_t k = 1 + tam + tamval; k < 100; k++)  // pad with spaces
        fila[k] = ' ';

      if (fseek(f, j * 100, SEEK_SET) != 0 ||
          fwrite(fila, 100, 1, f) != 1 ||
          fclose(f) != 0) {
        perror(filename);
        exit(1);
      }
      return 0;
    }

    //found same size: check key
    if (t == tam) {
      char keyf[100];
      
      readres = fread(keyf, 1, tam, f);
      if (readres != tam){              //read error
        perror(filename);
        exit(1);
      }
      keyf[tam] = 0;

      if (strcmp(key, keyf) == 0) {     //key coincidence
        fprintf(stderr, "La llave %s ya se encuentra en el diccionario\n", key);
        fclose(f);
        exit(1);
      }
    
  }// otherwise: the next iteration seeks to the next row
  fclose(f);
  fprintf(stderr, "%s: el diccionario esta lleno\n", filename);
  exit(1);
  }
}
