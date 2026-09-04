#include <sodium.h>
#include <iostream>
#include <vector>
#include <iomanip>
#include <string>
#include <fstream> // NOUVEAU : Pour lire et écrire des fichiers

void afficher_hex(const std::string& label, const std::vector<unsigned char>& data) {
    std::cout << label << " : ";
    for (unsigned char c : data) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)c;
    }
    std::cout << std::dec << std::endl;
}

int main() {
    if (sodium_init() < 0) {
        std::cerr << "Erreur d'initialisation de libsodium." << std::endl;
        return 1;
    }

    std::string password = "TestPassword123!";
    std::string nom_fichier_sel = "salt.bin";
    std::vector<unsigned char> salt(crypto_pwhash_SALTBYTES);

    std::cout << "--- GESTIONNAIRE DE MOTS DE PASSE ---" << std::endl;

    // 1. TENTATIVE DE LECTURE DU FICHIER
    // std::ios::binary est crucial car notre sel n'est pas du texte, mais des octets purs
    std::ifstream fichier_lecture(nom_fichier_sel, std::ios::binary);

    if (fichier_lecture.is_open()) {
        // MODE AUTHENTIFICATION
        std::cout << "[Etat] Fichier trouve -> MODE AUTHENTIFICATION" << std::endl;
        
        // On lit le fichier et on le met dans notre variable 'salt'
        // (Le reinterpret_cast sert juste à dire au C++ "Fais-moi confiance, lis ces octets")
        fichier_lecture.read(reinterpret_cast<char*>(salt.data()), salt.size());
        fichier_lecture.close();
    } else {
        // MODE INITIALISATION
        std::cout << "[Etat] Aucun fichier trouve -> MODE INITIALISATION (Premier lancement)" << std::endl;
        
        // On génère un nouveau sel aléatoire
        randombytes_buf(salt.data(), salt.size());
        
        // On crée le fichier pour sauvegarder ce sel
        std::ofstream fichier_ecriture(nom_fichier_sel, std::ios::binary);
        fichier_ecriture.write(reinterpret_cast<const char*>(salt.data()), salt.size());
        fichier_ecriture.close();
        
        std::cout << "[Info] Nouveau Sel sauvegarde dans " << nom_fichier_sel << std::endl;
    }

    afficher_hex("\nSel utilise", salt);

    // 2. CALCUL DE LA CLÉ ARGON2ID (Identique à avant)
    std::vector<unsigned char> key(32);
    
    if (crypto_pwhash(
            key.data(), key.size(),
            password.c_str(), password.length(),
            salt.data(),
            crypto_pwhash_OPSLIMIT_INTERACTIVE,
            crypto_pwhash_MEMLIMIT_INTERACTIVE,
            crypto_pwhash_ALG_ARGON2ID13) != 0) {
        std::cerr << "Erreur de calcul de la cle." << std::endl;
        return 1;
    }

    afficher_hex("Cle derivee", key);
    std::cout << "-------------------------------------" << std::endl;

    return 0;
}