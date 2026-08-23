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
  int cap_uint = sizeof(uint) << 3;                                             //obtener capacidad de destino

  if (nbits >= cap_uint){               //si nbits excede cap: solo truncar

    int mask = (unsigned int)-1 >> (32-nbits);
    return (a[0] & mask);
  }
  
  else{
    //tener cuantos caben: contar cuantas veces cabe nbits en cap_uint
    int k = 0;                                                                  //guardar nros que caben
    int bits_aReservar = nbits;                                                 //aux: ir contando bits usados
      
    while(bits_aReservar <= cap_uint){
      k++;                                                                      //cabe 1 mas
      bits_aReservar += nbits;
    }

    /* Ingresar los k numeros extraidos en el uint */
    uint res = 0;                                                               //uint final
    
    uint bits_extrs = 0;                                                        //para almacenar extraidos
    int mask = (unsigned int)-1 >> (32-nbits);                                  //crear mascara

    //para cada nro en arreglo
    for (int i=0; i<k; i++){                                                    //i lleva la cuenta de nros ingresados
      
      bits_extrs = a[i] & mask;                                                 //extraer nbits de a[i] con mascara
      res = (res << nbits) | bits_extrs;                                        //unir bits en pos segun #ciclo
    }
    return res;
  }
}
