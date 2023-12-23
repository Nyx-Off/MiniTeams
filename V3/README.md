# Mini Teams V3

## Introduction

Ce projet représente une évolution de la communication interprocessus (IPC) en utilisant la fonction `sigqueue` pour transmettre des messages enrichis entre un client et un serveur sous UNIX. Cette méthode permet l'envoi de données supplémentaires avec des signaux, offrant une flexibilité accrue par rapport aux méthodes traditionnelles utilisant `kill`.

## Fonctionnement Technique

### Côté Client (`client.c`)

1. **Préparation du Message** :
   - Chaque caractère du message, y compris un caractère nul à la fin, est envoyé séparément au serveur.
   - Un caractère spécial (`'\n'`) est envoyé au début pour signaler un nouveau message.

2. **Envoi des Caractères avec `sigqueue`** :
   - La fonction `sigqueue` est utilisée pour envoyer des signaux avec des données (`sival_int`) au serveur.
   - Chaque caractère est envoyé comme une donnée dans le signal `SIGUSR1`.

3. **Délais Entre les Signaux** :
   - Un délai de 0.5ms est maintenu entre l'envoi de chaque signal pour assurer la réception et le traitement corrects par le serveur.

### Côté Serveur (`server.c`)

1. **Configuration de la Réception de Signal** :
   - La structure `sigaction` est utilisée avec `SA_SIGINFO` pour recevoir des informations supplémentaires avec les signaux.
   - La fonction `handle_signal` est définie pour gérer les signaux `SIGUSR1`.

2. **Traitement des Signaux et Reconstruction du Message** :
   - Chaque signal reçu contient un caractère du message.
   - Le message est reconstruit caractère par caractère jusqu'à la réception du caractère nul.

3. **Horodatage et Journalisation** :
   - À la réception du caractère nul, le message complet est enregistré avec un horodatage dans `messages.logs`.
   - Le message est également affiché à l'écran.

## Installation

1. Cloner le dépôt Git :
   ```
   git clone [URL_DU_DEPOT]
   ```
2. Compiler les fichiers sources :
   ```
   gcc -o client client.c
   gcc -o server server.c
   ```

## Utilisation

1. Exécuter le serveur :
   ```
   ./server
   ```
   Noter le PID affiché.

2. Exécuter le client en indiquant le PID du serveur et le message :
   ```
   ./client [PID_DU_SERVEUR] "Votre Message Ici"
   ```

## Limitations et Considérations

- La méthode `sigqueue` permet une communication plus riche mais nécessite une synchronisation précise.
- La longueur du message est limitée à 1024 caractères.
- Le serveur utilise une boucle avec `usleep` pour réduire la charge CPU.
