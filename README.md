# Projet MiniTeams

## Introduction

Ce dépôt présente trois versions innovantes du projets MiniTeams. Chaque version explore une méthode différente pour la transmission de messages entre un client et un serveur, démontrant la polyvalence et l'efficacité des signaux dans les systèmes UNIX.

## Projets Inclus

1. **Communication IPC avec Signaux Binaires** :
   - Utilise `SIGUSR1` et `SIGUSR2` pour transmettre des bits individuels d'un message.
   - Le client envoie chaque bit, et le serveur reconstruit le message bit par bit.

2. **Communication IPC avec Transmission de Deux Bits** :
   - Améliore l'efficacité en transmettant deux bits à la fois à l'aide de quatre signaux différents (`SIGUSR1`, `SIGUSR2`, `SIGINT`, `SIGTERM`).
   - Cette méthode réduit de moitié le nombre de signaux nécessaires pour envoyer un message.

3. **Communication IPC avec `sigqueue`** :
   - Exploite `sigqueue` pour envoyer des caractères complets comme données supplémentaires avec des signaux.
   - Permet une communication plus riche et plus rapide, utilisant moins de signaux pour transmettre le même message.

## Fonctionnalités Communes

- Transmission de messages en utilisant les capacités des signaux UNIX.
- Horodatage et journalisation des messages reçus sur le serveur.
- Synchronisation et gestion des délais pour assurer l'intégrité des messages. (blablabla fichier logs quoi)

## Installation

1. Cloner le dépôt Git :
   ```
   git clone [URL_DU_DEPOT]
   ```

2. Compiler les fichiers sources pour chaque version :
   ```
   gcc -o client client.c
   gcc -o server server.c
   ```
   Répétez cette étape dans les répertoires de chaque version.

## Utilisation

### Pour chaque version :

1. **Exécuter le Serveur** :
   - Lancez le fichier exécutable du serveur.
   - Notez le PID affiché.

2. **Exécuter le Client** :
   - Avec le PID du serveur et un message comme arguments, lancez le client.

## Limitations et Considérations

- Sensibilité aux délais et à la synchronisation des signaux.
- Capacité de gestion des signaux varie selon le système d'exploitation.
- Longueur maximale des messages spécifique à chaque version. [j'ai liniter a 1024 caracteres mais on peut metre plus bref a vous de voir]

## Licence

Les projets sont distribués sous la licence MIT truc je crois, bref m'en fou .
