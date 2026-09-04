#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "desescapar.h"


/* fn reemplaza cada secuencia de escape por el byte que representa
 *  por ej \n -> 0x0A
 */
void desescapar(char *s) {

  char *cab = s;                        //ptero aux apunta al mismo 1er char
  
  while (*cab != '\0'){                     //mientras no sea el fin del str

    if (*cab != 0x5C){                  //se ve backslash
      cab++;                              //avanzar a sig caracter
    }
  
    else{                               //caso backslash
      char *sig = cab;
      sig++;                            //ptero a sig caracter
      
      //casos segun sig
      if (*sig == 't'){
        *cab = 0x09;                    //backslash inicial -> char
      }
      else if (*sig == 'n'){
        *cab = 0x0A;
      }
      else if (*sig == 0x5C){
        *cab = 0x5C;
      }
      else if (*sig == 0x22){
        *cab = 0x22;
      }
      else if (*sig == 'x'){            //caso xhh
        sig++;                          //ver h1
        unsigned char h1 = *sig;
        sig++;
        unsigned char h2 = *sig;        //ver h2

        unsigned char resultado = (h1 << 4) | h2;                               //unir ambos

        *cab = resultado;
      }    
    }
    cab++;
  }
  *cab = '\0'
  return;
}

/* 
 *  recibe secuencia de bytes y retorna string que representan
 */
char *desescapado(const char *s) {
  /* Idea: se tiene mezcla de headecimal y otros
  Crear string de retorno
  Mientras se este en s
    si caracter es 0: se tiene hexadecimal
      copiar caracteres a retorno
    si caracter no es 0:
      traducir a byte
      agregar byte a string retorno
  devolver string de retorno
  
    */
}
