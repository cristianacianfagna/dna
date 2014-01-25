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
#include <ctype.h>
#define E 9
#define F 10




struct s_enzima {
	char *nome; /* nome enzima */
	int uea; /* unita' elementari di energia richiesta dall'enzima per attivarsi */
	int attivo; /* indica se l'enzina e' inibito (0) o se e' attivo (1)*/
	struct s_enzima *next; /*puntatore all'enzima successivo */
};
typedef struct s_enzima t_enzima;


struct s_frammento {
	char *nucleotidi; /* nucleotidi che formano il frammento */
	t_enzima *en; /* puntatore all'enzima attaccato al frammento*/
	struct s_frammento *next; /* puntatore al frammento successivo */
};
typedef struct s_frammento t_frammento;

struct s_provetta {
	t_frammento *fr; /* nucleotidi che formano il frammento */
	t_enzima *en; /* puntatore all'enzima attaccato al frammento*/
};
typedef struct s_provetta t_provetta;

struct s_tabella_costo {
	char *nome_en; /* nome enzima  */
	int costo; /* unita' elementari di energia richiesta dall'enzima per attivarsi */
	int moltiplicatore; /*  valore da usare nel caso in cui il costo vari in base all'ultimo carattere dell'enzima  tipo di enzima: 1 se cambia o 0 altrimenti */
	struct s_tabella_costo *next; /* puntatore all'enzima successivo */
};
typedef struct s_tabella_costo t_tabella_costo;



struct s_istante {
	int istante; /* istante temporale dell'esperimento */
	int costo; /* somma dei costi (incluso se stesso) */
	struct s_istante *padre; /* puntatore all'istante precedente  */
	struct s_istante *figlio; /* puntatore all'istante successivo */
	struct s_istante *fratello; /* puntatore all'alternativa dello stesso istante */
	t_frammento *fr; /* il frammento con applicato enzima */
	t_enzima *en; /* puntatore all'enzima presente nella provetta */
};
typedef struct s_istante t_istante;

struct s_lista_soluzioni {
	t_istante *istante; /* puntatore all'istante trovato*/
	struct s_lista_soluzioni *next; /*puntatore alla soluzione successiva */
};
typedef struct s_lista_soluzioni t_lista_soluzioni;

struct s_esperimento {
	t_istante *ist; /* puntatore all'istante 0 dell'esperimento */
	int tempo;  /* durata massima esperimento */
	t_provetta *provetta; /* puntatore alla provetta dell'esperimento */
	t_tabella_costo *tc; /* puntatore alla tabella dei costi */
};
typedef struct s_esperimento t_esperimento;

//creo una nuova tabella passandogli la testa della tabella e i dati di ogni enzima
t_tabella_costo* crea_tab_costi(char *nome, char *costo, char *molt, t_tabella_costo *t){
	t_tabella_costo *nuovo_el;
	nuovo_el=malloc(sizeof(t_tabella_costo));
	nuovo_el->nome_en=nome;
	nuovo_el ->costo=atoi(costo);
	nuovo_el ->moltiplicatore=atoi(molt);
	nuovo_el ->next=t;
	return nuovo_el;
}

/* funzione che legge dei caratteri (allocandoli dinamicamente) dal file tabella_costi.txt (passato come primo argomento)
e memorizza le varie stringhe in un puntatore di puntatore a caratteri
passa i valori letti alla funzione crea_tab_costi che restituisce il puntatore alla tabella creata */
t_tabella_costo *leggi_TC(char *nomefile){
/* *nomefile: nome del file (primo argomento del main) */
/* **en: puntatore agli enzimi che verranno letti */
	char ch;
	char *stringa;
	char *deposito[3];
	int i=0, j=0;
	FILE *file;
	t_tabella_costo *tab_costi=NULL;

	file=fopen(nomefile,"r");
	i=0;
	j=0;
	//leggo la prima stringa allocandola dinamicamente
	stringa=malloc(sizeof(char));
	//fino al primo spazio
	while ((ch=fgetc(file)) != EOF){
		stringa[i]=ch;
		if (ch==' '){
			stringa[i]='\0';
			deposito[j]=stringa;
			j++;
			i=0;
			stringa=malloc(sizeof(char));
		}
		else if (ch=='\n'){
			stringa[i]='\0';
			deposito[j]=stringa;
			tab_costi=crea_tab_costi(deposito[j-2], deposito[j-1], deposito[j] , tab_costi);
			//free(stringa);
			stringa=malloc(sizeof(char));
			i=0;
			j=0;
		} else {
			//vado alla posizione successiva e rialloco un carattere alla volta
			i++;
			stringa=realloc(stringa, i+1*sizeof(char));
		}


	}

	free(stringa);
	fclose(file);
	return tab_costi;
}


