#include<stdio.h>
#include<malloc.h>

typedef struct
{
	int codISBN;
	char* titlu;
	int nrAutori;
	float pret;
	int nrPagini;
} carte;

typedef struct
{
	int BF;
	carte inf;
	struct nodArb* stanga, * dreapta;
} nodArb;

nodArb* creareNod(carte c, nodArb* st, nodArb* dr)
{
	nodArb* nou = (nodArb*)malloc(sizeof(nodArb));
	nou->inf.codISBN = c.codISBN;
	nou->inf.titlu = (char*)malloc((strlen(c.titlu) + 1) * sizeof(char));
	strcpy(nou->inf.titlu, c.titlu);
	nou->inf.nrAutori = c.nrAutori;
	nou->inf.pret = c.pret;
	nou->inf.nrPagini = c.nrPagini;

	nou->stanga = st;
	nou->dreapta = dr;

	return nou;
}

nodArb* inserareNod(nodArb* rad, carte c)
{
	if (rad != NULL)
	{
		if (c.codISBN < rad->inf.codISBN)
		{
			rad->stanga = inserareNod(rad->stanga, c);
			return rad;
		}
		else
			if (c.codISBN > rad->inf.codISBN)
			{
				rad->dreapta = inserareNod(rad->dreapta, c);
				return rad;
			}
			else
				return rad;
	}
	else
		return creareNod(c, NULL, NULL);
}

void preordine(nodArb* rad)//RSD
{
	if (rad != NULL)
	{
		printf("\nCod ISBN=%d, Titlu=%s, Nr. autori=%d, Pret=%5.2f, Nr. pagini=%d, BF=%d",
			rad->inf.codISBN, rad->inf.titlu, rad->inf.nrAutori, rad->inf.pret, rad->inf.nrPagini, rad->BF);
		preordine(rad->stanga);
		preordine(rad->dreapta);
	}
}

void inordine(nodArb* rad)//SRD
{
	if (rad != NULL)
	{
		inordine(rad->stanga);
		printf("\nCod ISBN=%d, Titlu=%s, Nr. autori=%d, Pret=%5.2f, Nr. pagini=%d",
			rad->inf.codISBN, rad->inf.titlu, rad->inf.nrAutori, rad->inf.pret, rad->inf.nrPagini);
		inordine(rad->dreapta);
	}
}

void postordine(nodArb* rad)//SDR
{
	if (rad != NULL)
	{
		postordine(rad->stanga);
		postordine(rad->dreapta);
		printf("\nCod ISBN=%d, Titlu=%s, Nr. autori=%d, Pret=%5.2f, Nr. pagini=%d",
			rad->inf.codISBN, rad->inf.titlu, rad->inf.nrAutori, rad->inf.pret, rad->inf.nrPagini);
	}
}

void dezalocare(nodArb* rad)
{
	if (rad != NULL)
	{
		//preordine
		nodArb* st = rad->stanga;
		nodArb* dr = rad->dreapta;
		free(rad->inf.titlu);
		free(rad);
		dezalocare(st);
		dezalocare(dr);

		//postordine
		/*dezalocare(rad->stanga);
		dezalocare(rad->dreapta);
		free(rad->inf.titlu);
		free(rad);*/
	}
}

void salvareVector(nodArb* rad, carte* vect, int* nr, char litera)
{
	if (rad != NULL)
	{
		if (rad->inf.titlu[0] == litera)
		{
			vect[*nr] = rad->inf;
			(*nr)++;
		}
		salvareVector(rad->stanga, vect, nr, litera);
		salvareVector(rad->dreapta, vect, nr, litera);
	}
}

void numarCartiPretFrunza(nodArb* rad, int* nr, float pretMin, float pretMax)
{
	if (rad != NULL)
	{
		if (rad->stanga == NULL && rad->dreapta == NULL)
		{
			if (rad->inf.pret >= pretMin && rad->inf.pret <= pretMax)
				(*nr)++;
		}
		numarCartiPretFrunza(rad->stanga, nr, pretMin, pretMax);
		numarCartiPretFrunza(rad->dreapta, nr, pretMin, pretMax);
	}
}

nodArb* stergeRadacina(nodArb* rad)
{
	nodArb* aux = rad;
	if (aux->stanga != NULL)
	{
		rad = aux->stanga;
		if (aux->dreapta != NULL)
		{
			nodArb* temp = aux->stanga;
			while (temp->dreapta != NULL)
				temp = temp->dreapta;
			temp->dreapta = aux->dreapta;
		}
	}
	else
		if (aux->dreapta != NULL)
			rad = aux->dreapta;
		else
			rad = NULL;
	free(aux->inf.titlu);
	free(aux);
	return rad;
}

nodArb* stergeNod(nodArb* rad, int cheieDeSters)
{
	if (rad != NULL)
	{
		if (cheieDeSters == rad->inf.codISBN)
		{
			rad = stergeRadacina(rad);
			return rad;
		}
		else
			if (cheieDeSters < rad->inf.codISBN)
			{
				rad->stanga = stergeNod(rad->stanga, cheieDeSters);
				return rad;
			}
			else
			{
				rad->dreapta = stergeNod(rad->dreapta, cheieDeSters);
				return rad;
			}
	}
	else
		return NULL;
}

