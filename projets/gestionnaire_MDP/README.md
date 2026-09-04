---
layout: default
title: Gestionnaire de Mots de Passe
parent: Projets
nav_order: 2
---

# GDMP - Core Crypto (Gestionnaire de Mots de Passe)

*Projet personnel en cours de développement.*

Ce projet est le moteur cryptographique (Core) d'un futur gestionnaire de mots de passe développé en C/C++. L'objectif principal est de mettre en pratique des concepts de **cryptographie appliquée** en utilisant des standards modernes et robustes.

## Primitives Cryptographiques Utilisées

L'implémentation s'appuie exclusivement sur la bibliothèque **Libsodium** afin de garantir une sécurité maximale et d'éviter les vulnérabilités liées aux implémentations cryptographiques "maison".

*   **Dérivation de clé (KDF) : `Argon2id`**
    *   Utilisé pour transformer le mot de passe maître de l'utilisateur en une clé de chiffrement robuste de 256 bits (32 octets).
    *   Génération et stockage sécurisé d'un sel cryptographique (Salt) unique à chaque initialisation.
    *   Argon2id protège efficacement contre les attaques par force brute (GPU/ASIC) et les attaques par canal auxiliaire.
*   **Chiffrement Authentifié (AEAD) : `XChaCha20-Poly1305`**
    *   Utilisé pour chiffrer le coffre-fort (les données sensibles).
    *   **XChaCha20** assure la confidentialité des données (avec un Nonce étendu de 24 octets, évitant les collisions de vecteurs d'initialisation).
    *   **Poly1305** génère un tag d'authentification (MAC) garantissant l'intégrité absolue des données : toute modification du fichier chiffré bloquera le déchiffrement.

## 🔍 Extrait du code : Chiffrement XChaCha20-Poly1305

Voici l'implémentation en C utilisant Libsodium pour chiffrer et authentifier les données :

```c
// 5. LE CHIFFREMENT (Génère le texte chiffré ET le tag Poly1305)
crypto_secretbox_easy(ciphertext, (const unsigned char*)plaintext, plaintext_len, nonce, key);

if (crypto_secretbox_open_easy(decrypted, ciphertext, ciphertext_len, nonce, key) != 0) {
    printf("ALERTE : Déchiffrement impossible ! Données corrompues ou mauvaise clé.\n");
    return 1;
}
```

##  Compilation et Exécution

**Prérequis :**
*   Compilateur C/C++ (GCC/Clang)
*   CMake (>= 3.15)
*   Bibliothèque Libsodium (`libsodium-dev`)

**Instructions de build :**
```bash
mkdir build && cd build
cmake ..
make
./password_manager