#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

void send_char_as_signals(char c, pid_t server_pid) {
    for (int i = 7; i >= 0; --i) {
        int bit = (c >> i) & 1;
        if (bit == 0) {
            kill(server_pid, SIGUSR1);
        } else {
            kill(server_pid, SIGUSR2);
        }
        usleep(500); // 0.5ms
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
        send_char_as_signals(message[i], server_pid);
    }

    send_char_as_signals('\0', server_pid); // Envoyer le caractère nul à la fin du message

    return 0;
}