//creo una nuova tabella passandogli la testa della tabella e i dati di ogni frammento
t_frammento *crea_frammenti(char *nome, t_frammento *t){
	t_frammento *nuovo_el;
	nuovo_el=malloc(sizeof(t_tabella_costo));
	nuovo_el->nucleotidi=nome;
	nuovo_el ->en=NULL;
	nuovo_el ->next=t;
	return nuovo_el;
}

/* funzione che legge dei caratteri (allocandoli dinamicamente) dal file frammenti_file.txt (passato come secondo argomento)
e memorizza le varie stringhe in un puntatore di puntatore a caratteri
passa i valori letti alla funzione crea_frammenti che restituisce il puntatore al primo frammento */
t_frammento *leggi_F(char *nomefile){
/* *nomefile: nome del file (secondo argomento del main) */
/* **frammenti: puntatore ai frammenti che verranno letti */
	char ch;
	char *stringa;
	int i=0;
	FILE *file;
	t_frammento *fr=NULL;

	file=fopen(nomefile,"r");
	//leggo la prima stringa allocandola dinamicamente
	stringa=malloc(sizeof(char));
	//fino al primo spazio
	while ((ch=fgetc(file)) != EOF){
		stringa[i]=ch;
		if (ch=='\n'){
			stringa[i]='\0';
			fr=crea_frammenti(stringa, fr);
			//free(stringa);
			stringa=malloc(sizeof(char));
			i=0;
		} else {
			//vado alla posizione successiva e rialloco un carattere alla volta
			i++;
			stringa=realloc(stringa, i+1*sizeof(char));
		}


	}

	free(stringa);
	fclose(file);
	return fr;
}

//restituisce la quantita' di energia in UEA necessaria all'enzima "nome_enzima" per provocare la trasformazione
int energia_enzima(char *nome_enzima, t_tabella_costo *t){
	int costo=0;
	int l=0, val=0;
	t_tabella_costo *x;
	x=t;
	//scorro la tabella dei costi
	while (x!= NULL){
		//se lo trovo
		if ((strstr(nome_enzima, x->nome_en) != NULL)){
			//se ha il moltiplicatore a 0, restituisco il costo
			if (x->moltiplicatore == 0)
				costo=x->costo;
			else {
				//altrimenti ricavo la n e la moltiplico per il costo
				l=strlen(nome_enzima);
				val=nome_enzima[l-1]-'0';
				costo=x->costo*val;
			}
		}
		x=x->next;
	}
	return costo;

}


//creo una nuova tabella passandogli la testa della tabella e i dati di ogni frammento
t_enzima *crea_enzimi(char *nome, t_enzima *t, t_tabella_costo *tx){
	t_enzima *nuovo_el;
	t_enzima *p;
	int esiste=0;
	p=t;
	//scorro prima la lista per vedere se c'e' gia' l'enzima
	while (p!=NULL){
		if (strcmp(p->nome, nome)==0)
			esiste=1;
		p=p->next;
	}

	if (esiste==0){
		nuovo_el=malloc(sizeof(t_enzima));
		nuovo_el->nome=nome;
		nuovo_el ->uea=energia_enzima(nome, tx);
		nuovo_el ->attivo=1;
		nuovo_el ->next=t;
		return nuovo_el;
	} else
		return t;
}

