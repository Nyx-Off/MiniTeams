#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

void send_bits_as_signal(char c, pid_t server_pid) {
    for (int i = 6; i >= 0; i -= 2) {
        int bits = (c >> i) & 3; // Récupère deux bits
        int signal;
        switch (bits) {
            case 0: signal = SIGUSR1; break; // 00
            case 1: signal = SIGUSR2; break; // 01
            case 2: signal = SIGINT; break;  // 10
            case 3: signal = SIGTERM; break; // 11
        }
        kill(server_pid, signal);
        usleep(500); // Délai ajusté
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <Server PID> <Message>\n", argv[0]);
        return 1;
    }

    pid_t server_pid = atoi(argv[1]);
    char *message = argv[2];

    for (int i = 0; i < strlen(message); ++i) {
        send_bits_as_signal(message[i], server_pid);
    }

    send_bits_as_signal('\0', server_pid); // Envoyer le caractère nul à la fin du message

    return 0;
}
