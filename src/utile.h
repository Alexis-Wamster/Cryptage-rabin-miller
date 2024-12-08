#ifndef UTILE_H
#define UTILE_H

#include "big.h"

extern big_n ZERO, UN, DEUX;

void mod_inverse(big_n a, big_n m, big_n result);
void exp_mod(big_n a, big_n d, big_n n, big_n result);
void initialisation();

#endif