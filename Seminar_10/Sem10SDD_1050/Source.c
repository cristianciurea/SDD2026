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
		printf("\nCod ISBN=%d, Titlu=%s, Nr. autori=%d, Pret=%5.2f, Nr. pagini=%d",
			rad->inf.codISBN, rad->inf.titlu, rad->inf.nrAutori, rad->inf.pret, rad->inf.nrPagini);
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

		free(c.titlu);
	}
	fclose(f);

	preordine(rad);
	printf("\n-------------------\n");
	preordine(rad->stanga);
	printf("\n-------------------\n");
	preordine(rad->dreapta);

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

	dezalocare(rad);
}
