
# Mini Teams V4

## Introduction

Ce projet est une évolution de "Mini Teams V3", en ajoutant la capacité d'afficher le PID du client dans les messages envoyés à un serveur. Cette version utilise toujours `sigqueue` pour la communication interprocessus (IPC) sous UNIX, permettant une interaction plus sophistiquée entre le client et le serveur.

## Fonctionnement Technique

### Côté Client (`client.c`)

1. **Envoi du PID du Client** :
   - Avant d'envoyer le message, le client envoie son propre PID au serveur. Ce PID est envoyé octet par octet.

2. **Préparation et Envoi du Message** :
   - Chaque caractère du message, y compris le caractère nul final, est envoyé séparément au serveur.
   - Un caractère spécial (`'\n'`) est envoyé en premier pour indiquer le début d'un nouveau message.

3. **Envoi des Caractères avec `sigqueue`** :
   - `sigqueue` est utilisée pour envoyer chaque caractère du message ainsi que le PID du client comme données (`sival_int`) avec le signal `SIGUSR1`.
   - Un délai de 0.5ms est maintenu entre chaque signal pour garantir la réception correcte par le serveur.

### Côté Serveur (`server.c`)

1. **Réception et Traitement du PID du Client** :
   - Le serveur commence par reconstruire le PID du client à partir des premiers signaux reçus.

2. **Configuration de la Réception de Signal** :
   - La structure `sigaction` avec `SA_SIGINFO` permet de recevoir des informations supplémentaires avec les signaux.
   - `handle_signal` gère les signaux `SIGUSR1`, en reconstruisant le message et le PID.

3. **Reconstruction du Message et Affichage du PID du Client** :
   - Chaque signal contient un caractère du message, qui est reconstruit progressivement.
   - Lorsque le caractère nul est reçu, le message complet est enregistré avec le PID du client et un horodatage dans `messages.logs`, et affiché à l'écran.

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

- La méthode `sigqueue` permet une communication enrichie mais nécessite une synchronisation précise.
- La longueur du message est limitée à 1024 caractères.
- L'affichage du PID du client améliore la traçabilité mais augmente la complexité de la communication.

---
