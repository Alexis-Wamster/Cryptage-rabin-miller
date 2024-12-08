#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utile.h"
#include <assert.h>

int main(int argc, char *argv[]) {
    initialisation();

    if (argc != 5 || (strcmp(argv[1], "-e") != 0 && strcmp(argv[1], "-d") != 0)) {
        fprintf(stderr, "Utilisation : %s -e|-d <fichier_clef> <fichier_in> <fichier_out>\n", argv[0]);
        return EXIT_FAILURE;
    }

    big_n n, e, d, messageIn, messageOut;
    FILE *fichierClef = fopen(argv[2], "r");
    FILE *fichierIn = fopen(argv[3], "r");
    FILE *fichierOut = fopen(argv[4], "w");

    if (fichierClef == NULL || fichierIn == NULL || fichierOut == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return EXIT_FAILURE;
    }

    read_hex(fichierClef, n);
    read_hex(fichierClef, e);
    read_hex(fichierClef, d);
    read_hex(fichierIn, messageIn);

    if (strcmp(argv[1], "-e") == 0) {
        exp_mod(messageIn, e, n, messageOut);
        write_hex(fichierOut, messageOut, '\n');
    }
    else if (strcmp(argv[1], "-d") == 0) {
        exp_mod(messageIn, d, n, messageOut);
        write_hex(fichierOut, messageOut, '\n');
    }

    fclose(fichierClef);
    fclose(fichierIn);
    fclose(fichierOut);

    //TEST
    big_n a, m, i1, i2;
    num_here("568754",a);
    num_here("45527",m);
    num_here("40203 ",i1);
    mod_inverse(a,m,i2);
    assert(cp_abs(i1, i2) == 0);

    return EXIT_SUCCESS;
}