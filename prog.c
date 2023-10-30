#include <stdio.h>
#include <stdlib.h>


int main() {
    //test un probleme de compilation
    //printf("Hello World\n")

    //Execution avec succès:
    printf("Hello World\n");
    return 0;

    //Execution avec erreur:
    //printf("Hello World\n");
    //return 1;

    //test un cas de deroutement , les erreurs de segmentation (image mémoire crée):
    /*
    printf("Hello World\n");
    int arr[2];
    arr[3] = 1; // segmentation fault
     */

    //test stopped par un signal;
    //while (1) {
    //sleep(10);
    /*
     * la boucle Infinie pour que le programme ne se termine pas;
     * executer la commande ps -ef | grep prog pour trouver le numéro du processus;
     * executer la commande kill -1 PID (PID est le numéro du processus) pour stopper le processus;
     * ne pas utiliser CTRL+Z pour stopper le processus ,cet instruction va mettre fin au processus ./main prog , et non pas au processus prog;
    */
    //}
}