#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tp1.h" 

int main() {
    char ligne[100];
    char **commande;
    int n;

    // Déplace le prompt dans la condition de boucle et vérifie si la lecture réussit
    while (1) {
        printf("Entrer Commandes > ");

        // Capture le résultat de fgets
        if (fgets(ligne, sizeof(ligne), stdin) == NULL) {
            break; // Sort de la boucle si EOF (<Ctrl D>) est rencontré
        }

        // Vérification du contenu (optionnel, mais robuste)
        if (ligne[0] == '\n' || ligne[0] == '\0') {
            continue;
        }

        int len = strlen(ligne);

        if (ligne[len-1] == '\n') {
            ligne[len-1] = '\0';
        }

        // Ligne2Argv DOIT s'assurer que le '\n' est remplacé par '\0'
        commande = Ligne2Argv(ligne); 
        
        // Exécution (en supposant que Executer est la fonction validée précédemment)
        n = Executer(commande); 
        
        // Optionnel : Libérer la mémoire allouée par Ligne2Argv
        free(commande); 
    }

    printf("FIN\n");
    return 0;
}