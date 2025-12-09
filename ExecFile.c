#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "tp1.h" 




int main(int argc, char **argv) {

    int n;
    time_t temps_total;
    time(&temps_total);


    ENRCOMM *tab = File2TabCom(argv[1], &n);
    for (int i = 0; i < n; i++)
    {
        
        time(&tab[i].debut);
        tab[i].statut = 1;
 
        tab[i].retour = Executer(tab[i].argv, &tab[i].pid);
        
        time(&tab[i].fin);
        tab[i].statut = 0;
    }
    
    // Rapport d'exécution (Demandé par Ex 11)
    // Format : Commande : PID Retour Debut Fin Durée
    time_t now;
    time(&now);
    temps_total = now - temps_total;
    
    for (int i = 0; i < n; i++) {
        int duree = (int)(tab[i].fin - tab[i].debut);

        printf("%s: %d %d %ld %ld %d\n", 
            tab[i].argv[0],    // Nom commande
            tab[i].pid,        // PID 
            tab[i].retour,     // Exit Status
            tab[i].debut,      // Epoch début
            tab[i].fin,        // Epoch fin
            duree              // Durée calculée
        );
    }

    printf("temps total: %ld\n", temps_total);

    return 0;
    
}