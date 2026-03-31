#include<stdio.h>
#include<malloc.h>

typedef struct
{
	int prioritate;
	char* denumire;
	float suma;
} cheltuiala;

typedef struct
{
	cheltuiala* vect;
	int nrElem;
} heap;

void filtrare(heap h, int index)
{
	int indexRad = index;
	int indexSt = 2 * index + 1;
	int indexDr = 2 * index + 2;

	if (indexSt < h.nrElem && h.vect[indexSt].prioritate<
		h.vect[indexRad].prioritate)
		indexRad = indexSt;

	if (indexDr < h.nrElem && h.vect[indexDr].prioritate<
		h.vect[indexRad].prioritate)
		indexRad = indexDr;

	if (index != indexRad)
	{
		cheltuiala aux = h.vect[index];
		h.vect[index] = h.vect[indexRad];
		h.vect[indexRad] = aux;

		filtrare(h, indexRad);
	}
}

void afisareHeap(heap h)
{
	for (int i = 0; i < h.nrElem; i++)
		printf("\nPrioritate = %d, Denumire = %s, Suma = %5.2f",
			h.vect[i].prioritate, h.vect[i].denumire, h.vect[i].suma);
}

void dezalocareHeap(heap h)
{
	for (int i = 0; i < h.nrElem; i++)
		free(h.vect[i].denumire);
	free(h.vect);
}

heap inserare(heap h, cheltuiala noua)
{
	cheltuiala* vectNou = (cheltuiala*)malloc((h.nrElem + 1) * sizeof(cheltuiala));
	for (int i = 0; i < h.nrElem; i++)
		vectNou[i] = h.vect[i];
	vectNou[h.nrElem] = noua;
	h.nrElem++;
	free(h.vect);
	h.vect = vectNou;

	for (int i = (h.nrElem - 1) / 2; i >= 0; i--)
		filtrare(h, i);

	return h;
}

heap extragere(heap h, cheltuiala* extrasa)
{
	cheltuiala* vectNou = (cheltuiala*)malloc((h.nrElem - 1) * sizeof(cheltuiala));
	cheltuiala aux = h.vect[0];
	h.vect[0] = h.vect[h.nrElem - 1];
	h.vect[h.nrElem - 1] = aux;

	*extrasa = h.vect[h.nrElem - 1];

	h.nrElem--;
	for (int i = 0; i < h.nrElem; i++)
		vectNou[i] = h.vect[i];
	free(h.vect);
	h.vect = vectNou;

	for (int i = (h.nrElem - 1) / 2; i >= 0; i--)
		filtrare(h, i);

	return h;
}

void main()
{
	heap h;
	char buffer[20];

	FILE* f = fopen("fisier.txt", "r");
	fscanf(f, "%d", &h.nrElem);
	h.vect = (cheltuiala*)malloc(h.nrElem * sizeof(cheltuiala));
	for (int i = 0; i < h.nrElem; i++)
	{
		fscanf(f, "%d", &h.vect[i].prioritate);
		fscanf(f, "%s", buffer);
		h.vect[i].denumire = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
		strcpy(h.vect[i].denumire, buffer);
		fscanf(f, "%f", &h.vect[i].suma);
	}
	fclose(f);

	for (int i = (h.nrElem - 1) / 2; i >= 0; i--)
		filtrare(h, i);

	afisareHeap(h);

	printf("\n--------------------\n");

	/*cheltuiala noua;
	printf("Prioritate=");
	scanf("%d", &noua.prioritate);
	printf("Denumire=");
	scanf("%s", buffer);
	noua.denumire = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
	strcpy(noua.denumire, buffer);
	printf("Suma=");
	scanf("%f", &noua.suma);*/

	//h = inserare(h, noua);

	//afisareHeap(h);

	printf("\n--------------------\n");

	cheltuiala extrasa;
	h = extragere(h, &extrasa);
	printf("\nExtras -> Prioritate = %d, Denumire = %s, Suma = %5.2f",
		extrasa.prioritate, extrasa.denumire, extrasa.suma);

	afisareHeap(h);

	printf("\n--------------------\n");
	cheltuiala* vectSortat = (cheltuiala*)malloc(h.nrElem * sizeof(cheltuiala));
	int nr = h.nrElem;
	for (int i = 0; i < nr; i++)
	{
		h = extragere(h, &extrasa);
		vectSortat[i] = extrasa;
	}
	for (int i = 0; i < nr; i++)
		printf("\nSortat -> Prioritate = %d, Denumire = %s, Suma = %5.2f",
			vectSortat[i].prioritate, vectSortat[i].denumire, 
			vectSortat[i].suma);

	for (int i = 0; i < nr; i++)
		free(vectSortat[i].denumire);
	free(vectSortat);

	//dezalocareHeap(h);
}