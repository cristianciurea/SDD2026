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

//structura nod lista principala
typedef struct
{
	struct nodLS* inf;
	struct nodLP* next;
} nodLP;

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

//inserare nod in lista principala
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

//traversare lista principala
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

//dezalocare lista principala
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

void salvareProduseVector(nodLP* capLP, produs* vect, int* nr, float pretPrag)
{
	nodLP* temp = capLP;
	while (temp)
	{
		nodLS* aux = temp->inf;
		while (aux)
		{
			if (aux->inf.pret >= pretPrag)
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
	int nrProd;
	produs p;
	char buffer[20];
	nodLP* capLP = NULL;
	nodLS* capLS1 = NULL, * capLS2 = NULL, * capLS3 = NULL;

	FILE* f = fopen("fisier.txt", "r");
	fscanf(f, "%d", &nrProd);
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

		if (strcmp(p.categorie, "electronice") == 0)
			inserareLS(&capLS1, p);
		else
			if (strcmp(p.categorie, "bauturi") == 0)
				inserareLS(&capLS2, p);
			else
				if (strcmp(p.categorie, "lactate") == 0)
					inserareLS(&capLS3, p);

		free(p.denumire);
		free(p.categorie);
	}
	fclose(f);

	inserareLP(&capLP, capLS1);
	inserareLP(&capLP, capLS2);
	inserareLP(&capLP, capLS3);

	traversareLP(capLP);

	printf("\n--------------------------\n");
	produs* vect = (produs*)malloc(nrProd * sizeof(produs));
	int nr = 0;
	salvareProduseVector(capLP, vect, &nr, 25.0f);
	for(int i=0;i<nr;i++)
		printf("\nCod=%d, Denumire=%s, Categorie=%s, Pret=%5.2f",
			vect[i].cod, vect[i].denumire, vect[i].categorie,
			vect[i].pret);
	free(vect);

	dezalocareLP(capLP);
}