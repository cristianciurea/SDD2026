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
	int BF;
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
		printf("\nCod VIN=%d, Marca=%s, Motorizare=%s, Pret=%5.2f, Nr. km=%d, BF = %d",
			rad->inf.codVIN, rad->inf.marca, rad->inf.motorizare, rad->inf.pret, rad->inf.nrKm, rad->BF);
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
	free(aux->inf.marca);
	free(aux->inf.motorizare);
	free(aux);
	return rad;
}

nodArb* stergeNod(nodArb* rad, int cheieSters)
{
	if (rad != NULL)
	{
		if (cheieSters == rad->inf.codVIN)
		{
			rad = stergeRadacina(rad);
			return rad;
		}
		else
			if (cheieSters < rad->inf.codVIN)
			{
				rad->stanga = stergeNod(rad->stanga, cheieSters);
				return rad;
			}
			else
			{
				rad->dreapta = stergeNod(rad->dreapta, cheieSters);
				return rad;
			}
	}
	else
		return NULL;
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
	printf("\n----Rotatie dreapta---------\n");
	nodArb* nod1 = rad->stanga;
	rad->stanga = nod1->dreapta;
	nod1->dreapta = rad;
	rad = nod1;
	return rad;
}

nodArb* rotatie_stanga(nodArb* rad)
{
	printf("\n----Rotatie stanga---------\n");
	nodArb* nod1 = rad->dreapta;
	rad->dreapta = nod1->stanga;
	nod1->stanga = rad;
	rad = nod1;
	return rad;
}

nodArb* rotatie_stanga_dreapta(nodArb* rad)
{
	printf("\n----Rotatie stanga-dreapta---------\n");
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
	printf("\n----Rotatie dreapta-stanga---------\n");
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

		rad = reechilibrare(rad);

		free(m.marca);
		free(m.motorizare);
	}
	fclose(f);

	calculBF(rad);

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

	printf("\n-----Stergere------\n");
	rad = stergeRadacina(rad);
	//rad = stergeNod(rad, 500);

	//calculBF(rad);

	rad = reechilibrare(rad);

	preordine(rad);

	dezalocare(rad);
}