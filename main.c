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
#define E 8
#define N 10


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

struct provetta {
	frammento *fr; /* nucleotidi che formano il frammento */
	enzima *en; /* puntatore all'enzima attaccato al frammento*/
};
typedef struct provetta provetta;

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

//creo una nuova tabella passandogli la testa della tabella e i dati di ogni enzima
tabella_costo* crea_tab_costi(char *nome, char *costo, char *molt, tabella_costo *t){
	tabella_costo *nuovo_el;
	nuovo_el=malloc(sizeof(tabella_costo));
	nuovo_el->nome_en=nome;
	nuovo_el ->costo=costo;
	nuovo_el ->moltiplicatore=molt;
	nuovo_el ->next=t;
	return nuovo_el;
}

/* funzione che legge dei caratteri (allocandoli dinamicamente) dal file tabella_costi.txt (passato come primo argomento)
e memorizza le varie stringhe in un puntatore di puntatore a caratteri
passa i valori letti alla funzione crea_tab_costi che restituisce il puntatore alla tabella creata */
tabella_costo *leggi_TC(char *nomefile, char **en){
/* *nomefile: nome del file (primo argomento del main) */
/* **en: puntatore agli enzimi che verranno letti */
	char ch;
	char *stringa;
	int i=0, j=0;
	FILE *file;
	tabella_costo *tab_costi=NULL;

	file=fopen(nomefile,"r");
	for (j=0; j<E; j++){
		i=0;
		//leggo la prima stringa allocandola dinamicamente
		stringa=malloc(sizeof(char));
		//fino al primo spazio
		while ((ch=fgetc(file)) != ' '){
			stringa[i]=ch;
			if (ch=='\n'){
				stringa[i]='\0';
				break;
			}
			//vado alla posizione successiva e rialloco un carattere alla volta
			i++;
			stringa=realloc(stringa, i+1*sizeof(char));
		}
		stringa[i]='\0';
		//memorizzo la stringa letta in en[0], poi en[1] etc..
		en[j]=stringa;
	}

	//creo la tabella (lista concatenata) passandogli i valori letti dal file
	for(i=0; i<j; i+=3){
		tab_costi=crea_tab_costi(en[i], en[i+1], en[i+2], tab_costi);
	}
	free(stringa);
	fclose(file);
	return tab_costi;
}


//creo una nuova tabella passandogli la testa della tabella e i dati di ogni frammento
frammento* crea_frammenti(char *nome, tabella_costo *t){
	tabella_costo *nuovo_el;
	nuovo_el=malloc(sizeof(tabella_costo));
	nuovo_el->nome_en=nome;
	nuovo_el ->costo=costo;
	nuovo_el ->moltiplicatore=molt;
	nuovo_el ->next=t;
	return nuovo_el;
}

/* funzione che legge dei caratteri (allocandoli dinamicamente) dal file frammenti_file.txt (passato come secondo argomento)
e memorizza le varie stringhe in un puntatore di puntatore a caratteri
passa i valori letti alla funzione crea_frammenti che restituisce il puntatore al primo frammento */
tabella_costo *leggi_F(char *nomefile, char **frammenti){
/* *nomefile: nome del file (secondo argomento del main) */
/* **en: puntatore agli enzimi che verranno letti */
	char ch;
	char *stringa;
	int i=0, j=0;
	FILE *file;
	frammento *fr=NULL;

	file=fopen(nomefile,"r");
	for (j=0; j<N; j++){
		i=0;
		//leggo la prima stringa allocandola dinamicamente
		stringa=malloc(sizeof(char));
		//fino al primo spazio
		while ((ch=fgetc(file)) != ' '){
			stringa[i]=ch;
			if (ch=='\n'){
				stringa[i]='\0';
				break;
			}
			//vado alla posizione successiva e rialloco un carattere alla volta
			i++;
			stringa=realloc(stringa, i+1*sizeof(char));
		}
		stringa[i]='\0';
		//memorizzo la stringa letta in en[0], poi en[1] etc..
		frammenti[j]=stringa;
	}

	//creo la tabella (lista concatenata) passandogli i valori letti dal file
	for(i=0; i<j; i++){
		frammenti=crea_frammenti(frammenti[i], fr);
	}
	free(stringa);
	fclose(file);
	return frammenti;
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
	char *enzimi[E];
	char *frammenti[N];

	tabella_costo *tab=NULL;
	frammento *frammenti=NULL;

	//leggo file "tabella_costi" passato al main
	tab=leggi_TC(argv[1], enzimi);
	printList_TC(tab);


	/*stampa di verifica del file "tabella costi" letto
	for (i=0; i<j; i++) {
		printf("%s\n", enzimi[i]);
	}*/


	return 1;
}
