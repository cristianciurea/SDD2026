#include<stdio.h>
#include<malloc.h>

typedef struct
{
	int varsta;
	char* nume;
	char sex;
	float medie;
} student;

//nod de lista secundara
typedef struct
{
	student inf;
	struct nodLS* next;
} nodLS;

//structura tabela dispersie
typedef struct
{
	struct nodLS** vect;
	int nrElem;
} hashT;

int functieHash1(int cheie, hashT tabela)
{
	return cheie % tabela.nrElem;
}

int functieHash2(char nume[20], hashT tabela)
{
	return nume[0] % tabela.nrElem;
}

//initializare nodLS
nodLS* initializareNodLS(student s)
{
	nodLS* nou = (nodLS*)malloc(sizeof(nodLS));
	nou->inf.varsta = s.varsta;
	nou->inf.nume = (char*)malloc((strlen(s.nume) + 1) * sizeof(char));
	strcpy(nou->inf.nume, s.nume);
	nou->inf.sex = s.sex;
	nou->inf.medie = s.medie;
	nou->next = NULL;
	return nou;
}

//inserare nodLS
void inserareLS(nodLS** capLS, student s)
{
	nodLS* nou = initializareNodLS(s);
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

//inserare tabela dispersie
void inserareTabela(hashT tabela, student s)
{
	//int poz = functieHash1(s.varsta, tabela);
	int poz = functieHash2(s.nume, tabela);
	inserareLS(&tabela.vect[poz], s);
}

//traversare LS
void traversareLS(nodLS* capLS)
{
	nodLS* temp = capLS;
	while (temp)
	{
		printf("\nVarsta=%d, Nume=%s, Sex=%c, Medie=%5.2f",
			temp->inf.varsta, temp->inf.nume, temp->inf.sex, temp->inf.medie);
		temp = temp->next;
	}
}

//traversare tabela dispersie
void traversareTabela(hashT tabela)
{
	for(int i=0;i<tabela.nrElem;i++)
		if (tabela.vect[i] != NULL)
		{
			printf("\nPozitie: %d", i);
			traversareLS(tabela.vect[i]);
		}
}

//dezalocare LS
void dezalocareLS(nodLS* capLS)
{
	nodLS* temp = capLS;
	while (temp)
	{
		nodLS* temp2 = temp->next;
		free(temp->inf.nume);
		free(temp);
		temp = temp2;
	}
}

//dezalocare tabela
void dezalocareTabela(hashT tabela)
{
	for (int i = 0; i < tabela.nrElem; i++)
		if (tabela.vect[i])
			dezalocareLS(tabela.vect[i]);
	free(tabela.vect);
}

void main()
{
	int nrStud;
	student s;
	char buffer[20];
	hashT tabela;

	FILE* f = fopen("fisier.txt", "r");
	fscanf(f, "%d", &nrStud);
	tabela.nrElem = 4 * nrStud + 3;
	tabela.vect = (nodLS**)malloc(tabela.nrElem * sizeof(nodLS*));
	for (int i = 0; i < tabela.nrElem; i++)
		tabela.vect[i] = NULL;

	for (int i = 0; i < nrStud; i++)
	{
		fscanf(f, "%d", &s.varsta);
		fscanf(f, "%s", buffer);
		s.nume = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
		strcpy(s.nume, buffer);
		fscanf(f, " %c", &s.sex);
		fscanf(f, "%f", &s.medie);
		
		inserareTabela(tabela, s);

		free(s.nume);
	}
	fclose(f);

	traversareTabela(tabela);

	
	dezalocareTabela(tabela);
}