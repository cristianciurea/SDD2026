#include<stdio.h>
#include<malloc.h>

typedef struct
{
	int cod;
	char* denumire;
	char* categorie;
	float pret;
} produs;

//structura nod lista secundara
typedef struct
{
	produs inf;
	struct nodLS* next;
} nodLS;

//structura tabela dispersie
typedef struct
{
	struct nodLS** vect;
	int nrElem;
} hashT;

//initializare nod lista secundara
nodLS* initializareNodLS(produs p)
{
	nodLS* nou = (nodLS*)malloc(sizeof(nodLS));
	nou->inf.cod = p.cod;
	nou->inf.denumire = (char*)malloc((strlen(p.denumire) + 1) * sizeof(char));
	strcpy(nou->inf.denumire, p.denumire);
	nou->inf.categorie = (char*)malloc((strlen(p.categorie) + 1) * sizeof(char));
	strcpy(nou->inf.categorie, p.categorie);
	nou->inf.pret = p.pret;
	nou->next = NULL;
	return nou;
}

//inserare nod in lista secundara
void inserareLS(nodLS** capLS, produs p)
{
	nodLS* nou = initializareNodLS(p);
	if (*capLS == NULL)
		*capLS = nou;
	else
	{
		nodLS* temp = *capLS;
		while (temp->next)
			temp = temp->next;
		temp->next = nou;
	}
}

int functieHash1(int cheie, hashT tabela)
{
	return cheie % tabela.nrElem;
}

int functieHash2(char categ[20], hashT tabela)
{
	return categ[0] % tabela.nrElem;
}

//inserara tabela dispersie
void inserareTabela(hashT tabela, produs p)
{
	//int poz = functieHash1(p.cod, tabela);
	int poz = functieHash2(p.categorie, tabela);
	inserareLS(&tabela.vect[poz], p);
}

//traversare lista secundara
void traversareLS(nodLS* capLS)
{
	nodLS* temp = capLS;
	while (temp)
	{
		printf("\nCod=%d, Denumire=%s, Categorie=%s, Pret=%5.2f",
			temp->inf.cod, temp->inf.denumire, temp->inf.categorie,
			temp->inf.pret);
		temp = temp->next;
	}
}

//traversare tabela dispersie
void travsersareTabela(hashT tabela)
{
	for(int i=0;i<tabela.nrElem;i++)
		if (tabela.vect[i] != NULL)
		{
			printf("\nPozitie: %d", i);
			traversareLS(tabela.vect[i]);
		}
}

//dezalocare lista secundara
void dezalocareLS(nodLS* capLS)
{
	nodLS* temp = capLS;
	while (temp)
	{
		nodLS* temp2 = temp->next;
		free(temp->inf.denumire);
		free(temp->inf.categorie);
		free(temp);
		temp = temp2;
	}
}

//dezalocare tabela dispersie
void dezalocareTabela(hashT tabela)
{
	for (int i = 0; i < tabela.nrElem; i++)
		if (tabela.vect[i] != NULL)
			dezalocareLS(tabela.vect[i]);
	free(tabela.vect);
}

void main()
{
	int nrProd;
	produs p;
	char buffer[20];
	hashT tabela;

	FILE* f = fopen("fisier.txt", "r");
	fscanf(f, "%d", &nrProd);
	tabela.nrElem = 4 * nrProd + 3;
	tabela.vect = (nodLS**)malloc(tabela.nrElem * sizeof(nodLS*));
	for (int i = 0; i < tabela.nrElem; i++)
		tabela.vect[i] = NULL;

	for (int i = 0; i < nrProd; i++)
	{
		fscanf(f, "%d", &p.cod);
		fscanf(f, "%s", buffer);
		p.denumire = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
		strcpy(p.denumire, buffer);
		fscanf(f, "%s", buffer);
		p.categorie = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
		strcpy(p.categorie, buffer);
		fscanf(f, "%f", &p.pret);

		inserareTabela(tabela, p);

		free(p.denumire);
		free(p.categorie);
	}
	fclose(f);

	travsersareTabela(tabela);

	dezalocareTabela(tabela);
}

