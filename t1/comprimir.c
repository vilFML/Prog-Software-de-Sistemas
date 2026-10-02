/* Idea:
Tener cuantos nros caben en uint de destino.
Para cada nro en arreglo:
  Extraer bits requeridos
  Unirlos en uint de destin
  pasar a siguientes bits de destino
se repite #veces = cant de nros que quepan
  */


#include "comprimir.h"

uint comprimir(uint a[], int nbits) { 
  int cap_uint = sizeof(uint) << 3;                                             //obtener capacidad de destino
  
  //tener cuantos caben: contar cuantas veces cabe nbits en cap_uint
  int k = 0;                                                                    //guardar nros que caben
  int bits_a_reservar = nbits;                                                  //aux: ir contando bits usados
  while(bits_a_reservar <= cap_uint){
    k++;                                                                        //cabe 1 mas
    bits_a_reservar += nbits;
  }

  //Ingresar los k numeros extraidos en el uint
  uint destino = 0;                                                             //uint final
  uint mascara = ( (unsigned int)1 << (nbits-1) << 1) - 1;
  uint extraido = 0;

  //para cada nro en arreglo
  for (int i=0; i<k; i++){                                                      //i lleva la cuenta de nros ingresados
  
    extraido = a[i] & mascara;                                                  //extraer nbits de a[i] con mascara
    destino = destino << (nbits-1) << 1;                                        //unir bits en pos segun ciclo
    destino = destino | extraido;
  
  }
  return destino;
}

