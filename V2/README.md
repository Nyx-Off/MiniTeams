# Mini Teams V2

## Introduction

Ce projet étend la méthode de communication interprocessus (IPC) en utilisant des signaux UNIX pour transmettre des messages plus complexes entre un client et un serveur. Contrairement à l'envoi d'un bit à la fois, ce projet transmet deux bits à la fois, permettant une communication plus rapide et plus efficace.

## Fonctionnement Technique

### Côté Client (`client.c`)

1. **Conversion de Caractères en Groupes de Deux Bits** :
   - Chaque caractère est décomposé en groupes de deux bits.
   - Pour chaque groupe de deux bits, un signal spécifique est envoyé :
     - `SIGUSR1` pour 00.
     - `SIGUSR2` pour 01.
     - `SIGINT` pour 10.
     - `SIGTERM` pour 11.

2. **Transmission de Caractères** :
   - Les caractères sont envoyés en suivant l'ordre du message.
   - Un délai de 0.5ms est observé entre chaque signal pour garantir la bonne réception par le serveur.

3. **Fin de Message** :
   - Un caractère nul (`'\0'`) est envoyé à la fin du message pour signaler sa fin.

### Côté Serveur (`server.c`)

1. **Réception des Signaux** :
   - Le serveur attend en continu les signaux.
   - À la réception d'un signal, il détermine le groupe de deux bits correspondant.

2. **Reconstitution des Caractères** :
   - Les groupes de deux bits sont ajoutés au caractère courant en utilisant des opérations de décalage.
   - Une fois 8 bits reçus (formant un caractère complet), le serveur passe au caractère suivant.

3. **Gestion de la Fin de Message** :
   - La réception du caractère nul indique la fin du message.
   - Le message est affiché avec un horodatage et enregistré dans un fichier journal.

4. **Horodatage et Journalisation** :
   - Chaque message est accompagné d'un horodatage au format `[HH:MM:SS - DD/MM/YYYY]`.
   - Les messages sont enregistrés dans `messages.logs`.

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

2. Dans un autre terminal, exécuter le client avec le PID du serveur et le message :
   ```
   ./client [PID_DU_SERVEUR] "Votre Message Ici"
   ```

## Limitations et Considérations

- La communication est plus complexe en raison de la gestion de plusieurs signaux.
- La précision de l'horodatage dépend de la rapidité de traitement des signaux.
- La longueur du message est limitée à 1024 caractères.