/* funzione che legge dei caratteri (allocandoli dinamicamente) dal file enzimi_file.txt (passato come terzo argomento)
e memorizza le varie stringhe in un puntatore di puntatore a caratteri
passa i valori letti alla funzione crea_enzimi che restituisce il puntatore al primo frammento */
t_enzima *leggi_E(char *nomefile, t_tabella_costo *tx){
/* *nomefile: nome del file (secondo argomento del main) */
/* **enzimi: puntatore agli enzimi che verranno letti */
	char ch;
	char *stringa;
	int i=0;
	FILE *file;
	t_enzima *en=NULL;

	file=fopen(nomefile,"r");
	//leggo la prima stringa allocandola dinamicamente
	stringa=malloc(sizeof(char));
	//fino al primo spazio
	while ((ch=fgetc(file)) != EOF){
		stringa[i]=ch;
		if (ch=='\n'){
			stringa[i]='\0';
			en=crea_enzimi(stringa, en, tx);
			//free(stringa);
			stringa=malloc(sizeof(char));
			i=0;
		} else {
			//vado alla posizione successiva e rialloco un carattere alla volta
			i++;
			stringa=realloc(stringa, i+1*sizeof(char));
		}
	}

	free(stringa);
	fclose(file);
	return en;
}

//stampa la lista dei valori della tabella costi partendo dalla tabella che passo (quella in testa)
void printList_TC(t_tabella_costo *testa){
	t_tabella_costo *p;
	p=testa;
	printf("La tabella e' formata dai seguenti enzimi\n");
	for (p=testa; p != NULL; p = p->next){
		printf("%s %d %d\n", p->nome_en, p->costo, p->moltiplicatore);
	}
	printf("\n");
}

//stampa la lista dei frammenti letti partendo dal frammento che passo (quello in testa)
void printList_F(t_frammento *testa){
	t_frammento *p;
	p=testa;
	printf("I frammenti letti sono:\n");
	for (p=testa; p != NULL; p = p->next){
		printf("%s\n", p->nucleotidi);
	}
	printf("\n");
}

//stampa la lista dei frammenti letti partendo dal frammento che passo (quello in testa)
void printList_E(t_enzima *testa){
	t_enzima *p;
	p=testa;
	printf("Gli enzimi sono:\n");
	for (p=testa; p != NULL; p = p->next){
		printf("nome: %s - attivo: %d\n", p->nome, p->attivo);
	}
	printf("\n");
}

//stampa la lista dei frammenti letti partendo dal frammento che passo (quello in testa)
void printList_ESP(t_esperimento *testa){
	t_istante *p;
	printf("Esperimento:\n");
	for (p=testa->ist; p != NULL; p = p->fratello){
		printf("istante: %d frammento: %10s\n", p->istante, p->fr->nucleotidi);
	}
	printf("\n");
}

void printList_SOL(t_lista_soluzioni *ls){
	t_lista_soluzioni *t;
	printf("La lista delle soluzioni:\n");
	for (t=ls; t!=NULL; t=t->next){
		printf("istante: %d frammento: %10s\n", t->istante->istante, t->istante->fr->nucleotidi);
	}
	printf("\n");

}

void print_sequenza_enzimi_din(t_lista_soluzioni *ls){
	t_lista_soluzioni *t;
	t_lista_soluzioni *tmp=NULL;
	t_lista_soluzioni *tmp_testa=NULL;


	t_istante *i=NULL;
	printf("\nEnzimi da aggiungere alla provetta con programmazione dinamica:\n");
	for (t=ls; t!=NULL; t=t->next){
		tmp_testa=NULL;
		//scorro verso l'alto
		for(i=t->istante; i!=NULL; i=i->padre){
			tmp=malloc(sizeof(t_lista_soluzioni));
			tmp->istante=i;
			tmp->next=tmp_testa;
			tmp_testa=tmp;
		}
		tmp=tmp_testa;
		while (tmp!=NULL){
			if (tmp->istante->en == NULL)
				printf("istante: %d - frammento: %s\n", tmp->istante->istante, tmp->istante->fr->nucleotidi);
			else
				printf("istante: %d - frammento: %s + enzima %s = %s\n", tmp->istante->istante, tmp->istante->padre->fr->nucleotidi, tmp->istante->en->nome, tmp->istante->fr->nucleotidi);

			tmp=tmp->next;
		}


		printf("\n");
	}
}



