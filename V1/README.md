# Mini Teams V1

## Introduction

Ce projet illustre une méthode de communication interprocessus (IPC) utilisant les signaux UNIX pour transmettre des messages entre un client et un serveur. Le client décompose les messages en bits et les envoie au serveur sous forme de signaux, qui sont ensuite reconstitués et traités par le serveur.

## Fonctionnement Technique

### Côté Client (`client.c`)

1. **Conversion de Caractères en Signaux** :
   - Chaque caractère du message est décomposé en bits.
   - Pour chaque bit, un signal est envoyé :
     - `SIGUSR1` pour représenter un bit à 0.
     - `SIGUSR2` pour un bit à 1.

2. **Transmission de Caractères** :
   - Les caractères sont envoyés un par un en suivant l'ordre du message.
   - Un délai de 0.5ms est observé entre l'envoi de chaque bit pour s'assurer que le serveur ait le temps de traiter le signal reçu.

3. **Fin de Message** :
   - Un caractère nul (`'\0'`) est envoyé à la fin du message pour indiquer sa fin.

### Côté Serveur (`server.c`)

1. **Réception des Signaux** :
   - Le serveur est en attente continue de signaux.
   - À la réception de `SIGUSR1` ou `SIGUSR2`, le serveur ajoute un bit au caractère en cours de formation.

2. **Reconstitution des Caractères** :
   - Les bits reçus sont ajoutés au caractère courant en utilisant des opérations de décalage et de masquage.
   - Après avoir reçu 8 bits (formant un caractère complet), le serveur passe au caractère suivant.

3. **Gestion de la Fin de Message** :
   - Lorsque le caractère nul est reçu, le serveur considère que le message est complet.
   - Le message est ensuite affiché avec un horodatage et enregistré dans un fichier journal.

4. **Horodatage et Journalisation** :
   - Chaque message est préfixé avec un horodatage au format `[HH:MM:SS - DD/MM/YYYY]`.
   - Les messages sont également enregistrés dans un fichier `messages.logs`.

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
   Prendre note du PID affiché.

2. Dans un autre terminal, exécuter le client avec le PID du serveur et le message à envoyer :
   ```
   ./client [PID_DU_SERVEUR] "Votre Message Ici"

## Limitations et Considérations

- La communication dépend de la gestion des signaux de l'OS.
- Sensible aux délais et à la synchronisation des signaux.
- Limité par la longueur maximale du message (1024 caractères).