#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include "utile.h"


// genere un entier aleatoire impaire de bits bits
void genererNombreAleatoireImpaire(big_n resultat, int bits) {
    big_n nombre;
    int i;

    zero_big(nombre);
    for (i=1; i<bits; i++){
        int nouveauBit = rand() % 2;
        mult_int(2, nombre, nombre);
        add_int(nouveauBit, nombre, nombre);
    }
    mult_int(2, nombre, nombre);
    add_int(1, nombre, nombre);
    copy_big(nombre, resultat);
}

// genere un nombre aleatoire dans resultat entre min et max
void genererNombreAleatoire(big_n min, big_n max, big_n resultat) {
    int i = PREC-1;
    int flag=1;
    big_n diff;
    sub_big(min, max, diff);

    while (diff[i] == 0 && i>=0){
        resultat[i] = 0;
        i--;
    }
    while (i>=0 && flag == 1){
        resultat[i] = rand() % (diff[i] + 1);
        if (resultat[i] != diff[i]){
            flag = 0;
        }
        i--;
    }
    while (i>=0){
        resultat[i] = rand();
        i--;
    }

    add_big(min, resultat, resultat);
}

//calcule de s et d tels que n-1 = d(2^s) avec d impair et s>0
void calculerSD(big_n n, big_n s, big_n d){
    big_n test;
    zero_big(s);
    copy_big(n, d);
    sub_int(1, d, d);

    mod_big(DEUX,d,test);
    while (cp_abs(test, ZERO) == 0) {
        div_int(2, d, d);
        add_int(1, s, s);
        mod_big(DEUX,d,test);
    }

    //test
    big_n nMoins1, d2, copieS;
    copy_big(s, copieS);
    sub_int(1,n,nMoins1);
    num_here("1", d2);
    while(cp_abs(ZERO,copieS) == 1){
        mult_int(2,d2,d2);
        sub_int(1,copieS,copieS);
    }
    mult_big(d,d2,d2);
    assert(cp_abs(d2,nMoins1)==0);
}

// verifie si un nombre est untemoin de miller (plus tourBoucle est grand plus le nombre est teste)
int estTemoinMiller(big_n n, int tourBoucle){
    printf("*");
    fflush(stdout);
    int i;
    big_n a, nMoins1, s, d, x, index;
    sub_int(1, n, nMoins1);
    for (i=0; i<tourBoucle; i++){
        genererNombreAleatoire(DEUX, nMoins1, a);
        assert(cp_abs(a, n)==1);
        assert(cp_abs(UN, a)==1);
        calculerSD(n, s, d);
        exp_mod(a, d, n, x);
        if (cp_abs(x, UN) == 0 || cp_abs(x, nMoins1) == 0) {
            return 0;
        }
        num_here("1", index);
        for (copy_big(UN, index); cp_abs(index, s) == 1; add_int(1,index,index)) {
            mult_big(x, x, x);
            mod_big(n, x, x);

            if (cp_abs(x, nMoins1) == 0) {
                return 0;
            }
        }
    }
    return 1;
}

// genere un nombre premier dans resultat de bits bits
void genererNombrePremier(big_n resultat, int bits) {
    big_n nombre;
    do{
        genererNombreAleatoireImpaire(nombre, bits);
    }
    while (estTemoinMiller(nombre, 20));
    copy_big(nombre, resultat);
}

// calcul de n, e, d avec n et d de taille tailleClef/2
void creerClefRSA(big_n n, big_n e, big_n d, int tailleClef) {
    big_n p, q, pMoins1, phiN;

    genererNombrePremier(p, tailleClef/2);
    genererNombrePremier(q, tailleClef/2);

    mult_big(p, q, n);

    sub_int(1, p, pMoins1);
    sub_int(1, q, phiN);
    mult_big(pMoins1, phiN, phiN);

    num_here("65537", e);

    mod_inverse(e, phiN, d);

    // test
    big_n test;
    mult_big(e, d, test);
    mod_big(phiN, test, test);
    assert(cp_abs(UN, test)==0);
}

void sauvegarderClef(const char *nomFichierPublique, const char *nomFichierPrivee, big_n n, big_n e, big_n d) {

    FILE* fichierPublique = fopen(nomFichierPublique, "w");
    FILE * fichierPrivee = fopen(nomFichierPrivee, "w");

    if (fichierPublique == NULL || fichierPrivee == NULL) {
        perror("Erreur lors de l'ouverture des fichiers");
        exit(EXIT_FAILURE);
    }

    write_hex(fichierPublique, n, '\n');
    write_hex(fichierPublique, e, '\n');

    write_hex(fichierPrivee, n, '\n');
    write_hex(fichierPrivee, e, '\n');
    write_hex(fichierPrivee, d, '\n');

    fclose(fichierPublique);
    fclose(fichierPrivee);
}

int main(int argc, char *argv[]) {
    printf("chargement : ");
    initialisation();

    srand(time(NULL));
    if (argc != 3) {
        fprintf(stderr, "Utilisation : %s <fichier_clef_publique> <fichier_clef_privee>\n", argv[0]);
        return EXIT_FAILURE;
    }

    big_n n, e, d;

    creerClefRSA(n, e, d, 1024);
    printf("\n");
    sauvegarderClef(argv[1], argv[2], n, e, d);

    return EXIT_SUCCESS;
}