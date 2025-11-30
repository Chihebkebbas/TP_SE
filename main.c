#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_ARGV 100


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
int Executer(char **argv) {

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

        if (WIFEXITED(status)) {
            
            return WEXITSTATUS(status);
        }
        
        return 1;
        
    } 
}



int main() {

    char *s = "ls -l";

    char **argv = Ligne2Argv(s);

    AfficheArgv(argv);
    
    char *m = Argv2Ligne(argv);
    printf("%s\n", m);

    printf("%d\n", Executer(argv));
    return 0;
}