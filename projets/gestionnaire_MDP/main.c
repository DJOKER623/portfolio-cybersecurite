#include <sodium.h>
#include <stdio.h>
#include <string.h>

// Fonction pour afficher des octets bruts en Hexadécimal
void afficher_hex(const char* label, const unsigned char* data, size_t len) {
    printf("%s : ", label);
    for (size_t i = 0; i < len; i++) {
        printf("%02x", data[i]);
    }
    printf("\n");
}

int main() {
    // 0. Initialisation de libsodium
    if (sodium_init() < 0) {
        printf("Erreur : Impossible d'initialiser libsodium\n");
        return 1;
    }

    printf("--- TEST DE CHIFFREMENT XCHACHA20-POLY1305 (En pur C !) ---\n\n");

    // 1. Notre donnée en clair (Le faux JSON représentant un compte)
    const char* plaintext = "{\"id\":\"1\",\"user\":\"samsan\",\"pass\":\"MonSuperSecret!\"}";
    size_t plaintext_len = strlen(plaintext);
    printf("Message en clair (%zu octets) : %s\n\n", plaintext_len, plaintext);

    // 2. La Clé Secrète
    // Dans un vrai code, elle viendra d'Argon2id. Ici, on génère 32 octets de hasard pour le test.
    unsigned char key[crypto_secretbox_KEYBYTES];
    randombytes_buf(key, sizeof(key));
    afficher_hex("Cle secrete (RAM)", key, sizeof(key));

    // 3. Le Nonce (Vecteur d'initialisation)
    // Pour XChaCha20, le nonce fait obligatoirement 24 octets (crypto_secretbox_NONCEBYTES)
    unsigned char nonce[crypto_secretbox_NONCEBYTES];
    randombytes_buf(nonce, sizeof(nonce));
    afficher_hex("Nonce (IV public)", nonce, sizeof(nonce));

    // 4. Préparation du buffer pour le texte chiffré
    // Attention règle d'or de libsodium : le texte chiffré prend de la place !
    // Il fait la taille du texte en clair + 16 octets (crypto_secretbox_MACBYTES) pour le tag de sécurité
    size_t ciphertext_len = plaintext_len + crypto_secretbox_MACBYTES;
    unsigned char ciphertext[ciphertext_len];

    // 5. LE CHIFFREMENT
    // Cette fonction magique fait le chiffrement ET génère le tag Poly1305 d'un coup
    crypto_secretbox_easy(ciphertext, (const unsigned char*)plaintext, plaintext_len, nonce, key);
    
    printf("\n");
    afficher_hex("Texte Chiffre (Ciphertext + Tag)", ciphertext, ciphertext_len);
    printf("\n");

    // ==========================================
    // 6. LE DÉCHIFFREMENT (Pour vérifier que ça marche)
    // ==========================================
    
    // On prépare un buffer pour accueillir le texte déchiffré (+1 pour le caractère de fin de chaîne '\0')
    unsigned char decrypted[plaintext_len + 1];

    // On tente de déchiffrer
    if (crypto_secretbox_open_easy(decrypted, ciphertext, ciphertext_len, nonce, key) != 0) {
        // Si la fonction ne renvoie pas 0, c'est que soit la clé est mauvaise, soit le texte a été modifié
        printf("ALERTE : Dec chiffrement impossible ! Donnees corrompues ou mauvaise cle.\n");
        return 1;
    }

    // On ajoute le caractère de fin de chaîne pour pouvoir l'afficher avec printf
    decrypted[plaintext_len] = '\0';
    
    printf("Succes du dechiffrement !\n");
    printf("Message decouvert : %s\n", decrypted);
    printf("-------------------------------------------------------\n");

    return 0;
}