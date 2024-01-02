#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

#define MAX_MESSAGE_LENGTH 1024 // Longueur maximale du message

// Fonction principale du programme
int main(int argc, char *argv[]) { // On récupère les arguments passés au programme
    // Vérifier le nombre d'arguments
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <Server PID> <Message>\n", argv[0]);
        return 1;
    }

    // Vérifier que le PID du serveur est un nombre
    pid_t server_pid = atoi(argv[1]); // PID du serveur 
    char *message = argv[2]; // Message à envoyer

    // Vérifier que le message n'est pas trop long
    if (strlen(message) > MAX_MESSAGE_LENGTH) {
        fprintf(stderr, "Erreur : Message trop long.\n");
        return 1;
    }

    // Vérifier que le PID du serveur est valide
    union sigval value;

    // Envoyer le PID du client
    pid_t client_pid = getpid(); // PID du client
    for (int i = 0; i < sizeof(pid_t); ++i) { // On envoie le PID octet par octet
        value.sival_int = (client_pid >> (i * 8)) & 0xff; // On décale le PID de 8 bits à droite et on le masque avec 0xff pour ne garder que les 8 premiers bits
        if (sigqueue(server_pid, SIGUSR1, value) == -1) { // On envoie le signal SIGUSR1 au serveur avec le PID du client
            perror("Erreur lors de l'envoi du PID"); // perror affiche un message d'erreur en fonction de la valeur de errno
            return 1;
        }
        usleep(500);
    }

     // Envoyer le message caractère par caractère (y compris le caractère nul de fin de chaîne)
    for (int i = 0; i <= strlen(message); ++i) { // On envoie le message caractère par caractère
        value.sival_int = (i < strlen(message)) ? message[i] : '\0'; // On envoie le caractère suivant ou le caractère nul de fin de chaîne
        if (sigqueue(server_pid, SIGUSR1, value) == -1) { // On envoie le signal SIGUSR1 au serveur avec le caractère à envoyer
            perror("Erreur lors de l'envoi du signal"); // perror affiche un message d'erreur en fonction de la valeur de errno
            return 1; // On quitte le programme avec un code d'erreur
        }
        usleep(500);
    }

    return 0;
}