void print_ric(t_istante *ist){
	t_istante *i=ist;
	if (i->padre != NULL){
		print_ric(i->padre);
	}

	if (i->en == NULL)
		printf("istante: %d - frammento: %s\n", i->istante, i->fr->nucleotidi);
	else
		printf("istante: %d - frammento: %s + enzima %s = %s\n", i->istante, i->padre->fr->nucleotidi, i->en->nome, i->fr->nucleotidi);


}

void print_sequenza_enzimi_ric(t_lista_soluzioni *ls){
	t_lista_soluzioni *t;
	printf("\nEnzimi da aggiungere alla provetta con chiamata ricorsiva:\n");
	for (t=ls; t!=NULL; t=t->next){
		print_ric(t->istante);
		printf("\n");
	}
}

void print_energia_enzimi_din(t_lista_soluzioni *ls){
	t_lista_soluzioni *t;
	t_lista_soluzioni *tmp=NULL;
	t_lista_soluzioni *tmp_testa=NULL, *tmp_last=NULL;
	int min=INT_MAX;
	t_lista_soluzioni *lista_min=NULL;

	t_istante *i=NULL;
	printf("\nEnergia necessaria per ottenere il frammento (con programmazione dinamica):\n");
	for (t=ls; t!=NULL; t=t->next){
		tmp_testa=NULL;
		//scorro verso l'alto
		for(i=t->istante; i!=NULL; i=i->padre){
			tmp=malloc(sizeof(t_lista_soluzioni));
			tmp->istante=i;
			tmp->next=tmp_testa;
			tmp_testa=tmp;
		}
		tmp=tmp_testa;
		while (tmp!=NULL){
			if (tmp->istante->en == NULL)
				printf("istante: %d - frammento: %s - uea: %d\n", tmp->istante->istante, tmp->istante->fr->nucleotidi, tmp->istante->costo);
			else
				printf("istante: %d - frammento: %s + enzima %s = %s - costo: %d\n", tmp->istante->istante, tmp->istante->padre->fr->nucleotidi, tmp->istante->en->nome, tmp->istante->fr->nucleotidi, tmp->istante->costo);

			tmp_last=tmp;
			tmp=tmp->next;
		}
		printf("\n");
		if (tmp_last->istante->costo<min){
			min=tmp_last->istante->costo;
			lista_min=tmp_testa;
		}

	}

	printf("La soluzione meno costosa e':\n");
	//riscorro la lista di costo minimo per stamparla
	while (lista_min!=NULL){
		if (lista_min->istante->en == NULL)
			printf("istante: %d - frammento: %s - uea: %d\n", lista_min->istante->istante, lista_min->istante->fr->nucleotidi, lista_min->istante->costo);
		else
			printf("istante: %d - frammento: %s + enzima %s = %s - costo: %d\n", lista_min->istante->istante, lista_min->istante->padre->fr->nucleotidi, lista_min->istante->en->nome, lista_min->istante->fr->nucleotidi, lista_min->istante->costo);


		lista_min=lista_min->next;
	}
	printf("\n");

}



//funzione che crea la provetta verificando che gli enzimi siano attivi
t_provetta *crea_provetta(t_frammento *testa_fr, t_enzima *testa_en){
	t_provetta *nuovo_el;
	t_enzima *p;
	p=testa_en;

	//verifico che gli enzimi siano tutti attivi e in caso contrario li attivo
	while (p!= NULL) {
		if (p->attivo == 0 )
				p->attivo=1;
		p=p->next;
	}

	nuovo_el=malloc(sizeof(t_provetta));
	nuovo_el->fr=testa_fr;
	nuovo_el ->en=testa_en;

	return nuovo_el;
}


