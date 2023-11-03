#include <stdio.h>
#include <stdlib.h>


int main() {
    //1// verifier le cas ou l'argument est superieure a 2

    //2// test un probleme de compilation
    // printf("Hello World\n")
    // enter un nom de fichier qui n'existe pas pour avoir un probleme de compilation aussi par example enter "./main progggggg.c" au lieu de "./main prog.c"

    //3//Execution avec succès:
    printf("Hello World\n");
    return 0;

    //4// Execution avec erreur:
    //printf("Hello World\n");
    //return 1;

    //5// test un cas de deroutement , les erreurs de segmentation (image mémoire crée):
/*
    printf("Hello World\n");
    int arr[2];
    arr[3] = 1; // segmentation fault
*/
/*
    int x = 0;
    int y = 1;
    int result = y / x;
    */
    //1 / 0;

    //6// test stopped par un signal;
    // while (1) {
    //sleep(10);
    /*
                if (status != 0) {
                    printf("-----------------------------------------------------------------\n");
                    printf("execution du programme %s avec erreur\n", nom_programme);
                    printf("------------
     * la boucle Infinie pour que le programme ne se termine pas;
     * executer la commande ps -ef | grep prog pour trouver le numéro du processus;
     * executer la commande kill -1 PID (PID est le numéro du processus) pour stopper le processus;
     * ne pas utiliser CTRL+Z pour stopper le processus ,cet instruction va mettre fin au processus ./main prog , et non pas au processus prog;
    */
    //}
}