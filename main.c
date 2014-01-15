/*
 * main.c
 *
 *  Created on: 15/gen/2014
 *      Author: cristianac
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


struct enzima {
	char *nome; /* nome enzima */
	int uea; /* unita' elementari di energia richiesta dall'enzima per attivarsi */
	int attivo; /* indica se l'enzina e' inibito (0) o se e' attivo (1)*/
	struct enzima *next; /*puntatore all'enzima successivo */
};
typedef struct enzima enzima;


struct frammento {
	char *nucleotidi; /* nucleotidi che formano il frammento */
	enzima *en; /* puntatore all'enzima attaccato al frammento*/
	struct frammento *next; /* puntatore al frammento successivo */
};
typedef struct frammento frammento;

struct tabella_costo {
	char *nome_en; /* nome enzima  */
	int costo; /* unita' elementari di energia richiesta dall'enzima per attivarsi */
	int moltiplicatore; /*  valore da usare nel caso in cui il costo vari in base all'ultimo carattere dell'enzima  tipo di enzima: 1 se cambia o 0 altrimenti */
	struct tabella_costo *next; /* puntatore all'enzima successivo */
};
typedef struct tabella_costo tabella_costo;

struct istante {
	frammento *elenco_f; /* puntatore ai frammenti dell'esperimento */
	enzima *elenco_en; /* puntatore agli enzimi dell'esperimento*/
	int istante; /* istante temporale dell'esperimento */
	int costo; /* somma dei costi (incluso se stesso) */
	struct istante *padre; /* puntatore all'istante precedente  */
	struct istante *figlio; /* puntatore all'istante successivo */
	struct istante *fratello; /* puntatore all'alternativa dello stesso istante */
};
typedef struct istante istante;

struct esperimento {
	istante *ist; /* puntatore all'istante 0 dell'esperimento */
	int tempo;  /* durata massima esperimento */
};
typedef struct istante istante;

/*funzione inizializza che:
legge dal file i costi e crea tabella_costo
*/

/*
 prepara_esperimento:
 creo frammenti
 creo enzimi
 torna la provetta

 esperimento()
 torna esperimento simulato fino all'istante delta


 creare struttura provetta
 */


int main (int argc, char *argv[]) {


}
