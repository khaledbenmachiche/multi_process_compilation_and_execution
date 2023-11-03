#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>


int code_retour_tests(pid_t p, int status) {
    if (p == -1) {
        printf("Erreur de wait\n");
        perror("waitpid");
        exit(EXIT_FAILURE);
    } else {
        printf("pid=%d \t", p);
        if (WIFEXITED(status)) {
            printf("Terminaison normale avec le code de sortie=%d\n", WEXITSTATUS(status));
            return WEXITSTATUS(status);
        }
        if (WIFSIGNALED(status)) {
            printf("Arrêt par le signal=%d%s\n", WTERMSIG(status), WCOREDUMP(status) ? " (image mémoire crée)" : "");
            exit(2);
        }
        if (WIFSTOPPED(status)) {
            printf("Processus stoppé par le signal=%d\n", WSTOPSIG(status));
            exit(3);
        }
    }
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc > 2) {
        printf("utilisation: %s <programme.c>\n", argv[0]);
        return 1;
    }
    // process père
    int err, status;
    // nom du programme
    char *nom_programme = argv[1];
    size_t nom_programme_len = strlen(nom_programme);
    if (nom_programme_len < 3) {
        printf("Le nom du programme doit avoir au moins 3 caractères\n");
        return 1;
    }
    if (nom_programme[nom_programme_len - 1] != 'c' || nom_programme[nom_programme_len - 2] != '.') {
        printf("Le nom du programme doit se terminer par .c\n");
        return 1;
    }
    // nom du module objet
    char nom_module_objet[nom_programme_len + 1];
    memset(nom_module_objet, 0, nom_programme_len + 1);
    strncpy(nom_module_objet, nom_programme, nom_programme_len - 2);
    strcat(nom_module_objet, ".o");

    // nom d'exécutable
    char nom_executable[nom_programme_len - 1];
    memset(nom_executable, 0, nom_programme_len - 1);
    strncpy(nom_executable, nom_programme, nom_programme_len - 2);

    // affichage
    printf("-----------------------------------------------------------------\n");
    printf("compilation du programme %s\n", nom_programme);
    printf("nom de l'exécutable à créer: %s\n", nom_executable);
    printf("nom du module objet à créer: %s\n", nom_module_objet);
    printf("-----------------------------------------------------------------\n\n");

    pid_t p = fork(); /* 1 */
    if (p == 0) {
        // process P1
        err = execlp("gcc", "gcc", "-c", nom_programme, "-o", nom_module_objet, NULL);
        if (err == -1) {
            printf("\nexeclp de P1 non executée\n");
            perror("execlp 1");
            exit(EXIT_FAILURE);
        }
    } else if (p > 0) {
        p = waitpid(p, &status, 0);
        printf("-----------------------------------------------------------------\n");
        printf("status de P1:\n");
        status = code_retour_tests(p, status);
        if (status != 0) {
            printf("-----------------------------------------------------------------\n");
            printf("compilation du programme %s failed\n", nom_programme);
            printf("-----------------------------------------------------------------\n");
            exit(EXIT_FAILURE);
        }
        p = fork(); /* 2 */
        if (p == 0) {
            // process P2
            err = execlp("gcc", "gcc", nom_module_objet, "-o", nom_executable, NULL);
            if (err == -1) {
                printf("\nexeclp de P2 non executée\n");
                perror("execlp 2 ");
                exit(EXIT_FAILURE);
            }
        } else if (p > 0) {
            p = waitpid(p, &status, 0);
            printf("-----------------------------------------------------------------\n");
            printf("status de P2:\n");
            status = code_retour_tests(p, status);
            if (status != 0) {
                printf("-----------------------------------------------------------------\n");
                printf("edition de lien de %s failed\n", nom_module_objet);
                printf("-----------------------------------------------------------------\n");
                exit(EXIT_FAILURE);
            }
            p = fork(); /* 3 */
            if (p == 0) {
                // process P3
                char executer_programme[nom_programme_len + 1];
                memset(executer_programme, 0, nom_programme_len + 1);
                strncpy(executer_programme, "./", 2);
                strcat(executer_programme, nom_executable);
                printf("-----------------------------------------------------------------\n");
                printf("execution de programme %s\n", nom_programme);
                printf("-----------------------------------------------------------------\n");
                err = execlp(executer_programme, nom_executable, NULL);
                if (err == -1) {
                    printf("\nexeclp de P3 non executée\n");
                    perror("execlp 3 ");
                    exit(EXIT_FAILURE);
                }
            } else if (p > 0) {
                p = waitpid(p, &status, 0);
                printf("-----------------------------------------------------------------\n");
                printf("processus P3 ,status de programme a executer %s :\n", nom_programme);
                status = code_retour_tests(p, status);
                if (status != 0) {
                    printf("-----------------------------------------------------------------\n");
                    printf("execution du programme %s avec erreur\n", nom_programme);
                    printf("-----------------------------------------------------------------\n");
                    exit(EXIT_FAILURE);
                } else {
                    printf("-----------------------------------------------------------------\n");
                    printf("execution du programme %s avec success\n", nom_programme);
                    printf("-----------------------------------------------------------------\n");
                }
            } else {
                printf("erreur dans l'execution de fork 3\n");
                perror("fork 3 ");
                exit(EXIT_FAILURE);
            }
        } else {
            printf("erreur dans l'execution fork 2\n");
            perror("fork 2 ");
            exit(EXIT_FAILURE);
        }
    } else {
        printf("erreur dans l'execution fork 1\n");
        perror("fork 1 ");
        exit(EXIT_FAILURE);
    }
    return 0;
}
