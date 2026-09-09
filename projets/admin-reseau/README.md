---
layout: default
title: Administration & Sécurité Réseau
parent: Projets
nav_order: 4
---

# Architecture, Administration et Sécurisation Réseau

*Projet académique (Master 1) - Déploiement et sécurisation d'infrastructures.*

Ce module regroupe l'ensemble des compétences acquises lors de la conception, du routage et de la sécurisation de réseaux locaux et étendus. L'objectif était de configurer des infrastructures complètes en ligne de commande sous Linux, d'analyser le trafic et de mettre en place des politiques de filtrage strictes.

## Compétences Déployées

### 1. Sécurité Périmétrique et Firewalling (Netfilter/Iptables)
*   **Filtrage (Stateful Firewall) :** Définition de politiques de sécurité strictes (DROP par défaut) sur les chaînes INPUT, OUTPUT et FORWARD, en filtrant le trafic en fonction des états de connexion (NEW, ESTABLISHED, RELATED).
*   **Translation d'adresses (NAT) :** Mise en place de SNAT/MASQUERADE pour l'accès internet d'un LAN, et de DNAT (Port Forwarding) pour rendre des services internes accessibles depuis l'extérieur (ex: serveurs HTTP ou SSH).
*   **Ségrégation et DMZ :** Conception d'architectures réseau sécurisées isolant les serveurs exposés du réseau local interne afin de limiter l'impact d'une compromission.

### 2. Analyse de Trafic et Manipulation de Paquets
*   **Analyse bas niveau :** Capture et inspection de trames via `tcpdump` et `wireshark` pour diagnostiquer des erreurs de routage, des requêtes ARP, ou analyser les codes de retour ICMP (*Destination host unreachable*, *Time exceeded*, *Redirect*).
*   **Forge de paquets :** Développement de scripts Python utilisant **Scapy** pour forger des paquets IP/ICMP sur mesure (ex: développement d'un outil de type `traceroute` personnalisé en analysant les paquets ICMP Type 11 et Type 0).

### 3. Routage et Architecture de Base
*   **Configuration d'interfaces :** Gestion des tables de routage statiques et configuration du forwarding IP.
*   **Réseaux Virtuels :** Déploiement de VLANs (Virtual LANs) pour segmenter logiquement le trafic et optimiser la gestion d'un réseau local .
*   **IPv6 :** Configuration d'adresses globales et locales, gestion du routage IPv6, et compréhension du protocole NDP (Neighbor Discovery Protocol) remplaçant ARP.

### 4. Services d'Infrastructure et Identité
*   **Annuaires et Partages :** Déploiement de serveurs OpenLDAP (`slapd`) pour la gestion centralisée des identités et configuration de partages de fichiers via NFS.
*   **Résolution de noms & DHCP :** Configuration de clients/serveurs DNS et maîtrise des mécanismes d'attribution d'adresses IP par diffusion (Broadcast).

## Extrait de Configuration : Pare-feu & NAT (iptables)

Voici un exemple des règles de pare-feu déployées pour sécuriser une passerelle, interdire le trafic non sollicité par défaut, et autoriser le trafic légitime vers un serveur spécifique :

```bash
# 1. Politique par défaut stricte : on bloque tout
iptables -F
iptables -t nat -F
iptables -P INPUT DROP
iptables -P OUTPUT DROP
iptables -P FORWARD DROP

# 2. Accès Internet pour le réseau local via la passerelle
iptables -t nat -A POSTROUTING -o eth1 -j MASQUERADE

# 3. Redirection de port (DNAT) pour un accès SSH externe vers une machine interne
iptables -t nat -A PREROUTING -p tcp -i eth2 -d 147.210.20.2 --dport 22 -j DNAT --to 192.168.0.2:22

# 4. Autorisation du trafic traversant (FORWARD) pour les connexions déjà établies
iptables -A FORWARD -m state --state ESTABLISHED,RELATED -j ACCEPT
```