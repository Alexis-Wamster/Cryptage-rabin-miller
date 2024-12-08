#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "utile.h"

/*
Cette methode renvoie le nombre de unsigned int que contient un big_n.
On ne compte pas les 0 du poid fort
*/

int getNbIntInBigN(big_n n) {
    int i;
    for (i = PREC - 1; i >= 0; i--) {
        if (n[i] != 0) {
            return i+1;
        }
    }
    return 0;
}

/*
Cette methode ecrit un big_n dans fichierOut
tailleMax definis le nombre maximum de unsigned int que que contient vraiment le big_n (le but c'est de ne pas ecrire les 0 du poid fort)
En ecrivant le big_n, on regarde dans l'index 0 qui sert d'en tete le nombre de unsigned int qu'il contient vraiment.
Afin de ne pas ecrire les unsigned int qui ne codent pas le message

si tailleMax vaut -1, on ecrit tout le big_n sans se soucier de son poids fort
*/

void writeBigN(big_n data, FILE* fichierOut, int tailleMax){
    int limite = tailleMax-data[0]+1;
    int i;
    unsigned int entier;
    if (tailleMax == -1){
        tailleMax = PREC-1;
        limite = 0;
    }
    for (i = tailleMax; i >= limite; i--) {
        entier = data[i];
        fwrite(&entier, sizeof(unsigned int), 1, fichierOut);
    }
}

/*
Cette methode lit un big_n dans fichier et le stocke dans resultat
tailleMax definis le nombre maximum de unsigned int que doit contenir le big_n lu.
Le but c'est de ne pas depasser la valeur de la clef n.
En lisant le big_n, on se sert de l'index 0 comme en tete pour indiquer le nombre de unsigned int qu'il contient vraiment

si tailleMax vaut -1, on lit PREC-1 unsigned int dans le fichier, et on ne met pas d'en tete.
*/

int readBigN(FILE* fichier, big_n resultat, int tailleMax){
    int addEnTete=1;
    unsigned int entier;
    int nbEntierLu;
    zero_big(resultat);

    if (tailleMax == -1){
        tailleMax = PREC-1;
        addEnTete = 0;
    }

    int i=tailleMax;

    do{
        nbEntierLu = fread(&entier, sizeof(unsigned int), 1, fichier);
        if (nbEntierLu > 0){
            resultat[i] = entier;
            i --;
        }
        else{
            if (addEnTete){
                resultat[0] = tailleMax-i;
            }
            return 0;
        }
    } while (i>=addEnTete);

    if (addEnTete){
        resultat[0] = tailleMax-i;
    }

    return 1;
}

/* cette methode encrypte le fichier fichierIn avec les clef n et e et met le resultat dans fichierOut*/
void cryptageFichier(FILE* fichierIn, FILE* fichierOut, big_n n, big_n e) {

    big_n data, dataEncrypt;
    int ilResteDesBignALire;
    int tailleMaxData = getNbIntInBigN(n)-2;

    if (tailleMaxData < 0){
        perror("Votre clef n est trop petite");
        exit(EXIT_FAILURE);
    }
    do{
        ilResteDesBignALire = readBigN(fichierIn, data, tailleMaxData);
        exp_mod(data, e, n, dataEncrypt);
        writeBigN(dataEncrypt, fichierOut, -1);
    }while(ilResteDesBignALire);
}

/* cette methode decrypte le fichier fichierIn avec les clef n et d et met le resultat dans fichierOut*/
void decryptageFichier(FILE* fichierIn, FILE* fichierOut, big_n n, big_n d) {
    big_n data, dataDecrypt;
    int ilResteDesBignALire;
    int tailleMaxData = getNbIntInBigN(n)-2;

    if (tailleMaxData < 0){
        perror("Votre clef n est trop petite");
        exit(EXIT_FAILURE);
    }

    do {
        ilResteDesBignALire = readBigN(fichierIn, data, -1);
        exp_mod(data, d, n, dataDecrypt);
        writeBigN(dataDecrypt, fichierOut, tailleMaxData);
    } while (ilResteDesBignALire);
}



int main(int argc, char *argv[]) {
    initialisation();

    if (argc != 5 || (strcmp(argv[1], "-e") != 0 && strcmp(argv[1], "-d") != 0)) {
        fprintf(stderr, "Utilisation : %s -e|-d <fichier_clef> <fichier_in> <fichier_out>\n", argv[0]);
        return EXIT_FAILURE;
    }

    FILE* fichierIn = fopen(argv[3], "r");
    FILE* fichierOut = fopen(argv[4], "w");
    if (fichierIn == NULL || fichierOut == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    big_n n, e, d;
    FILE *fichierClef = fopen(argv[2], "r");

    if (fichierClef == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return EXIT_FAILURE;
    }

    read_hex(fichierClef, n);
    read_hex(fichierClef, e);
    read_hex(fichierClef, d);

    if (cp_abs(n, ZERO) == 0 || cp_abs(e, ZERO) == 0){
        perror("Merci de fournir un fichier clef valide");
        return EXIT_FAILURE;
    }

    if (strcmp(argv[1], "-e") == 0) {
        cryptageFichier(fichierIn, fichierOut, n, e);
    }
    else if (strcmp(argv[1], "-d") == 0) {
        if (cp_abs(d, ZERO) == 0){
            perror("Merci de fournir une fichier clef privee valide");
            return EXIT_FAILURE;
        }
        decryptageFichier(fichierIn, fichierOut, n, d);
    }

    fclose(fichierClef);
    fclose(fichierIn);
    fclose(fichierOut);

    return EXIT_SUCCESS;
}