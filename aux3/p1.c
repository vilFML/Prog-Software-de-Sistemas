//recibe string en mayus y devuelve el mismo string en minus
void to_lower(char *s){
    n = strlen(char *s);                                                        //largo de string

    for(int i=0; i<n+1; i++){
        if (*s[i]>= 'A' && *s[i]<='Z'){                                         //si caract esta en A-Z
            *s[i] += 32;                                                        //cambiarlo a minus
        }
        s++;                                                                    //puntero a sig pos
    }
    print
    return *(s-n);
}
