#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "desescapar.h"

/* auxiliar, devuelve val numérico de char */
int char_a_int(char c){
  if (c >= '0' && c <= '9'){            //caso num
    return c - '0';                     //distancia al 0
  }

  //casos letras
  else if (c >= 'A' && c <= 'F'){
    return (c - 'A') + 10;              //distancia a 'A' mas nros
  }
  else if (c >= 'a' && c <= 'f'){       //caso minuscula
    return (c - 'a') + 10;
  }

  else{//hex invalido
    return -1;
  }
}

/* fn reemplaza cada secuencia de escape por el byte que representa
 *  por ej \n -> 0x0A
 */
void desescapar(char *s) {

  char *lec = s;                        //ptero solo lectura
  char *esc = s;                        //pt escritura
  
  while (*lec != '\0'){                 //mientras no sea el fin del str

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
        *esc = 0x09;                    //reemplazar x secuencia
        esc++;                          //pasar a sig casilla
        lec += 2;                       //saltar secuencia
      }
      else if (*sig == 'n'){
        *esc = 0x0A;
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
        char *h1p = sig;
        h1p++;
        
        //se tienen casos para chars
        if (*h1p == '\0'){              //fin str
          lec++;
        }
        else{
          int h1 = char_a_int(*sig);    //analizar 1er char
          
          //hex invalido
          if (h1 == -1){
            lec++;                      //saltar backslash
          }
          //hex valido: ver segundo
          else{
            char *h2p = h1p;
            h2p++;
            if(*h2p == '\0'){           //fin str
                lec++;
              }
            }
            else{
              int h2 = char_a_int(*h2p);
              if(h2 == -1){             //segundo invalido
                lec++;
              }
              else{ //ambos validos: unir
                unsigned char resultado = [(unsigned char)((h1 << 4) | h2)];
                *esc = (char)resultado;

                esc++;
                lec+=4;                 //saltar a sigs
              }
            }
          }
        }
      }
      else{                             //fuera de tabla: saltar backslash
        lec++;
      }
    }
  }
  *lec = '\0';
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
