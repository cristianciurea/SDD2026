#include<stdio.h>
#include<malloc.h>

typedef struct
{
	int varsta;
	char* nume;
	float medie;
} student;

typedef struct
{
	student** vect;
	int nrElem;
} hashT;

int functieHash(int cheie, hashT tabela)
{
	return cheie % tabela.nrElem;
}

void inserareTabela(hashT tabela, student* s)
{
	int poz = functieHash((*s).varsta, tabela);
	if (tabela.vect[poz] == NULL)
		tabela.vect[poz] = s;
	else
	{
		//coliziune
		int index = 1;
		int vb = 0;
		while (poz + index < tabela.nrElem)
		{
			if (tabela.vect[poz + index] == NULL)
			{
				vb = 1;
				poz += index;
				tabela.vect[poz] = s;
				break;
			}
			else
				index++;
		}
		if (vb == 0)
		{
			index = 1;
			while (poz - index >= 0)
			{
				if (tabela.vect[poz - index] == NULL)
				{
					poz -= index;
					tabela.vect[poz] = s;
					break;
				}
				else
					index++;
			}
		}
	}
}

void traversareTabela(hashT tabela)
{
	for(int i=0;i<tabela.nrElem;i++)
		if (tabela.vect[i] != NULL)
		{
			printf("\nPozitie=%d", i);
			printf("\nVarsta = %d, Nume = %s, Medie = %5.2f",
				tabela.vect[i]->varsta, tabela.vect[i]->nume,
				tabela.vect[i]->medie);
		}
}

void dezalocareTabela(hashT tabela)
{
	for(int i=0;i<tabela.nrElem;i++)
		if (tabela.vect[i] != NULL)
		{
			free(tabela.vect[i]->nume);
			free(tabela.vect[i]);
		}
	free(tabela.vect);
}

void stergereTabela(hashT tabela, int varsta)
{
	int poz = functieHash(varsta, tabela);
	if (tabela.vect[poz] != NULL && tabela.vect[poz]->varsta == varsta)
	{
		free(tabela.vect[poz]->nume);
		free(tabela.vect[poz]);
		tabela.vect[poz] = NULL;
	}
	else
	{
		int index = 1;
		int vb = 0;
		while (poz + index < tabela.nrElem)
		{
			if (tabela.vect[poz + index] != NULL &&
				tabela.vect[poz+index]->varsta==varsta)
			{
				vb = 1;
				poz += index;
				free(tabela.vect[poz]->nume);
				free(tabela.vect[poz]);
				tabela.vect[poz] = NULL;
				break;
			}
			else
				index++;
		}
		if (vb == 0)
		{
			index = 1;
			while (poz - index >= 0)
			{
				if (tabela.vect[poz - index] != NULL
					&&tabela.vect[poz-index]->varsta==varsta)
				{
					poz -= index;
					free(tabela.vect[poz]->nume);
					free(tabela.vect[poz]);
					tabela.vect[poz] = NULL;
					break;
				}
				else
					index++;
			}
		}
	}
}

void main()
{
	int nrStud;
	student* s;
	char buffer[20];

	hashT tabela;

	FILE* f = fopen("fisier.txt", "r");
	fscanf(f, "%d", &nrStud);
	tabela.nrElem = 4 * nrStud + 3;
	tabela.vect = (student**)malloc(tabela.nrElem * sizeof(student*));
	for (int i = 0; i < tabela.nrElem; i++)
		tabela.vect[i] = NULL;
	for (int i = 0; i < nrStud; i++)
	{
		s = (student*)malloc(sizeof(student));
		fscanf(f, "%d", &s->varsta);
		fscanf(f, "%s", buffer);
		s->nume = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
		strcpy(s->nume, buffer);
		fscanf(f, "%f", &s->medie);

		inserareTabela(tabela, s);
	}
	fclose(f);

	traversareTabela(tabela);

	printf("\n---------------------dupa stergere---------\n");
	stergereTabela(tabela, 26);

	traversareTabela(tabela);

	dezalocareTabela(tabela);
}