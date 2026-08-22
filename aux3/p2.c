/* 
La idea es recorrer el string con dos punteros comparando los caracteres que almacenan.
Se realiaz el proceso mientras el puntero que comienza desde el final esté a la derecha del que empieza al inicio. 
Entonces la condición de while() es s_izq <= s_der
*/


int is_palindromo(char *s){
    char *pFinal = s + strlen(s) - 1;

    while(s < pFinal){
        if(*s != *pFinal){
            return 0;
        }
        s++;
        pFinal--;
    }// Si finaliza bucle es palindromo
    return 1;
}