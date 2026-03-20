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

//nod de lista principala
typedef struct
{
	struct nodLS* inf;
	struct nodLP* next;
} nodLP;

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

//inserare nodLP
void inserareLP(nodLP** capLP, nodLS* capLS)
{
	nodLP* nou = (nodLP*)malloc(sizeof(nodLP));
	nou->inf = capLS;
	nou->next = NULL;
	if (*capLP == NULL)
		*capLP = nou;
	else
	{
		nodLP* temp = *capLP;
		while (temp->next)
			temp = temp->next;
		temp->next = nou;
	}
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

//traversare LP
void traversareLP(nodLP* capLP)
{
	nodLP* temp = capLP;
	int i = 1;
	while (temp)
	{
		printf("\nSublista: %d", i);
		traversareLS(temp->inf);
		temp = temp->next;
		i++;
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

//dezalocare LP
void dezalocareLP(nodLP* capLP)
{
	nodLP* temp = capLP;
	while (temp)
	{
		nodLP* temp2 = temp->next;
		dezalocareLS(temp->inf);
		free(temp);
		temp = temp2;
	}
}

void salvareVectorVarsta(nodLP* capLP, student* vect, int* nr, int valVarsta)
{
	nodLP* temp = capLP;
	while (temp)
	{
		nodLS* aux = temp->inf;
		while (aux)
		{
			if (aux->inf.varsta == valVarsta)
			{
				vect[*nr] = aux->inf;
				(*nr)++;
			}
			aux = aux->next;
		}
		temp = temp->next;
	}
}

void main()
{
	int nrStud;
	student s;
	char buffer[20];
	nodLP* capLP = NULL;
	nodLS* capLSF = NULL, * capLSM = NULL, * capLSN = NULL;

	FILE* f = fopen("fisier.txt", "r");
	fscanf(f, "%d", &nrStud);
	for (int i = 0; i < nrStud; i++)
	{
		fscanf(f, "%d", &s.varsta);
		fscanf(f, "%s", buffer);
		s.nume = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
		strcpy(s.nume, buffer);
		fscanf(f, " %c", &s.sex);
		fscanf(f, "%f", &s.medie);
		if (s.sex == 'F')
			inserareLS(&capLSF, s);
		else
			if (s.sex == 'M')
				inserareLS(&capLSM, s);
			else
				inserareLS(&capLSN, s);
		free(s.nume);
	}
	fclose(f);

	inserareLP(&capLP, capLSF);
	inserareLP(&capLP, capLSM);
	inserareLP(&capLP, capLSN);

	traversareLP(capLP);

	printf("\n----------------------\n");
	student* vect = (student*)malloc(nrStud * sizeof(student));
	int nr = 0;
	salvareVectorVarsta(capLP, vect, &nr, 21);
	for(int i=0;i<nr;i++)
		printf("\nVarsta=%d, Nume=%s, Sex=%c, Medie=%5.2f",
			vect[i].varsta, vect[i].nume, vect[i].sex, vect[i].medie);
	free(vect);

	dezalocareLP(capLP);
}