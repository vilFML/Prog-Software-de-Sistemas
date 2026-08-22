/* Idea:
Tener cuantos nros caben en uint de destino.
Para cada nro en arreglo:
  Extraer bits requeridos
  Unirlos en uint de destin
  pasar a siguientes bits de destino
se repite #veces = cant de nros que quepan
  */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "comprimir.h"

uint comprimir(uint a[], int nbits) { 
  /*  I.  Obtener capacidad en uint destino
  */  
  int cap_uint = sizeof(uint) << 3;                                             //obtener capacidad de destino

  //tener cuantos caben: contar cuantas veces cabe nbits en cap_uint
  int k = 0;                                                                    //cantidad de nros que caben
  int bits_aReservar = nbits;                                                   //para ir contando bits usados del uint
    
  while(bits_aReservar <= cap_uint){
    k++;                                                                        //cabe 1 mas
    bits_aReservar += nbits;
  }
  printf("Caben %i numeros\n", k);


  /* II.  Ingresar los k numeros extraidos en el uint
   */
  uint res = 0;                                                                 //uint final
  
  int bits_extrs = 0;                                                           //para almacenar extraidos
  int masc = ~(1<<nbits);                                                       //crear mascara segun cant de bits a extraer

  for (int i=0; i<k; i++){                                                      //i lleva la cuenta de nros ingresados
    
    bits_extrs = a[i] & masc;                                                   //extraer nbits de a[i] con mascara
    res = res | (bits_extrs << (i*nbits) );                                     //unir bits en pos segun ciclo
  }

  printf("TEST: resultado: %u\n", res);
  return res;
}
