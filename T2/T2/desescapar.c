#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "desescapar.h"


/* fn reemplaza cada secuencia de escape por el byte que representa
 *  por ej \n -> 0x0A
 */
void desescapar(char *s) {
  /* Idea:
    crear ptero aux
    Mientras s no apunte a 0:      
      si apunta a '\'
      aux apunta a sig
        si aux apunta a n:
          cambiar apuntado por s = 0x0A
        si auz apunta a t:
          cambiar apuntado por s = 0x09
        si aux apunta a \
          cambiar pauntado pos s = 0x5C
        si aux apunta a "
          cambiar apuntado por s = 0x22
        si aux apunta a x:
          pasar sig char a bytes
          pasar subsig char a byte
          combinar
          ingresar en casilla apuntada por s
      si no es '\':
        avanzar s a sig caracter
    
    fuera de while: agregar 0x00 en s

    retornar
    */
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