t_enzima *elimina_enzima(char *nome_en, t_enzima *testa_en){
	t_enzima *p;
	p=testa_en;
	while (p!=NULL){
		if ((strcmp(p->nome, nome_en)) == 0){
			p->attivo = 0;
		}
		p=p->next;
	}
	return testa_en;


}

//se l'enzima non e' presente lo aggiunge, e se e' gia' presente ma e' inibito lo riattiva
void aggiungi_enzima(t_provetta *pr, char *nome_en, t_tabella_costo *tx){
	t_enzima *p;
	int trovato=0;
	p=pr->en;
	while (p!=NULL){
		if (strcmp(p->nome, nome_en) == 0){
			trovato=1;
			p->attivo=1;
		}
		p=p->next;
	}
	p=pr->en;
	if (trovato==0){
		crea_enzimi(nome_en, p, tx);
	}
}

//funzione che restituisce il frammento ottenuro da frammento_src mediante l'enzima nome_enzima
char *enzima(char *nome_enzima, char *frammento_src, t_tabella_costo *tx){
	t_tabella_costo *x;
	char *nuovo_fr;
	char c;
	int l_enzima=0, l=0, n=0, i=0, j=0;
	x=tx;
	//scorro la tabella degli enzimi per verificare che sia un enzima corretto
	while (x!=NULL){
		//se lo trovo
		if ((strstr(nome_enzima, x->nome_en)) != NULL){
			switch(*nome_enzima){
			//se e' del tipo insertasi
			case 'i':
				l_enzima=strlen(nome_enzima);
				l=strlen(frammento_src);
				n=nome_enzima[l_enzima-2]-'0';
				c=toupper(nome_enzima[l_enzima-1]);
				nuovo_fr=malloc((l+1)*sizeof(char));
				//verifico allocazione
				if (nuovo_fr != NULL){
					for (i=0; i<l+1; i++){
						if (i<n)
							nuovo_fr[i]=frammento_src[i];
						else if (i==n)
							nuovo_fr[i]=c;
						else
							nuovo_fr[i]=frammento_src[i-1];
					}
				}
				break;
			//se e' del tipo roovesciasi
			case 'r':
				l=strlen(frammento_src);
				j=l-1;
				nuovo_fr=malloc(l*sizeof(char));
				//verifico allocazione
				if (nuovo_fr != NULL){
					for (i=0; i<l; i++){
						nuovo_fr[i]=frammento_src[j];
						j--;
					}
				}
				break;
			//se e' del tipo troncasi
			case 't':
				l=strlen(frammento_src);
				l_enzima=strlen(nome_enzima);
				n=nome_enzima[l_enzima-1]-'0';
				//se il frammento ha lunghezza almeno n
				if (l>=n){
					nuovo_fr=malloc((l-n)*sizeof(char));
					//verifico allocazione
					if (nuovo_fr != NULL){
						for (i=0; i<(l-n); i++){
							nuovo_fr[i]=frammento_src[i];
						}
					}
				}
				break;
			//se e' del tipo prefissasi
			case 'p':
				l=strlen(frammento_src);
				l_enzima=strlen(nome_enzima);
				n=nome_enzima[l_enzima-1]-'0';
				j=n;
				nuovo_fr=malloc(l-n*sizeof(char));
				//verifico allocazione
				if (nuovo_fr != NULL){
					//se il framento ha lunghezza superiore a n
					if (l>n){
						for (i=0; i<(l-n); i++){
							nuovo_fr[i]=frammento_src[j];
							j++;
						}
					}
					if (l==n)
						return NULL;
				}



				break;
			}

		}
		x=x->next;
	}

	return nuovo_fr;
}

