#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

void handle_returned_status(pid_t p, int status) {
    if (p == -1) {
        perror("waitpid");
        exit(EXIT_FAILURE);
    } else {
        printf("pid=%d \t", p);
        if (WIFEXITED(status))
            printf("Terminaison normale avec le code de sortie=%d\n", WEXITSTATUS(status));
        if (WIFSIGNALED(status)) {
            printf("Arrêt par le signal=%d%s\n", WTERMSIG(status), WCOREDUMP(status) ? " (image mémoire crée)" : "");
            exit(EXIT_FAILURE);
        }
        if (WIFSTOPPED(status)) {
            printf("Processus stoppé par le signal=%d\n", WSTOPSIG(status));
            exit(EXIT_FAILURE);
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("utilisation: %s <programme.c>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    // process père
    int err, status;
    // nom du programme
    char *nom_programme = argv[1];
    size_t nom_programme_len = strlen(nom_programme);
    if (nom_programme_len < 3) {
        printf("Le nom du programme doit avoir au moins 3 caractères\n");
        exit(EXIT_FAILURE);
    }
    // nom du module objet
    char nom_module_objet[nom_programme_len + 1];
    memset(nom_module_objet, 0, nom_programme_len + 1);
    strncpy(nom_module_objet, nom_programme, nom_programme_len - 2);
    strcat(nom_module_objet, ".o");
    // nom d'exécutable
    char nom_executable[nom_programme_len - 0];
    memset(nom_executable, 0, nom_programme_len);
    strncpy(nom_executable, nom_programme, nom_programme_len - 2);
    // affichage
    printf("-----------------------------------------------------------------\n");
    printf("compilation du programme %s\n", nom_programme);
    printf("nom de l'exécutable à créer: %s\n", nom_executable);
    printf("nom du module objet à créer: %s\n", nom_module_objet);
    pid_t p = fork(); /* 1 */
    if (p == 0) {
        // process P1
        err = execlp("gcc", "gcc", "-c", nom_programme, "-o", nom_module_objet, NULL);
        if (err == -1) {
            printf("\nerreur dans la compilation\n");
            exit(EXIT_FAILURE);
        }
    } else if (p > 0) {
        p = waitpid(p, &status, 0);
        printf("-----------------------------------------------------------------\n");
        printf("status du P1:\n");
        handle_returned_status(p, status);
        p = fork(); /* 2 */
        if (p == 0) {
            // process P2
            err = execlp("gcc", "gcc", nom_module_objet, "-o", nom_executable, NULL);
            if (err == -1) {
                printf("\nerreur dans l'edition de lien\n");
                exit(EXIT_FAILURE);
            }
        } else if (p > 0) {
            p = waitpid(p, &status, 0);
            printf("-----------------------------------------------------------------\n");
            printf("status du P2:\n");
            handle_returned_status(p, status);
            p = fork(); /* 3 */
            if (p == 0) {
                // process P3
                char executer_programme[nom_programme_len + 1];
                memset(executer_programme, 0, nom_programme_len + 1);
                strncpy(executer_programme, "./", 2);
                strcat(executer_programme, nom_executable);
                printf("-----------------------------------------------------------------\n");
                printf("execution du programme %s\n", nom_programme);
                printf("-----------------------------------------------------------------\n");
                err = execlp(executer_programme, nom_executable, NULL);
                if (err == -1) {
                    printf("\n erreur dans l'execution du programme\n");
                    exit(EXIT_FAILURE);
                }
            } else if (p > 0) {
                p = waitpid(p, &status, 0);
                printf("-----------------------------------------------------------------\n");
                printf("status du programme quand a executer %s :\n\n", nom_programme);
                handle_returned_status(p, status);
                printf("-----------------------------------------------------------------\n");
            } else {
                perror("fork");
                exit(EXIT_FAILURE);
            }
        } else {
            perror("fork");
            exit(EXIT_FAILURE);
        }
    } else {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    return 0;
}