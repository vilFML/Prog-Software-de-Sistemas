#include <stddef.h>

#include "prev.h"

//Asigna campos prev y prox 
void asignarPrev(Nodo *t, Nodo **pprev) {
  if(t == NULL){
    return;                             //CB nodo vacio
  }

  //subarbol izq
  asignarPrev(t->izq, pprev);           //recursivo a izq

  // visitar nodo t
  t->prev = *pprev;                     // nodo previo
  t->prox = NULL;                       // por ahora es null

  if (*pprev != NULL){                  // si previo no es null
    (*pprev)->prox = t;                 //  t es el prox nodo del nodo previo a t
  }

  *pprev = t;                           // asignar t a *pprev

  //sub arbol der
  asignarPrev(t->der, pprev);
}
