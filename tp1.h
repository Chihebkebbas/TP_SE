#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_LIGNES 100
#define MAX_ARGV 100


typedef struct enrc
{
int pid;
int statut;
int retour;
time_t debut;
time_t fin;
char ** argv;
} ENRCOMM;



// Exercice 01
char **Ligne2Argv(char *ligne) {

    char **argv = (char**)malloc(MAX_ARGV * sizeof(char *));
    char *li = strdup(ligne);

    int i = 0;

    char *l = strtok(li, " ");

    while (l) {

        argv[i] = l;
        l = strtok(NULL, " ");
        i++;
    }

    return argv;
}

// Exercice 02
void AfficheArgv(char** argv) {
    int i = 0;

    while(argv[i]) {
        printf("Arg %d : %s\n", i, argv[i]);
        i++;
    }
    printf("\n");

}

// Exercice 03
char *Argv2Ligne(char** argv) {
    int i = 0;
    char *s = (char*)malloc(sizeof(char*));

    while(argv[i+1]) {
        strcat(s, strdup(argv[i]));
        strcat(s, " ");
        i++;
    }

    strcat(s, strdup(argv[i]));

    return s;
}


// Exercice 04
int Executer(char **argv, int *p) {

    int pid = fork();

    if (pid == -1)
    {
        return 255;

    } else if (pid == 0) {
       
        execvp(argv[0], argv);
        perror("Commande n'est pas trouvé !");

        exit(254); 

    } else {
   
        int status;
        int res_wait;
                
        res_wait = wait(&status); 

        if (res_wait == -1) {
            
            return 255; 
        }

        *p = pid;

        if (WIFEXITED(status)) {
            
            return WEXITSTATUS(status);
        }
        
        return 1;
        
    } 
}


// Exercie 06
char ***File2TabArgv(char *file, int *nbrCommande) {

    char ***result = (char ***)malloc(MAX_LIGNES * sizeof(char **));

    
    /*
        "r" = lecture
        "w" = écriture (écrase)
        "a" = ajout (append)
    */
    FILE *f = fopen(file, "r");
    if (!f) {
        free(result);
        perror("Fichier n'existe pas !");
        return NULL;
    }
    char ligne[250];
    int len;
    int i = 0;

    char **argv;

    while(fgets(ligne, sizeof(ligne), f)) {
        len = strlen(ligne);
        if (ligne[len-1] == '\n') {
            ligne[len-1] = '\0';
        }

        argv = Ligne2Argv(ligne);
        result[i] = argv;
        i++;
    }

    *nbrCommande = i;

    fclose(f);
    
    return result;
    
}


int ExecuteBatch(char **argv) {
    int pid = fork();

    if (pid == -1) {
        // Échec du fork (problème système)
        perror("ExecuteBatch: fork failed");
        return -1;
    }

    if (pid == 0) {
        execvp(argv[0], argv);

        // Si execvp retourne, c'est une erreur (commande non trouvée)
        perror("ExecuteBatch: command not found");
        // Le Fils doit signaler l'échec et mourir
        exit(127); // Code d'erreur standard pour "command not found"
    }

    return pid;
}

// Exercie 10

ENRCOMM * File2TabCom(char *file, int *nbrCommande) {

    ENRCOMM *result = (ENRCOMM *)malloc(MAX_LIGNES * sizeof(ENRCOMM));

    
    /*
        "r" = lecture
        "w" = écriture (écrase)
        "a" = ajout (append)
    */
    FILE *f = fopen(file, "r");
    if (!f) {
        free(result);
        perror("Fichier n'existe pas !");
        exit(0);
    }
    char ligne[250];
    int len;
    int i = 0;

    char **argv;

    while(fgets(ligne, sizeof(ligne), f)) {
        len = strlen(ligne);
        if (ligne[len-1] == '\n') {
            ligne[len-1] = '\0';
        }

        // Ignorer les lignes vides
        if (ligne[0] == '\0') {
            continue;
        }

        argv = Ligne2Argv(ligne);
        result[i].argv = argv;
        result[i].pid = 0;
        result[i].statut = -1;
        result[i].retour = 0;
        result[i].debut = 0;
        result[i].fin = 0;

        i++;
    }

    *nbrCommande = i;

    fclose(f);
    
    return result;
    
}

