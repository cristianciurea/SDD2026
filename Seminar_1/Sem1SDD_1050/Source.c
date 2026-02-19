#include<stdio.h>
#include<malloc.h>

typedef struct
{
	int *cod;
	char* denumire;
	float pret;
	float cantitate;
} produs;

void citire4Vectori(int* coduri, char** denumiri,
	float* preturi, float* cantitati, int nr)
{
	char buffer[20];
	for (int i = 0; i < nr; i++)
	{
		printf("Cod=");
		scanf("%d", &coduri[i]);
		printf("Denumire=");
		scanf("%s", buffer);
		denumiri[i] = (char*)malloc((strlen(buffer) + 1) * 
			sizeof(char));
		strcpy(denumiri[i], buffer);
		printf("Pret=");
		scanf("%f", &preturi[i]);
		printf("Cantitate=");
		scanf("%f", &cantitati[i]);
	}
}

void citireMatrice(float **mat, char** denumiri, int nr)
{
	char buffer[20];
	for (int i = 0; i < nr; i++)
	{
		printf("Cod=");
		scanf("%f", &mat[i][0]);
		printf("Denumire=");
		scanf("%s", buffer);
		denumiri[i] = (char*)malloc((strlen(buffer) + 1) *
			sizeof(char));
		strcpy(denumiri[i], buffer);
		printf("Pret=");
		scanf("%f", &mat[i][1]);
		printf("Cantitate=");
		scanf("%f", &mat[i][2]);
	}
}

void citireVectorProduse(FILE *f, produs **vp, int nr)
{
	char buffer[20];
	for (int i = 0; i < nr; i++)
	{
		vp[i] = (produs*)malloc(sizeof(produs));
		//printf("Cod=");
		vp[i]->cod = (int*)malloc(sizeof(int));
		fscanf(f, "%d", vp[i]->cod);
		//printf("Denumire=");
		fscanf(f, "%s", buffer);
		vp[i]->denumire = (char*)malloc((strlen(buffer) + 1) *
			sizeof(char));
		strcpy(vp[i]->denumire, buffer);
		//printf("Pret=");
		fscanf(f, "%f", &vp[i]->pret);
		//printf("Cantitate=");
		fscanf(f, "%f", &vp[i]->cantitate);
	}
}

void afisare4Vectori(int* coduri, char** denumiri,
	float* preturi, float* cantitati, int nr)
{
	for (int i = 0; i < nr; i++)
		printf("\nCod=%d, Denumire=%s, Pret=%5.2f, Cantitate=%5.2f",
			coduri[i], denumiri[i], preturi[i], cantitati[i]);
}

void afisareVectorProduse(produs **vp, int nr)
{
	for (int i = 0; i < nr; i++)
		printf("\nCod=%d, Denumire=%s, Pret=%5.2f, Cantitate=%5.2f",
			*(vp[i]->cod), vp[i]->denumire, vp[i]->pret, vp[i]->cantitate);
}

void afisareMatrice(float** mat, char** denumiri, int nr)
{
	for (int i = 0; i < nr; i++)
		printf("\nCod=%5.2f, Denumire=%s, Pret=%5.2f, Cantitate=%5.2f",
			mat[i][0], denumiri[i], mat[i][1], mat[i][2]);
}

void dezalocare4Vectori(int* coduri, char** denumiri,
	float* preturi, float* cantitati, int nr)
{
	for (int i = 0; i < nr; i++)
		free(denumiri[i]);
	free(coduri);
	free(denumiri);
	free(preturi);
	free(cantitati);
}

void dezalocareMatrice(float** mat, char** denumiri, int nr)
{
	for (int i = 0; i < nr; i++)
	{
		free(mat[i]);
		free(denumiri[i]);
	}
	free(mat);
	free(denumiri);
}

void dezalocareVectorProduse(produs** vp, int nr)
{
	for (int i = 0; i < nr; i++)
	{
		free(vp[i]->cod);
		free(vp[i]->denumire);
		free(vp[i]);
	}
	free(vp);
}

float sumaTotalaProduse(produs** vp, int nr)
{
	float suma = 0.0f;
	for (int i = 0; i < nr; i++)
		suma += vp[i]->pret * vp[i]->cantitate;
	return suma;
}

void main()
{
	int nrProd;
	FILE* f = fopen("fisier.txt", "r");
	//printf("Nr. produse=");
	fscanf(f, "%d", &nrProd);
	/*int* coduri = (int*)malloc(nrProd * sizeof(int));
	char** denumiri = (char**)malloc(nrProd * sizeof(char*));
	float* preturi = (float*)malloc(nrProd * sizeof(float));
	float* cantitati = (float*)malloc(nrProd * sizeof(float));

	citire4Vectori(coduri, denumiri, preturi, cantitati, nrProd);
	afisare4Vectori(coduri, denumiri, preturi, cantitati, nrProd);
	dezalocare4Vectori(coduri, denumiri, preturi, cantitati, nrProd);*/

	/*float** mat = (float**)malloc(nrProd * sizeof(float*));
	for (int i = 0; i < nrProd; i++)
		mat[i] = (float*)malloc(3 * sizeof(float));
	char** denumiri = (char**)malloc(nrProd * sizeof(char*));
	citireMatrice(mat, denumiri, nrProd);
	afisareMatrice(mat, denumiri, nrProd);
	dezalocareMatrice(mat, denumiri, nrProd);*/

	produs** vp = (produs**)malloc(nrProd * sizeof(produs*));
	citireVectorProduse(f, vp, nrProd);
	fclose(f);
	afisareVectorProduse(vp, nrProd);
	printf("\nSuma totala = %5.2f", sumaTotalaProduse(vp, nrProd));

	dezalocareVectorProduse(vp, nrProd);
}