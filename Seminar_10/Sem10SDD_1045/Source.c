#include<stdio.h>
#include<malloc.h>

typedef struct
{
	int codVIN;
	char* marca;
	char* motorizare;
	float pret;
	int nrKm;
} masina;

typedef struct
{
	masina inf;
	struct nodArb* stanga, * dreapta;
} nodArb;

nodArb* creareNod(masina m, nodArb* st, nodArb* dr)
{
	nodArb* nou = (nodArb*)malloc(sizeof(nodArb));
	nou->inf.codVIN = m.codVIN;
	nou->inf.marca = (char*)malloc((strlen(m.marca) + 1) * sizeof(char));
	strcpy(nou->inf.marca, m.marca);
	nou->inf.motorizare = (char*)malloc((strlen(m.motorizare) + 1) * sizeof(char));
	strcpy(nou->inf.motorizare, m.motorizare);
	nou->inf.pret = m.pret;
	nou->inf.nrKm = m.nrKm;
	nou->stanga = st;
	nou->dreapta = dr;
	return nou;
}

nodArb* inserareNod(nodArb* rad, masina m)
{
	if (rad != NULL)
	{
		if (m.codVIN < rad->inf.codVIN)
		{
			rad->stanga = inserareNod(rad->stanga, m);
			return rad;
		}
		else
			if (m.codVIN > rad->inf.codVIN)
			{
				rad->dreapta = inserareNod(rad->dreapta, m);
				return rad;
			}
			else
				return rad;
	}
	else
		return creareNod(m, NULL, NULL);
}

void preordine(nodArb* rad)//RSD
{
	if (rad != NULL)
	{
		printf("\nCod VIN=%d, Marca=%s, Motorizare=%s, Pret=%5.2f, Nr. km=%d",
			rad->inf.codVIN, rad->inf.marca, rad->inf.motorizare, rad->inf.pret, rad->inf.nrKm);
		preordine(rad->stanga);
		preordine(rad->dreapta);
	}
}

void inordine(nodArb* rad)//SRD
{
	if (rad != NULL)
	{
		inordine(rad->stanga);
		printf("\nCod VIN=%d, Marca=%s, Motorizare=%s, Pret=%5.2f, Nr. km=%d",
			rad->inf.codVIN, rad->inf.marca, rad->inf.motorizare, rad->inf.pret, rad->inf.nrKm);
		inordine(rad->dreapta);
	}
}

void postordine(nodArb* rad)//SDR
{
	if (rad != NULL)
	{
		postordine(rad->stanga);
		postordine(rad->dreapta);
		printf("\nCod VIN=%d, Marca=%s, Motorizare=%s, Pret=%5.2f, Nr. km=%d",
			rad->inf.codVIN, rad->inf.marca, rad->inf.motorizare, rad->inf.pret, rad->inf.nrKm);
	}
}

void dezalocare(nodArb* rad)
{
	if (rad != NULL)
	{
		//preordine
		/*nodArb* st = rad->stanga;
		nodArb* dr = rad->dreapta;
		free(rad->inf.marca);
		free(rad->inf.motorizare);
		free(rad);
		dezalocare(st);
		dezalocare(dr);*/

		//postordine
		dezalocare(rad->stanga);
		dezalocare(rad->dreapta);
		free(rad->inf.marca);
		free(rad->inf.motorizare);
		free(rad);
	}
}

void salvareVectorBENZINA(nodArb* rad, masina* vect, int* nr, char* motorizare)
{
	if (rad != NULL)
	{
		if (strcmp(rad->inf.motorizare, motorizare) == 0)
		{
			vect[*nr] = rad->inf;
			(*nr)++;
		}
		salvareVectorBENZINA(rad->stanga, vect, nr, motorizare);
		salvareVectorBENZINA(rad->dreapta, vect, nr, motorizare);
	}
}

void nrMasiniHibrideFrunza(nodArb* rad, int* nr, char* motorizare)
{
	if (rad != NULL)
	{
		if (rad->stanga == NULL && rad->dreapta == NULL)
		{
			if (strcmp(rad->inf.motorizare, motorizare) == 0)
				(*nr)++;
		}
		nrMasiniHibrideFrunza(rad->stanga, nr, motorizare);
		nrMasiniHibrideFrunza(rad->dreapta, nr, motorizare);
	}
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
		return 1 + maxim(inaltimeArbore(rad->stanga),
			inaltimeArbore(rad->dreapta));
	else
		return 0;
}

void main()
{
	nodArb* rad = NULL;
	char buffer[20];
	int nrMasini;
	masina m;

	FILE* f = fopen("fisier.txt", "r");
	fscanf(f, "%d", &nrMasini);
	for (int i = 0; i < nrMasini; i++)
	{
		fscanf(f, "%d", &m.codVIN);
		fscanf(f, "%s", buffer);
		m.marca = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
		strcpy(m.marca, buffer);
		fscanf(f, "%s", buffer);
		m.motorizare = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
		strcpy(m.motorizare, buffer);
		fscanf(f, "%f", &m.pret);
		fscanf(f, "%d", &m.nrKm);

		rad = inserareNod(rad, m);

		free(m.marca);
		free(m.motorizare);
	}
	fclose(f);

	preordine(rad);
	printf("\n-----------\n");
	preordine(rad->stanga);
	printf("\n-----------\n");
	preordine(rad->dreapta);

	printf("\n-----Vector masini------\n");
	int nr = 0;
	masina* vect = (masina*)malloc(nrMasini * sizeof(masina));
	salvareVectorBENZINA(rad, vect, &nr, "HYBRID");
	for(int i=0;i<nr;i++)
		printf("\nCod VIN=%d, Marca=%s, Motorizare=%s, Pret=%5.2f, Nr. km=%d",
			vect[i].codVIN, vect[i].marca, vect[i].motorizare, vect[i].pret, vect[i].nrKm);
	free(vect);

	printf("\n-----Nr. masini------\n");
	int nrFrunza = 0;
	nrMasiniHibrideFrunza(rad, &nrFrunza, "HYBRID");
	printf("\nNr. masini hibride frunza = %d", nrFrunza);

	printf("\nInaltime arbore = %d", inaltimeArbore(rad));

	dezalocare(rad);
}