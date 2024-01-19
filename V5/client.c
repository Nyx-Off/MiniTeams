#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>


#define MAX_MESSAGE_LENGTH 1024 // Définition de la longueur maximale d'un message

int main(int argc, char *argv[]) {
    // Vérifier que le nombre d'arguments est correct
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <Server PID>\n", argv[0]);
        return 1;
    }

    // Convertir le PID du serveur en nombre et le stocker
    pid_t server_pid = atoi(argv[1]);
    // Déclaration du tableau pour stocker le message
    char message[MAX_MESSAGE_LENGTH];
    // Structure pour envoyer des données avec les signaux
    #include <signal.h>

    union sigval value;

    if (!isatty(STDIN_FILENO)) {
        fprintf(stderr, "Erreur: Ce programme nécessite une entrée interactive.\n");
        return 1;
    }

    // Boucle infinie pour envoyer des messages en continu
    while (1) {
        printf("Entrez votre message: ");
        // Lire le message de l'utilisateur
        if (fgets(message, MAX_MESSAGE_LENGTH, stdin) == NULL) {
            if (feof(stdin)) {
                // Fin de l'entrée standard (EOF), quitter le programme
                printf("Fin de l'entrée standard. Arrêt du client.\n");
                return 0;
            } else {
                // Erreur de lecture, afficher un message et continuer
                perror("Erreur lors de la lecture de l'entrée standard");
                continue;
            }
        }
        // Supprimer le caractère de retour à la ligne à la fin du message
        message[strcspn(message, "\n")] = 0;

        // Vérifier que le message n'est pas trop long
        if (strlen(message) >= MAX_MESSAGE_LENGTH) {
            fprintf(stderr, "Erreur : Message trop long.\n");
            continue; // Ne pas envoyer le message et recommencer la boucle
        }

        // Obtenir le PID du client
        pid_t client_pid = getpid();
        // Envoyer le PID du client au serveur, octet par octet
        for (int i = 0; i < sizeof(pid_t); ++i) {
            value.sival_int = (client_pid >> (i * 8)) & 0xff;
            if (sigqueue(server_pid, SIGUSR1, value) == -1) {
                perror("Erreur lors de l'envoi du PID");
                return 1; // Quitter en cas d'erreur
            }
            usleep(500); // Petite pause pour la stabilité
        }

        // Envoyer le message au serveur, caractère par caractère
        for (int i = 0; i <= strlen(message); ++i) {
            value.sival_int = (i < strlen(message)) ? message[i] : '\0';
            if (sigqueue(server_pid, SIGUSR1, value) == -1) {
                perror("Erreur lors de l'envoi du signal");
                return 1; // Quitter en cas d'erreur
            }
            usleep(500); // Petite pause entre chaque caractère
        }
    }

    return 0; // Fin du programme
}
