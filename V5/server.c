#include <signal.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <sys/types.h>


// On définit un type pour le PID du client
#define MAX_MESSAGE_LENGTH 1024

char message[MAX_MESSAGE_LENGTH]; // Message reçu du client
int current_char = 0; // Position du prochain caractère à écrire dans le message
int message_updated = 0; // Indique si le message a été mis à jour
unsigned int client_pid = 0; // PID du client, traité comme non signé
int pid_bytes_received = 0; // Nombre d'octets reçus pour le PID

// Fonction appelée lors de la réception d'un signal
void handle_signal(int signum, siginfo_t *info, void *context) {
    unsigned char received_char = (unsigned char)info->si_value.sival_int; // Caractère reçu du client 

    //  Si le PID du client n'a pas encore été reçu, on le récupère
    if (pid_bytes_received < sizeof(pid_t)) { // On vérifie qu'on n'a pas déjà reçu tous les octets du PID
        client_pid |= ((unsigned int)received_char << (pid_bytes_received * 8)); // On décale le caractère reçu de 8 bits à gauche et on l'ajoute au PID du client
        pid_bytes_received++; // On incrémente le nombre d'octets reçus
        return;
    }

    // Si le caractère reçu est un retour à la ligne, on affiche le message
    if (received_char == '\n') {
        memset(message, 0, MAX_MESSAGE_LENGTH); // On réinitialise le message
        current_char = 0; // On réinitialise la position du prochain caractère à écrire
        client_pid = 0; // On réinitialise le PID du client
        pid_bytes_received = 0; // On réinitialise le nombre d'octets reçus
    } else if (received_char == '\0') { // Si le caractère reçu est un caractère nul, on affiche le message
        char timestamp[64]; // Timestamp du message (format : [HH:MM:SS - JJ/MM/AAAA])
        time_t now = time(NULL); // Date actuelle
        struct tm *tm_info = localtime(&now); // Date locale
        strftime(timestamp, sizeof(timestamp), "[%H:%M:%S - %d/%m/%Y]", tm_info); // Conversion de la date en chaîne de caractères

        FILE *log_file = fopen("messages.logs", "a");// Ouverture du fichier de logs en mode ajout
        if (log_file == NULL) { // Si le fichier n'a pas pu être ouvert, on affiche un message d'erreur et on quitte
            perror("Erreur lors de l'ouverture du fichier log"); // perror affiche un message d'erreur en fonction de la valeur de errno
            exit(1); // On quitte le programme avec un code d'erreur
        }
        fprintf(log_file, "[%s] - [From client %u] %s\n", timestamp, client_pid, message); // On écrit le message dans le fichier de logs
        fclose(log_file); // On ferme le fichier de logs

        printf("%s - [From client %u] %s\n", timestamp, client_pid, message); // On affiche le message dans la console
        memset(message, 0, MAX_MESSAGE_LENGTH); // On réinitialise le message
        current_char = 0; // On réinitialise la position du prochain caractère à écrire
        message_updated = 1; // On indique que le message a été mis à jour
        client_pid = 0; // On réinitialise le PID du client
        pid_bytes_received = 0; // On réinitialise le nombre d'octets reçus
    } else if (current_char < MAX_MESSAGE_LENGTH - 1) { // Si le caractère reçu est un caractère normal, on l'ajoute au message
        message[current_char++] = received_char; // On ajoute le caractère au message et on incrémente la position du prochain caractère à écrire
    }
}

// Fonction principale du programme
int main() { 
    printf("Server PID: %d\n", getpid()); // On affiche le PID du serveur

    struct sigaction sa; // Structure contenant les informations de configuration du signal
    memset(&sa, 0, sizeof(sa)); // On réinitialise la structure de configuration du signal
    sa.sa_flags = SA_SIGINFO; // On indique que la fonction de traitement du signal est sigaction
    sa.sa_sigaction = &handle_signal; // On indique la fonction de traitement du signal

    if (sigaction(SIGUSR1, &sa, NULL) == -1) { // On configure le signal SIGUSR1
        perror("Erreur lors de la configuration de SIGUSR1"); // perror affiche un message d'erreur en fonction de la valeur de errno
        exit(1); // On quitte le programme avec un code d'erreur
    }

    while (1) { // Boucle infinie
        if (message_updated) { // Si le message a été mis à jour, on envoie un signal au client
            message_updated = 0; // On indique que le message n'a pas été mis à jour
        }
        usleep(100000); // On attend 100ms
    }

    return 0; // On quitte le programme avec un code d'erreur
}
