#include <signal.h>    // Inclure la bibliothèque pour les fonctions de signal
#include <stdio.h>     // Inclure la bibliothèque standard d'entrée/sortie
#include <stdlib.h>    // Inclure la bibliothèque standard pour des fonctions comme atoi
#include <string.h>    // Inclure pour des fonctions liées aux chaînes de caractères
#include <unistd.h>    // Inclure pour usleep
#include <sys/types.h> // Inclure pour des définitions de types utilisés dans syscalls

int main(int argc, char *argv[]) {
    // Vérifier le nombre d'arguments passés au programme
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <Server PID> <Message>\n", argv[0]);
        return 1;
    }

    // Convertir l'argument PID en nombre (Process ID)
    pid_t server_pid = atoi(argv[1]);
    char *message = argv[2]; // Stocker le message à envoyer
    union sigval value;      // Union pour stocker des données à envoyer avec le signal

    // Envoi du caractère spécial '\n' pour indiquer le début d'un nouveau message
    value.sival_int = '\n';  // Stocker '\n' dans value
    sigqueue(server_pid, SIGUSR1, value); // Envoyer le signal avec la valeur
    usleep(500); // Petite pause pour permettre au serveur de traiter le signal

    // Boucle pour envoyer le message caractère par caractère
    for (int i = 0; i <= strlen(message); ++i) {
        // Condition pour envoyer soit un caractère du message soit le caractère nul à la fin
        value.sival_int = (i < strlen(message)) ? message[i] : '\0';
        sigqueue(server_pid, SIGUSR1, value); // Envoyer chaque caractère ou le caractère nul
        usleep(500); // Pause entre chaque signal
    }

    return 0; // Fin du programme
}
