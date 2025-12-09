#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/wait.h>
#include <unistd.h>

#include "tp1.h"    // Contient File2TabCom


int main(int argc, char **argv) {

    // Vérification des arguments
    if (argc < 3) {
        printf("Usage: %s <fichier_commandes> <limite_processus>\n", argv[0]);
        exit(1);
    }

    int n; // Nombre total de commandes dans le fichier
    ENRCOMM *t = File2TabCom(argv[1], &n);

    // 1. Correction Syntaxe : Conversion chaîne -> entier
    int p = atoi(argv[2]); 
    if (p <= 0) {
        fprintf(stderr, "Erreur: La limite de processus doit être > 0\n");
        exit(1);
    }

    time_t global_start, global_end;
    time(&global_start); // Top chrono global

    int k = 0;      // Index de la prochaine commande à lancer
    int actifs = 0; // Nombre de processus actuellement en cours

    printf("--- PHASE 1 : Remplissage initial (Max %d) ---\n", p);
    
    // On lance les P premiers processus (ou moins si n < p)
    // Correction Sécurité : "&& k < n" pour ne pas dépasser le tableau
    while (actifs < p && k < n) {
        time(&t[k].debut);
        t[k].statut = 1;
        printf("Lancement commande %d : %s\n", k, t[k].argv[0]);
        t[k].pid = ExecuteBatch(t[k].argv);
        
        k++;
        actifs++;
    }

    printf("--- PHASE 2 : Roulement (Fenêtre glissante) ---\n");

    // Tant qu'il reste des commandes à lancer dans le fichier
    while (k < n) {
        int statut;
        // 1. On attend qu'UNE place se libère
        int pid_mort = wait(&statut);

        // 2. On identifie qui a libéré la place
        for (int j = 0; j < n; j++) {
            if (t[j].pid == pid_mort) {
                time(&t[j].fin);
                t[j].statut = 0;
                
                if (WIFEXITED(statut)) t[j].retour = WEXITSTATUS(statut);
                else t[j].retour = -1;

                // Correction Logique : Utilisation de l'index 'j' (le trouvé)
                printf("[LIVE] Fini: %s (PID %d). Reste a lancer: %d\n", 
                       t[j].argv[0], t[j].pid, n - k);
                break;
            }
        }

        // 3. La place est libre, on lance IMMÉDIATEMENT le suivant (k)
        time(&t[k].debut);
        t[k].statut = 1;
        printf("Lancement commande %d : %s\n", k, t[k].argv[0]);
        t[k].pid = ExecuteBatch(t[k].argv);
        
        k++;
        // Note: 'actifs' reste stable ici (un sort, un rentre)
    }

    printf("--- PHASE 3 : Vidange (Attente des derniers) ---\n");

    // Il n'y a plus rien à lancer, mais il reste 'actifs' processus dehors
    while (actifs > 0) {
        int statut;
        int pid_mort = wait(&statut);
        
        // Même logique de recherche
        for (int j = 0; j < n; j++) {
            if (t[j].pid == pid_mort) {
                time(&t[j].fin);
                t[j].statut = 0;

                if (WIFEXITED(statut)) t[j].retour = WEXITSTATUS(statut);
                else t[j].retour = -1;

                printf("[LIVE] Fini: %s (PID %d). En cours: %d\n", 
                       t[j].argv[0], t[j].pid, actifs - 1);
                break;
            }
        }
        actifs--; // Un de moins sur le parking
    }

    printf("FIN\n");
    time(&global_end);

    // --- RAPPORT FINAL ---
    int cumul_cpu = 0;
    
    printf("\n--- RAPPORT COMPLET ---\n");
    printf("CMD\tPID\tRET\tDEBUT\tFIN\tDUREE\n");
    
    for (int i = 0; i < n; i++) {
        int duree = (int)(t[i].fin - t[i].debut);
        cumul_cpu += duree;

        printf("%s\t%d\t%d\t%ld\t%ld\t%d\n", 
            t[i].argv[0],    
            t[i].pid,        
            t[i].retour,     
            t[i].debut,      
            t[i].fin,        
            duree              
        );
    }

    printf("-----------------------------------\n");
    printf("Temps total reel (Mur) : %ld s\n", global_end - global_start);
    printf("Temps total CPU cumule : %d s\n", cumul_cpu);

    return 0;
}