int maxim(int a, int b)
{
	int max = a;
	if (max < b)
		max = b;
	return max;
}

int inaltimeArbore(nodArb* rad)
{
	if (rad != NULL)
		return 1 + maxim(inaltimeArbore(rad->stanga), inaltimeArbore(rad->dreapta));
	else
		return 0;
}

void calculBF(nodArb* rad)
{
	if (rad != NULL)
	{
		rad->BF = inaltimeArbore(rad->dreapta) - inaltimeArbore(rad->stanga);
		calculBF(rad->stanga);
		calculBF(rad->dreapta);
	}
}

nodArb* rotatie_dreapta(nodArb* rad)
{
	printf("\n---Rotatie dreapta---\n");
	nodArb* nod1 = rad->stanga;
	rad->stanga = nod1->dreapta;
	nod1->dreapta = rad;
	rad = nod1;
	return rad;
}

nodArb* rotatie_stanga(nodArb* rad)
{
	printf("\n---Rotatie stanga---\n");
	nodArb* nod1 = rad->dreapta;
	rad->dreapta = nod1->stanga;
	nod1->stanga = rad;
	rad = nod1;
	return rad;
}

nodArb* rotatie_stanga_dreapta(nodArb* rad)
{
	printf("\n---Rotatie stanga-dreapta---\n");
	nodArb* nod1 = rad->stanga;
	nodArb* nod2 = nod1->dreapta;
	nod1->dreapta = nod2->stanga;
	nod2->stanga = nod1;
	rad->stanga = nod2->dreapta;
	nod2->dreapta = rad;
	rad = nod2;
	return rad;
}

nodArb* rotatie_dreapta_stanga(nodArb* rad)
{
	printf("\n---Rotatie dreapta-stanga---\n");
	nodArb* nod1 = rad->dreapta;
	nodArb* nod2 = nod1->stanga;
	nod1->stanga = nod2->dreapta;
	nod2->dreapta = nod1;
	rad->dreapta = nod2->stanga;
	nod2->stanga = rad;
	rad = nod2;
	return rad;
}

nodArb* reechilibrare(nodArb* rad)
{
	calculBF(rad);
	nodArb* fiuSt = rad->stanga;
	nodArb* fiuDr = rad->dreapta;
	if (rad->BF <= -2 && fiuSt->BF <= -1)
	{
		rad = rotatie_dreapta(rad);
		calculBF(rad);
	}
	else
		if (rad->BF >= 2 && fiuDr->BF >= 1)
		{
			rad = rotatie_stanga(rad);
			calculBF(rad);
		}
		else
			if (rad->BF <= -2 && fiuSt->BF >= 1)
			{
				rad = rotatie_stanga_dreapta(rad);
				calculBF(rad);
			}
			else
				if (rad->BF >= 2 && fiuDr->BF <= -1)
				{
					rad = rotatie_dreapta_stanga(rad);
					calculBF(rad);
				}
	return rad;
}

void main()
{
	nodArb* rad = NULL;
	char buffer[30];
	int nrCarti;
	carte c;

	FILE* f = fopen("fisier.txt", "r");
	fscanf(f, "%d", &nrCarti);
	for (int i = 0; i < nrCarti; i++)
	{
		fscanf(f, "%d", &c.codISBN);
		fscanf(f, "%s", buffer);
		c.titlu = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
		strcpy(c.titlu, buffer);
		fscanf(f, "%d", &c.nrAutori);
		fscanf(f, "%f", &c.pret);
		fscanf(f, "%d", &c.nrPagini);

		rad = inserareNod(rad, c);

		rad = reechilibrare(rad);

		free(c.titlu);
	}
	fclose(f);

	//calculBF(rad);

	preordine(rad);
	printf("\n-------------------\n");
	preordine(rad->stanga);
	printf("\n-------------------\n");
	preordine(rad->dreapta);

	printf("\nInaltime arbore = %d", inaltimeArbore(rad));

	printf("\n---------Vector----------\n");
	carte* vect = (carte*)malloc(nrCarti * sizeof(carte));
	int nr = 0;
	salvareVector(rad, vect, &nr, 'P');
	for(int i=0;i<nr;i++)
		printf("\nCod ISBN=%d, Titlu=%s, Nr. autori=%d, Pret=%5.2f, Nr. pagini=%d",
			vect[i].codISBN, vect[i].titlu, vect[i].nrAutori, vect[i].pret, vect[i].nrPagini);
	free(vect);

	printf("\n-------------------\n");
	int nrFrunza = 0;
	numarCartiPretFrunza(rad, &nrFrunza, 39.0f, 45.0f);
	printf("\nNr. carti noduri frunza=%d", nrFrunza);

	printf("\n----------stergere---------\n");
	rad = stergeRadacina(rad);
	//rad = stergeNod(rad, 500);

	//calculBF(rad);

	rad = reechilibrare(rad);

	preordine(rad);

	dezalocare(rad);
}
