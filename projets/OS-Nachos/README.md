---
layout: default
title: OS Nachos
parent: Projets
nav_order: 3
---

# ⚙️ OS Nachos : Programmation Système & Noyau

*Projet académique (Master 1) - Implémentation de mécanismes bas niveau.*

Ce projet consistait à modifier et étendre le code source de Nachos, un système d'exploitation pédagogique, pour y intégrer des fonctionnalités critiques d'un OS moderne. L'objectif était d'interagir directement avec le matériel simulé et de gérer l'exécution des processus au plus bas niveau.

## 🛠️ Fonctionnalités Implémentées

*   **Appels Système (Syscalls) :** Implémentation des mécanismes de transition entre l'espace utilisateur (User-land / Ring 3) et l'espace noyau (Kernel-land / Ring 0). Création des ponts permettant aux programmes d'interagir de manière sécurisée avec le matériel.
*   **Concurrence et Multithreading :** Gestion de l'exécution concurrente de multiples processus.
*   **Synchronisation de Threads :** Mise en place de sémaphores, mutex et variables de condition pour protéger les sections critiques en mémoire.

## 🧠 Compétences Démontrées

*   **Compréhension de l'Architecture Système :** Maîtrise du fonctionnement des interruptions matérielles/logicielles et des contextes d'exécution.
*   **Prévention des Race Conditions :** Sécurisation de la mémoire partagée pour éviter les accès concurrents et les interblocages (*deadlocks*).
*   **Débogage Bas Niveau :** Traque de comportements indéterministes liés à l'ordonnancement des threads en utilisant GDB directement sur la mémoire de l'OS.

## 🔍 Extrait de Code : Gestion des Appels Système

Voici un aperçu de la façon dont le basculement entre l'espace utilisateur et le noyau a été géré lors d'une interruption logicielle (Syscall) :

```c
// Insère ici un court extrait de ton code. 
// Par exemple, ton bloc "ExceptionHandler" qui gère les codes d'appels système, 
// ou un extrait de ton implémentation de Mutex/Sémaphore.