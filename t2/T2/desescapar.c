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
void desescapar(char *s){

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
          int h1 = char_a_int(*h1p);    //analizar 1er char
          
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
            else{
              int h2 = char_a_int(*h2p);
              if(h2 == -1){             //segundo invalido
                lec++;
              }
              else{ //ambos validos: unir
                unsigned char resultado = (unsigned char)((h1 << 4) | h2);
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
  *esc = '\0';
  return;
}

/* fn hace lo mismo pero crea string nuevo */
char *desescapado(const char *s){
  /* I. Obtener espacio */
  int mem = 0;

  //recorrer string
  const char *lec = s;
  while (*lec != '\0'){

    //no backslash
    if (*lec != '\\'){
      mem++;
      lec++;
    }
    else{ //backslash: casos de sig caract
      
      const char *aux = lec;
      aux++;

      if (*aux == '\0'){                //fin de string
        lec++;
      }
      else{ //especiales

        if(*aux == 't' || *aux == 'n' || *aux == '\\' || *aux == '"'){
          mem++;                        //sumar 1 para char
          lec += 2;                     //saltar ambos
        }
        else if (*aux == 'x'){          //caso xhh
          const char *h1p = aux;
          h1p++;
          
            //se tienen casos para chars
          if (*h1p == '\0'){            //fin str
            lec++;
          }
          else{
            int h1 = char_a_int(*h1p);  //analizar 1er char

            if (h1 == -1){              //hex invalido
              lec++;                    //saltar backslash
            }
            //hex valido: ver segundo
            else{
              const char *h2p = h1p;
              h2p++;
              if(*h2p == '\0'){         //fin str
                lec++;
              }
              else{
                int h2 = char_a_int(*h2p);
                if(h2 == -1){           //segundo invalido
                  lec++;
                }
                else{ //ambos validos, se unen en 1
                  mem++;
                  lec+=4;               //saltar secuencia
                }
              }
            }
          }
        }
        else{//cualquier otro
          mem++;
          lec += 2;
        }
      }
    }
  }//fin while

  /* II. Pedir mem */
  char *res = malloc(mem + 1);
  
  if (res == NULL){                     //caso borde: str vacio
    return NULL;
  }
  

  /* III. Agregar caracters */
  lec = s;                  //reiniciar lectura desde el inicio
  char *esc = res;                      //puntero de escritura sobre res

  while (*lec != '\0'){
    if (*lec != '\\'){                  //no backslash
      *esc = *lec;
      esc++;
      lec++;
    }
    else{                               //caso backslash
      const char *sig = lec;
      sig++;                            //ptero a sig caracter

      if (*sig == '\0'){                //secuencia incompleta
        lec++;                          //saltar
      }

      else if (*sig == 't'){
        *esc = 0x09;
        esc++;
        lec += 2;
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
      else if (*sig == 'x') {           //caso xhh
        const char *h1p = sig;
        h1p++;

        if (*h1p == '\0') {            //fin str
          lec++;
        }
        else {
          int h1 = char_a_int(*h1p);

          if (h1 == -1) {               //hex invalido
            lec++;
          }
          else {
            const char *h2p = h1p;
            h2p++;
            if (*h2p == '\0') {         //fin str
              lec++;
            }
            else {
              int h2 = char_a_int(*h2p);
              if (h2 == -1) {           //segundo invalido
                lec++;
              }
              else {  //ambos validos: unir
                unsigned char resultado =
                (unsigned char)((h1 << 4) | h2);
                *esc = (char)resultado;
                esc++;
                lec += 4;
              }
            }
          }
        }
      }
      else {                            //fuera de tabla: saltar backslash
        *esc = *sig;                    //el char se conserva
        esc++;
        lec += 2;
      }
    }
  }//fin while

  *esc = '\0';

  return res;
}
