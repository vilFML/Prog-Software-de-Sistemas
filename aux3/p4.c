/* 
Idea 1:
Considerando que en ASCII hay 256 caracteres, se hace un arreglo con 256 espacios.
Luego, se recorre el string y por cada caracter que aparece, sumarle 1 al espacio respectivo en el arreglo.
Finalmente se retorna el valor ASCII de la posicion con mayor cantidad de apariciones. 
*/


char mas_repetido(char *s){
    int chars[256] = {0};                                                       //arreglo de posiciones de caracteres llenar con 0

    //procesar string
    while(*s!=0){
        chars[(int)*s]++;
        s++;                            //ptero a pos siguiente
    }

    //encontrar pos de mayor ocurrencia
    int max = 0;
    for (int i=0; i < 256; i++){
        if (chars[i] > max){            //valor en pos es mayor a max: reemplazar
            max = chars[i];
        }
    }
    return (char)max;
}
