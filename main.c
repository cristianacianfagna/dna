/*
 * main.c
 *
 *  Created on: 15/gen/2014
 *      Author: cristianac
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#define N 8


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
	char *costo; /* unita' elementari di energia richiesta dall'enzima per attivarsi */
	char *moltiplicatore; /*  valore da usare nel caso in cui il costo vari in base all'ultimo carattere dell'enzima  tipo di enzima: 1 se cambia o 0 altrimenti */
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
typedef struct esperimento esperimento;

//creo una nuova cava nella lista passandogli la testa della lista e le coordinate
tabella_costo* crea_tab_costi(char *nome, char *costo, char *molt, tabella_costo *t){

	tabella_costo *nuovo_el;

	nuovo_el=malloc(sizeof(tabella_costo));
	nuovo_el->nome_en=nome;
	nuovo_el ->costo=costo;
	nuovo_el ->moltiplicatore=molt;
	nuovo_el ->next=t;
	return nuovo_el;

}

/* funzione che legge dei caratteri e restituisce l'ultima posizione */
tabella_costo *readword(char *nomefile, char **en){
	char ch;
	char *stringa;
	int i=0, j=0;
	FILE *file;
	tabella_costo *tab_costi=NULL;

	file=fopen(nomefile,"r");
	for (j=0; j<N; j++){
		i=0;
		stringa=malloc(sizeof(char));
		while ((ch=fgetc(file)) != ' '){
			stringa[i]=ch;
			if (ch=='\n'){
				stringa[i]='\0';
				break;
			}
			//printf(" stringa[i]: %c - i:%d\n", stringa[i], i);
			i++;
			stringa=realloc(stringa, i+1*sizeof(char));
		}
		stringa[i]='\0';
		en[j]=stringa;
		//printf("en[j] punta a: %s j:%d\n", en[j], j);
		//j++;
	}
	//printf("\n j: %d \n", j);

	//creo la tabella (lista concatenata) passandogli i valori letti dal file
	for(i=0; i<j; i+=3){
		printf("en[i] punta a: %s \n", en[i]);
		printf("en[i+1] punta a: %s \n", en[i+1]);
		tab_costi=crea_tab_costi(en[i], en[i+1], en[i+2], tab_costi);
	}

	fclose(file);
	return tab_costi;
}

//stampa la lista partendo dalla cava (in realt dal sito) che passo
void printList_TC(tabella_costo *testa){
	tabella_costo *p;
	p=testa;
	printf("La tabella e' formata dai seguenti enzimi\n");
	for (p=testa; p != NULL; p = p->next){
		printf("%s %s %s\n", p->nome_en, p->costo, p->moltiplicatore);
	}
	printf("\n");
}



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
	char *enzimi[N];
	//int i, j;
	tabella_costo *tab=NULL;

	//leggo file "tabella_costi" passato al main
	tab=readword(argv[1], enzimi);

	printList_TC(tab);


	/*stampa di verifica del file "tabella costi" letto
	for (i=0; i<j; i++) {
		printf("%s\n", enzimi[i]);
	}*/


	return 1;
}
