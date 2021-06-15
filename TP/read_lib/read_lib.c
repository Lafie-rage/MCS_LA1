/*******************************************************
Nom ......... : read_lib.c
Role ........ : Permet de lire une chaine de caractére
Auteur ...... : Saint-Omer

********************************************************/
#include "read_lib.h"

/*******************************************************
Nom ......... : viderBuffer
Role ........ : vider le buffer
Arg ......... : 
Return....... : VOID
********************************************************/
void viderBuffer()
{
    int c = 0;
    while (c != '\n' && c != EOF)
    {
        c = getchar();
    }
}

/*******************************************************
Nom ......... : lire
Role ........ : lire une saisie utilisateur avec une limite de taille
Arg ......... : *chaine  : pointeur vers la chaine à remplir
		        longueur : longeur de la chaîne
Return....... : int
********************************************************/
int lire(char *chaine, int longueur)
{
    char *positionEntree = NULL;
    if (fgets(chaine, longueur, stdin) != NULL){
        positionEntree = strchr(chaine, '\n');
        if (positionEntree != NULL){
            *positionEntree = '\0';
        }
        else{
            viderBuffer();
        }
        return 1;
    }
    else{
        viderBuffer();
        return 0;
    }
}
