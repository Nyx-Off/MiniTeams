#include <signal.h>    // Inclure pour les fonctions de signal
#include <stdio.h>     // Inclure pour les fonctions d'entrée/sortie
#include <stdlib.h>    // Inclure pour les fonctions standard
#include <string.h>    // Inclure pour les fonctions de manipulation de chaînes
#include <unistd.h>    // Inclure pour usleep
#include <time.h>      // Inclure pour les fonctions liées au temps

#define MAX_MESSAGE_LENGTH 1024  // Définir la longueur maximale du message

char message[MAX_MESSAGE_LENGTH]; // Tableau pour stocker le message reçu
int current_char = 0;             // Index pour le caractère courant dans le message
int message_updated = 0;          // Flag pour indiquer si le message a été mis à jour

void handle_signal(int signum, siginfo_t *info, void *context) {
    char received_char = info->si_value.sival_int; // Extraire le caractère reçu
    
    if (received_char == '\n') {  // Réinitialiser le message si c'est un nouveau message
        memset(message, 0, MAX_MESSAGE_LENGTH);
        current_char = 0;
    } else if (received_char == '\0') { // Fin du message
        // Générer l'horodatage
        char timestamp[64];
        time_t now = time(NULL);
        struct tm *tm_info = localtime(&now);
        strftime(timestamp, sizeof(timestamp), "[%H:%M:%S - %d/%m/%Y]", tm_info);

        // Écrire le message avec horodatage dans le fichier log
        FILE *log_file = fopen("messages.logs", "a");
        if (log_file != NULL) {
            fprintf(log_file, "%s : %s\n", timestamp, message);
            fclose(log_file);
        }

        // Afficher le message avec horodatage
        printf("%s : %s\n", timestamp, message);
        memset(message, 0, MAX_MESSAGE_LENGTH); // Préparation pour le prochain message
        current_char = 0;
        message_updated = 1;
    } else if (current_char < MAX_MESSAGE_LENGTH - 1) { // Ajout du caractère au message
        message[current_char++] = received_char;
    }
}

int main() {
    printf("Server PID: %d\n", getpid()); // Afficher le PID du serveur

    struct sigaction sa;  // Struct pour la configuration du signal
    memset(&sa, 0, sizeof(sa));
    sa.sa_flags = SA_SIGINFO; // Utiliser sa_sigaction pour recevoir des informations supplémentaires
    sa.sa_sigaction = &handle_signal; // Définir la fonction de gestion du signal

    sigaction(SIGUSR1, &sa, NULL); // Configurer la gestion du signal SIGUSR1

    // Boucle infinie pour attendre les signaux
    while (1) {
        if (message_updated) { // Si le message a été mis à jour
            message_updated = 0; // Réinitialiser le flag
        }
        usleep(100000); // Pause pour réduire la charge CPU
    }

    return 0; // Fin du programme
}
