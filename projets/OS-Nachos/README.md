---
layout: default
title: OS Nachos
parent: Projets
nav_order: 3
---

# OS Nachos : Programmation Système & Noyau

*Projet académique (Master 1) - Implémentation de mécanismes bas niveau.*

Ce projet consistait à modifier et étendre le code source de Nachos, un système d'exploitation pédagogique, pour y intégrer des fonctionnalités critiques d'un OS moderne. L'objectif était d'interagir directement avec le matériel simulé et de gérer l'exécution des processus au plus bas niveau.

## Fonctionnalités Implémentées

*   **Appels Système (Syscalls) :** Implémentation des mécanismes de transition entre l'espace utilisateur (User-land / Ring 3) et l'espace noyau (Kernel-land / Ring 0). Création des ponts permettant aux programmes d'interagir de manière sécurisée avec le matériel.
*   **Concurrence et Multithreading :** Gestion de l'exécution concurrente de multiples processus.
*   **Synchronisation de Threads :** Mise en place de sémaphores, mutex et variables de condition pour protéger les sections critiques en mémoire.

## Compétences Démontrées

*   **Compréhension de l'Architecture Système :** Maîtrise du fonctionnement des interruptions matérielles/logicielles et des contextes d'exécution.
*   **Prévention des Race Conditions :** Sécurisation de la mémoire partagée pour éviter les accès concurrents et les interblocages (*deadlocks*).
*   **Débogage Bas Niveau :** Traque de comportements indéterministes liés à l'ordonnancement des threads en utilisant GDB directement sur la mémoire de l'OS.

## Extrait de Code : Gestion de la Concurrence et Atomicity

Voici l'implémentation des mécanismes de synchronisation (Sémaphores et Verrous) dans le noyau. Pour garantir l'atomicité des opérations critiques (`Wait` / `Signal`) sur un monoprocesseur, les interruptions matérielles sont temporairement désactivées, empêchant ainsi tout changement de contexte inopiné.

```cpp
// Verrouillage (Mutex) basé sur les sémaphores
void Lock::Acquire() {
    ASSERT(!isHeldByCurrentThread()); // Prévention des deadlocks locaux
    semaphore->P();                   // Attente de la libération
    owner = currentThread;
}

// Opération P (Wait) d'un Sémaphore
void Semaphore::P() {
    // Désactivation des interruptions pour garantir l'atomicité
    IntStatus oldLevel = interrupt->SetLevel(IntOff); 

    ASSERT_MSG(value >= 0, "Semaphore became negative!?\n");

    while (value == 0) {                                      
        queue->Append((void *)currentThread); 
        currentThread->Sleep(); // Endormissement du thread si ressource indisponible
    }
    value--; 

    // Restauration de l'état précédent des interruptions
    (void)interrupt->SetLevel(oldLevel); 
}