//creo una nuova tabella passandogli la testa della tabella e i dati di ogni frammento
t_istante *crea_istante_zero( t_istante *t, t_frammento *fr){
	t_istante *nuovo_is;
	nuovo_is=malloc(sizeof(t_istante));
	nuovo_is->costo=0;
	nuovo_is->fr=fr;
	nuovo_is->en=NULL;
	nuovo_is->padre=t;
	nuovo_is->figlio=NULL;
	nuovo_is->fratello=NULL;
	nuovo_is->istante=0;
	if (t!=NULL)
		t->fratello=nuovo_is;
	return nuovo_is;
}


//creo l'esperimento
t_esperimento *crea_esperimento (t_provetta *pr, t_tabella_costo *tc){
	t_esperimento *esp;
	t_istante *ist;
	t_frammento *p=pr->fr;
	esp=malloc(sizeof(t_esperimento));
	esp->provetta=pr;
	esp->tempo=0;
	esp->tc=tc;
	esp->ist=NULL;
	esp->ist=crea_istante_zero(esp->ist, p);
	ist=esp->ist;
	//scorro i frammenti per creare tanti istanti 0 quanti sono i frammenti
	//parto dal secondo frammento perche' il primo e' gia' fatto
	for (p=p->next; p!=NULL; p=p->next){
		ist=crea_istante_zero(ist, p);
	}
	return esp;
}

t_esperimento *nuovo_esperimento(){
	t_esperimento *esp;
	esp=malloc(sizeof(t_esperimento));
	esp->provetta=NULL;
	esp->tempo=0;
	esp->tc=NULL;
	esp->ist=NULL;

	return esp;
}

t_esperimento *prepara_esperimento (char *frammenti, char *enzimi, t_tabella_costo *tc){
	t_frammento *fr, *p;
	t_enzima *en;
	t_provetta *prov;
	t_esperimento *esp;
	t_istante *ist;
	//leggo i frammenti
	fr=leggi_F(frammenti);
	//leggo gli enzimi
	en=leggi_E(enzimi, tc);
	//creo la provetta
	prov=crea_provetta(fr, en);
	//creo l'esperimenti
	esp=nuovo_esperimento();
	p=prov->fr;
	esp->provetta=prov;
	esp->tc=tc;
	esp->ist=crea_istante_zero(esp->ist, p);
	ist=esp->ist;
	//scorro i frammenti per creare tanti istanti 0 quanti sono i frammenti
	//parto dal secondo frammento perche' il primo e' gia' fatto
	for (p=p->next; p!=NULL; p=p->next){
		ist=crea_istante_zero(ist, p);
	}
	return esp;

}

t_istante *crea_istante(t_istante *ist, t_enzima *en, t_tabella_costo *tc){
	t_istante *nuovo_is;
	t_frammento *nuovo_fr;
	nuovo_is=malloc(sizeof(t_istante));
	nuovo_is->costo=ist->costo+en->uea;
	nuovo_is->istante=ist->istante+1;
	//alloco il nuovo frammento ottenuto dal frammento ist->frammento con l'applicazione dell'enzima en
	nuovo_fr=malloc(sizeof(t_frammento));
	nuovo_fr->nucleotidi=enzima(en->nome, ist->fr->nucleotidi, tc);
	nuovo_fr->en=en;
	nuovo_fr->next=NULL;
	//faccio puntare l'istante al frammento appena creato
	nuovo_is->fr=nuovo_fr;
	nuovo_is->en=en;

	//se sto creando un figlio
	if (ist->figlio==NULL){
		nuovo_is->padre=ist;
		nuovo_is->figlio=NULL;
		nuovo_is->fratello=NULL;
		ist->figlio=nuovo_is;
	} else
	//se sto creando un fratello
	{
		nuovo_is->fratello=ist->figlio;
		nuovo_is->figlio=NULL;
		nuovo_is->padre=ist;
		ist->figlio=nuovo_is;
	}
	printf("figlio=> istante: %d - frammento: %s - enzima: %s\n", nuovo_is->istante, nuovo_is->fr->nucleotidi, en->nome);
	return nuovo_is;
}

void esegui_istante(t_istante *ist, t_enzima *enz, t_esperimento *esp){
	t_enzima *e;
	t_istante *i;
	//ciclo sugli enzimi
	for (e=enz; e!=NULL; e=e->next){
		i=crea_istante(ist, e, esp->tc);
		if (i->istante < esp->tempo)
			esegui_istante(i, enz, esp);
	}

}

