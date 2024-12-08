#include <stdio.h>
#include <stdlib.h>
#include "utile.h"

big_n ZERO, UN, DEUX;

void initialisation(){
    num_here("0", ZERO);
    num_here("1", UN);
    num_here("2", DEUX);
}

// resultat = inv de a mod m
void mod_inverse(big_n a, big_n m, big_n resultat) {
    big_n copieM, t, q, x, copieA;
    copy_big(a, copieA);
    copy_big(m, copieM);
    num_here("0", x);
    num_here("1", resultat);

    while(cp_abs(copieA, UN) == -1){ // while (copieA > 1)
        div_big(copieM, copieA, q);
        copy_big(copieM, t);

        mod_big(copieM, copieA, copieM);
        copy_big(t, copieA);

        copy_big(x, t);
        mult_big(q, x, x);
        sub_big(x, resultat, x);

        copy_big(t, resultat);
    }

    while (resultat[PREC] == 1){ // if 0 > resultat
        add_big(m, resultat, resultat);
    }
}

// calcul resultat = a^d mod n
void exp_mod(big_n a, big_n d, big_n n, big_n resultat) {
    big_n bit_poidFaible, pow, copieD;
    num_here("1", resultat);
    copy_big(a, pow);
    copy_big(d, copieD);

    while (cp_abs(copieD, ZERO) != 0) {
        mod_big(DEUX, copieD, bit_poidFaible);
        if (cp_abs(bit_poidFaible, UN) == 0) { // if poidFaible == 1
            mult_big(resultat, pow, resultat);
            mod_big(n, resultat, resultat);
        }

        mult_big(pow, pow, pow);
        mod_big(n, pow, pow);

        div_int(2, copieD, copieD);
    }
}