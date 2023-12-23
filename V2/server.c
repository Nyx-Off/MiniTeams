#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#define MAX_MESSAGE_LENGTH 1024

int bit_count = 0;
char message[MAX_MESSAGE_LENGTH];
int current_char = 0;

void handle_signal(int signal) {
    int bits;
    switch (signal) {
        case SIGUSR1: bits = 0; break; // 00
        case SIGUSR2: bits = 1; break; // 01
        case SIGINT: bits = 2; break;  // 10
        case SIGTERM: bits = 3; break; // 11
    }

    message[current_char] = (message[current_char] << 2) | bits;

    bit_count += 2;
    if (bit_count >= 8) {
        bit_count = 0;
        current_char++;
         if (message[current_char - 1] == '\0') {
            char timestamp[64];
            time_t now = time(NULL);
            struct tm *tm_info = localtime(&now);
            strftime(timestamp, sizeof(timestamp), "[%H:%M:%S - %d/%m/%Y]", tm_info);

            printf("%s : %s\n", timestamp, message);

            // Écriture dans le fichier log avec horodatage
            FILE *log_file = fopen("messages.logs", "a");
            if (log_file != NULL) {
                fprintf(log_file, "%s : %s\n", timestamp, message);
                fclose(log_file);
            }

            memset(message, 0, MAX_MESSAGE_LENGTH);
            current_char = 0;
        } else if (current_char >= MAX_MESSAGE_LENGTH) {
            printf("Message too long, resetting.\n");
            memset(message, 0, MAX_MESSAGE_LENGTH);
            current_char = 0;
        }
    }
}

int main() {
    printf("Server PID: %d\n", getpid());

    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = &handle_signal;  
    sa.sa_flags = SA_RESTART;  // Ajoutez cette ligne pour relancer les appels système

    sigaction(SIGUSR1, &sa, NULL);
    sigaction(SIGUSR2, &sa, NULL);
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);

    while (1) {
        pause(); // Attente d'un signal
    }

    return 0;
}