void esperimento(t_esperimento *esp, int tempo){
	t_istante *i;
	esp->tempo=tempo;
	//ciclo sugli istanti perche' devo ciclare sui frammenti dell'istante 0
	for (i=esp->ist; i!=NULL; i=i->fratello){
		esegui_istante(i, esp->provetta->en, esp);
	}

}

t_lista_soluzioni *cerca_frammento (char *s, t_istante *ist, t_lista_soluzioni *lista, int delta){
	t_istante *i=NULL;
	t_lista_soluzioni *l=lista;
	//se trovo la parola, creo la lista delle soluzioni
	if (strcmp(s, ist->fr->nucleotidi)==0 && (ist->istante<=delta)){
		l=malloc(sizeof(t_lista_soluzioni));
		l->istante=ist;
		l->next=lista;
	}
	//ciclo sui fratelli
	for (i=ist->figlio; i!=NULL; i=i->fratello){
		l=cerca_frammento(s, i, l, delta);
	}
	return l;
}

char *similarita (char *s, char *g){
	char *ris1,*ris2;
	int dim=0;
	int pos=0;
	int sim1=0, sim2=0;
	int i=0, j=0;
	//verifico la lunghezza delle stringhe perche' la similarita' sara' al massimo pari alla piu' piccola stringa
	if (strlen(s)<strlen(g))
		dim=strlen(s);
	else
		dim=strlen(g);
	//dimensiono l'array risultato usando la dimensione minima
	ris1=malloc(sizeof(char)*dim);
	ris2=malloc(sizeof(char)*dim);

	//verifico quante volte g sta in s
	//ciclo sui caratteri di g
	for (i=0; i<strlen(g); i++){
		for (j=pos; j<strlen(s); j++){
			if (g[i]==s[j]){
				ris1[sim1]=g[i];
				sim1++;
				pos=j+1;
				break;
			}
		}
	}
	pos=0;
	//verifico quante volte s sta in g
	//ciclo sui caratteri di s
	for (i=0; i<strlen(s); i++){
		for (j=pos; j<strlen(g); j++){
			if (s[i]==g[j]){
				ris2[sim2]=s[i];
				sim2++;
				pos=j+1;
				break;
			}
		}
	}
	if (strlen(ris1)<strlen(ris2))
		return ris2;
	else
		return ris1;




}

int main (int argc, char *argv[]) {
	t_tabella_costo *tab=NULL;
	t_frammento *fr=NULL;
	t_enzima *en=NULL;
	t_provetta *prov=NULL;
	t_esperimento *esp=NULL;
	t_lista_soluzioni *g=NULL;
	int tempo=3;
	char *s;


	//leggo file "tabella_costi" passato al main
	tab=leggi_TC(argv[1]);
	printList_TC(tab);

	//leggo file "frammenti_file.txt" passato al main
	//fr=leggi_F(argv[2]);
	//printList_F(fr);

	//leggo file "enzimi_file.txt" passato al main
	//en=leggi_E(argv[3], tab);
	//printList_E(en);

	//creo la provetta
	//prov=crea_provetta(fr, en);



	esp=prepara_esperimento(argv[2], argv[3], tab);

	//inibisco l'enzima insertasi-2c
	en=elimina_enzima("insertasi-2c", esp->provetta->en);
	printList_E(en);

	//aggiungo o riattivo l'enzima
	aggiungi_enzima(esp->provetta, "insertasi-2c", tab);
	printList_E(esp->provetta->en);

	esperimento(esp, 3);

	g=cerca_frammento("ACC", esp->ist, g, tempo);
	printList_SOL(g);
	print_sequenza_enzimi_ric(g);
	print_sequenza_enzimi_din(g);
	print_energia_enzimi_din(g);
	s=similarita("GATTACA", "TACCA");
	printf("similarita maggiore: %s", s);
	return 1;
}
