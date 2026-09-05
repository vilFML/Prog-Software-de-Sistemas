#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "desescapar.h"


/* fn reemplaza cada secuencia de escape por el byte que representa
 *  por ej \n -> 0x0A
 */
void desescapar(char *s) {

  char *lec = s;                        //ptero solo lectura
  char *esc = s;                        //pt escritura
  
  while (*lec != '\0'){                     //mientras no sea el fin del str

    if (*lec != '\\'){                  //no backslash
      //avanzar a sig caracter
      *esc = *lec;
      esc++;
      lec++;
    }
  
    else{                               //caso backslash
      char *sig = lec;
      sig++;                            //ptero a sig caracter
      
      //casos segun sig
      if (*sig == '\0'){                //secuencia incompleta
        lec++;                            //saltar
      }

      //casos reemplazo
      else if (*sig == 't'){
        *esc = 0x09;
        esc++;
        lec += 2;
      }
      else if (*sig == 'n'){
        *esc = 0x0A
        esc++;
        lec += 2;
      }
      else if (*sig == '\\'){
        *esc = 0x5C;
        esc++;
        lec += 2;
      }
      else if (*sig == '"'){
        *esc = 0x22;
        esc++;
        lec += 2;
      }
      else if (*sig == 'x'){            //caso xhh
        //TODO


        sig++;                          //ver h1
        unsigned char h1 = *sig;
        sig++;
        unsigned char h2 = *sig;        //ver h2

        unsigned char resultado = (h1 << 4) | h2;                               //unir ambos

        *lec = resultado;
      }    
    }
    lec++;
  }
  *lec = '\0'
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
