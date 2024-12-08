#include "big.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// calcul n! pour n passé 
// à la ligne de commande


void fact(int n, big_n f)
{
	num_here("1", f); // fact <- 1

	for(int i = 2; i <= n; i++)
		mult_int(i, f, f);
}
int main(int argc, char *argv[])
{
	int n ;
	big_n f;
	big_n fact_300;

	assert(argc >= 2);

	n = (int)strtol(argv[1], NULL, 0);
	fact(n, f);

	big_out ( 0 , NULL , f , stdout , '\0' , 0 , 0, 1, BNXICH , '\0',0);
	big_out ( 0 , NULL , f , stdout , '\0' , 0 , 0, 1, BNXICH , '\0',10);

	num_here("3060575122164406360353704612972686293885888041735769994167767412594765"
			"331767168674655152914224775733499391478887017263688642639077590031542268"
			"42927906974559841225476930271954604008012215776252176854255965356903506788"
			"725264321896264299365204576448830388909753943489625436053225980776521270822"
			"4376394491201286786753683057122936819436499564604981664502277165001851765464"
			"69340112226034729724066333258583506870150169794168850353752137554910289126407"
			"157154830282284937952636580145235233156936482233436799254594095276820608062232"
			"812387383880817049600000000000000000000000000000000000000000000000000000000000000000000000000",fact_300);

	fact(300, f);

	assert(cp_abs(fact_300, f) == 0);
	return 0;
}
