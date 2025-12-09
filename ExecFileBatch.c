#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "tp1.h"


int main(int argc, char **argv) {

    time_t temps_total;
    time(&temps_total);


    int n,p;
    ENRCOMM *t = File2TabCom(argv[1], &n);
    
    for (int i = 0; i < n; i++)
    {
        time(&t[i].debut);
        t[i].statut = 1;
        t[i].pid = ExecuteBatch(t[i].argv);

    }

    for (int i = 0; i < n; i++) {
        int statut;
        int pid_mort = wait(&statut);

        for (int j = 0; j < n; j++)
        {
            if (pid_mort == t[j].pid)
            {
                time(&t[j].fin);
                t[j].statut = 0;

                if (WIFEXITED(statut)) t[j].retour = WEXITSTATUS(statut);
                else t[j].retour = -1;

                // AFFICHAGE LIVE 
                printf("%s: %d %d %ld %ld %ld\n", 
                    t[i].argv[0],    // Nom commande
                    t[i].pid,        // PID 
                    t[i].retour,     // Exit Status
                    t[i].debut,      // Epoch début
                    t[i].fin,        // Epoch fin
                    t[i].fin - t[i].debut            // Durée calculée
                );
                
                break;
            }
        }
    }

    printf("FIN\n");

    time_t now;
    time(&now);
    temps_total = now - temps_total;
    
    
    // Rapport d'exécution (Demandé par Ex 11)
    // Format : Commande : PID Retour Debut Fin Durée
    
    for (int i = 0; i < n; i++) {
        int duree = (int)(t[i].fin - t[i].debut);

        printf("%s: %d %d %ld %ld %d\n", 
            t[i].argv[0],    // Nom commande
            t[i].pid,        // PID 
            t[i].retour,     // Exit Status
            t[i].debut,      // Epoch début
            t[i].fin,        // Epoch fin
            duree              // Durée calculée
        );
    }

    printf("temps total: %ld\n", temps_total);

    return 0;

}