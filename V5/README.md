
# Projet de Communication Client-Serveur en C

## Sommaire
- [Aperçu](#aperçu)
- [client.c - Analyse Détaillée](#clientc---analyse-détaillée)
- [server.c - Analyse Détaillée](#serverc---analyse-détaillée)
- [Transmission de Données](#transmission-de-données)
- [Gestion des Emojis](#gestion-des-emojis)
- [Points Importants](#points-importants)
- [Conclusion](#conclusion)

## Aperçu
Ce projet implémente un système de communication entre un client et un serveur en utilisant des signaux UNIX (SIGUSR1) pour transmettre des données. Il met en évidence la communication asynchrone et la manipulation de données à bas niveau en C.

## client.c - Analyse Détaillée

### Inclusions et Définitions
- **Lignes 1-6:** Inclusion des bibliothèques standard pour le signal, l'E/S standard, la gestion des erreurs, etc.
- **Ligne 8:** Définition de `MAX_MESSAGE_LENGTH` pour la longueur maximale d'un message.

### Fonction Main
- **Lignes 13-17:** Vérification des arguments (le PID du serveur). Affichage d'erreur en cas d'argument manquant.
- **Ligne 20:** Conversion de l'argument PID en type `pid_t`.
- **Lignes 26-32:** Lecture de l'entrée utilisateur.
- **Lignes 35-62:** Boucle pour lire et envoyer des messages. Gestion des erreurs et de la fin de l'entrée (EOF).

## server.c - Analyse Détaillée

### Inclusions et Définitions
- **Lignes 1-12:** Similaires à `client.c`. Déclaration de variables globales pour le message et le PID du client.

### Fonction handle_signal
- **Lignes 14-49:** Gère les signaux SIGUSR1. Assemble le PID du client et traite les messages reçus.

### Fonction Main du Serveur
- **Lignes 51-64:** Configuration du gestionnaire de signaux et attente de signaux.

## Transmission de Données

### Mécanisme de Base
- **Utilisation de Signaux UNIX (SIGUSR1)** pour la communication entre les processus.

### Transmission des Données
- **Client:** Envoie du PID et du message octet par octet.
- **Serveur:** Réception et assemblage du PID et du message.

## Gestion des Emojis

### Codage en UTF-8
- Les emojis sont codés en UTF-8 (1 à 4 octets).
- **Client:** Lecture et envoi des emojis en séquence d'octets.
- **Serveur:** Réception et reconstruction des octets en emojis.

### Affichage des Emojis
- Nécessite que le terminal ou le système du serveur supporte l'UTF-8.

## Points Importants

### Asynchronicité
- Communication asynchrone, permettant au client de continuer après l'envoi d'un signal.

### Fiabilité et Performance
- Méthode non conventionnelle, moins efficace que les sockets TCP/IP.

### Gestion des Erreurs
- Manque de gestion robuste pour des situations comme la perte de signal.

## Conclusion
Ce code démontre une communication inter-processus utilisant des signaux UNIX. La gestion des emojis en UTF-8 est un aspect intéressant, bien que la méthode ne soit pas standard pour les applications nécessitant une communication fiable et à haut débit.
