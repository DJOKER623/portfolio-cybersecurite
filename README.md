# portfolio-cybersecurite
Portfolio
# Mervyn Samsan - Portfolio Cybersécurité & Ingénierie Système

**Étudiant en Master 2 CSI (Cryptanalyse et Sécurité Informatique)** passionné par la sécurité des systèmes, la rétro-ingénierie et l'architecture réseau.

À travers mon parcours académique et mes entraînements sur des plateformes comme Root-Me, j'ai développé une compréhension de la machine à son plus bas niveau, de l'exploitation de vulnérabilités complexes, et de la sécurisation des infrastructures.

---

## Compétences Techniques

*   **Sécurité & Exploitation :** Rétro-ingénierie, Buffer Overflow (CWE-120), Format String (CWE-134), ROP Chains, Shellcoding.
*   **Bypass de Protections :** Contournement d'ASLR, SMEP, PXN, et KALLSYMS.
*   **Système & Bas Niveau :** Programmation C, appels système (Linux/Nachos), gestion mémoire, synchronisation de threads, architecture x86/ARM.
*   **Réseau & Infra :** Architecture TCP/IP, Routage, Firewall/NAT, DNS, DHCP, LDAP, NFS/CIFS.
*   **Développement :** Python, C, Bash.

---

## Réalisations et Projets Phares

### 1. Exploitation Avancée et Sécurité Logicielle (CTF Root-Me)
*   **Exploitation Noyau (Ring 0) :** Exploitation de vulnérabilités de type *Null Pointer Dereference* et *Buffer Overflow* sur des modules noyau Linux (x86 et ARM).
*   **Contournement de Sécurité :** Mise en place d'attaques de type ROP (Return-Oriented Programming) pour neutraliser la protection matérielle SMEP. Développement de micro-exploits en pur assembleur pour contourner le PXN (Privileged execute-Never) sur architecture ARM.
*   **Développement de Shellcodes :** Création de shellcodes "Zero-Process" (ORW - Open, Read, Write) pour extraire des données tout en évitant la perte de privilèges SGID/SUID, avec encodage polymorphe pour esquiver les filtres de caractères.
*   **Documentation :** Rédaction de *write-ups* détaillant systématiquement l'analyse de la vulnérabilité, le processus d'exploitation et les mesures de mitigation modernes (ex: Stack Canaries, CFI).

### 2. Programmation Système & OS (Projet Nachos)
*   **Appels Système :** Implémentation et gestion d'appels système personnalisés au sein du noyau pédagogique Nachos.
*   **Concurrence :** Mise en place de mécanismes robustes de synchronisation de threads pour prévenir les *Race Conditions* et assurer l'intégrité de la mémoire partagée.

### 3. Architecture et Sécurisation Réseau
*   **Cœur de Réseau :** Configuration du routage avancé, déploiement de règles de pare-feu et gestion du NAT.
*   **Services d'Infrastructure :** Mise en place de serveurs DNS pour la résolution de noms, DHCP pour l'adressage automatique, et SMTP pour la messagerie.
*   **Gestion des Identités et Partages :** Déploiement d'un annuaire LDAP/NIS pour la gestion centralisée des utilisateurs et configuration de partages sécurisés (NFS & CIFS).

### 4. Ingénierie Logicielle & Cryptographie Appliquée
*Développement d'outils et de jeux logiques.*
*   **Gestionnaire de Mots de Passe (Projet Personnel) :** Développement d'une application de gestion des secrets intégrant des concepts de cryptographie pour le stockage sécurisé des données.
*   **Moteurs de Jeux en C :** Développement d'algorithmes de résolution avancés (Sudoku) et implémentation de jeux logiques (Takuzu, Water Sort) exigeant une gestion stricte des pointeurs et de la mémoire.

---

## Me Contacter

*   **LinkedIn :** https://www.linkedin.com/in/samsan-mervyn-48454b23b/
*   **Email :** mervyn_samsan@outlook.fr
*   **Root-Me :** https://www.root-me.org/Client_Server?lang=fr#4dc6942bf510548f0e81ea17a17e0a52