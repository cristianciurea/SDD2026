#include<stdio.h>
#include<malloc.h>

typedef struct
{
	int cod;
	char* denumire;
	int nrPreturi;
	float* vectPreturi;
} produs;

typedef struct
{
	produs inf;
	struct nodLS* next, * prev;
} nodLS;

nodLS* initializareNod(produs p)
{
	nodLS* nou = (nodLS*)malloc(sizeof(nodLS));
	nou->inf.cod = p.cod;
	nou->inf.denumire = (char*)malloc((strlen(p.denumire) + 1) *
		sizeof(char));
	strcpy(nou->inf.denumire, p.denumire);
	nou->inf.nrPreturi = p.nrPreturi;
	nou->inf.vectPreturi = (float*)malloc(nou->inf.nrPreturi *
		sizeof(float));
	for (int i = 0; i < nou->inf.nrPreturi; i++)
		nou->inf.vectPreturi[i] = p.vectPreturi[i];
	nou->next = nou;
	nou->prev = nou;
	return nou;
}

nodLS* inserareNod1(nodLS* capLS, nodLS** coada, produs p)
{
	nodLS* nou = initializareNod(p);
	if (capLS == NULL)
	{
		capLS = nou;
		*coada = nou;
	}
	else
	{
		/*nodLS* temp = capLS;
		while (temp->next != NULL)
			temp = temp->next;
		temp->next = nou;
		nou->prev = temp;
		*coada = nou;*/
		(*coada)->next = nou;
		nou->prev = *coada;
		*coada = nou;
		(*coada)->next = capLS;
		capLS->prev = *coada;
	}
	return capLS;
}

void inserareNod2(nodLS** capLS, produs p)
{
	nodLS* nou = initializareNod(p);
	if (*capLS == NULL)
		*capLS = nou;
	else
	{
		nodLS* temp = *capLS;
		while (temp->next != NULL)
			temp = temp->next;
		temp->next = nou;
	}
}

nodLS* inserareNod3(nodLS* capLS, produs p)
{
	nodLS* nou = initializareNod(p);
	if (capLS == NULL)
		capLS = nou;
	else
	{
		nou->next = capLS;
		capLS = nou;
	}
	return capLS;
}

void traversareLS(nodLS* capLS)
{
	nodLS* temp = capLS;
	while (temp->next!=capLS)
	{
		printf("\nProdusul %s cu codul %d are %d preturi: ", temp->inf.denumire,
			temp->inf.cod, temp->inf.nrPreturi);
		for (int i = 0; i < temp->inf.nrPreturi; i++)
			printf("%5.2f, ", temp->inf.vectPreturi[i]);
		temp = temp->next;
	}
	printf("\nProdusul %s cu codul %d are %d preturi: ", temp->inf.denumire,
		temp->inf.cod, temp->inf.nrPreturi);
	for (int i = 0; i < temp->inf.nrPreturi; i++)
		printf("%5.2f, ", temp->inf.vectPreturi[i]);
}

void traversareInvers(nodLS* coada)
{
	nodLS* temp = coada;
	while (temp->prev!=coada)
	{
		printf("\nProdusul %s cu codul %d are %d preturi: ", temp->inf.denumire,
			temp->inf.cod, temp->inf.nrPreturi);
		for (int i = 0; i < temp->inf.nrPreturi; i++)
			printf("%5.2f, ", temp->inf.vectPreturi[i]);
		temp = temp->prev;
	}
	printf("\nProdusul %s cu codul %d are %d preturi: ", temp->inf.denumire,
		temp->inf.cod, temp->inf.nrPreturi);
	for (int i = 0; i < temp->inf.nrPreturi; i++)
		printf("%5.2f, ", temp->inf.vectPreturi[i]);
}

void dezalocareLS(nodLS* capLS)
{
	nodLS* temp = capLS;
	while (temp->next!=capLS)
	{
		nodLS* temp2 = temp->next;
		free(temp->inf.denumire);
		free(temp->inf.vectPreturi);
		free(temp);
		temp = temp2;
	}
	free(temp->inf.denumire);
	free(temp->inf.vectPreturi);
	free(temp);
}

float mediePreturi(float* vectPreturi, int nrPreturi)
{
	float suma = 0.0f;
	for (int i = 0; i < nrPreturi; i++)
		suma += vectPreturi[i];
	return suma / nrPreturi;
}

void salvareVectorProduse(nodLS* capLS, produs* vect, int* nrElem, float pretPrag)
{
	nodLS* temp = capLS;
	while (temp->next!=capLS)
	{
		if (mediePreturi(temp->inf.vectPreturi, temp->inf.nrPreturi) >= pretPrag)
		{
			vect[*nrElem] = temp->inf;
			(*nrElem)++;
		}
		temp = temp->next;
	}
	if (mediePreturi(temp->inf.vectPreturi, temp->inf.nrPreturi) >= pretPrag)
	{
		vect[*nrElem] = temp->inf;
		(*nrElem)++;
	}
}

void stergeProdusDenumire(nodLS** cap, nodLS** coada, char* denumireDeSters)
{
	if (strcmp((*cap)->inf.denumire, denumireDeSters) == 0)
	{
		nodLS* temp = *cap;
		*cap = (*cap)->next;
		/*if (*cap != NULL)
			(*cap)->prev = NULL;*/
		(*cap)->prev = *coada;
		(*coada)->next = *cap;
		free(temp->inf.denumire);
		free(temp->inf.vectPreturi);
		free(temp);
		return;
	}
	else
		if (strcmp((*coada)->inf.denumire, denumireDeSters) == 0)
		{
			nodLS* temp = *coada;
			*coada = (*coada)->prev;
			/*if (*coada != NULL)
				(*coada)->next = NULL;*/
			(*coada)->next = *cap;
			(*cap)->prev = *coada;
			free(temp->inf.denumire);
			free(temp->inf.vectPreturi);
			free(temp);
			return;
		}
		else
		{
			nodLS* temp = *cap;
			while (temp->next != *cap)
			{
				if (strcmp(temp->inf.denumire, denumireDeSters) == 0)
				{
					nodLS* urmator = temp->next;
					nodLS* anterior = temp->prev;
					anterior->next = urmator;
					urmator->prev = anterior;
					free(temp->inf.denumire);
					free(temp->inf.vectPreturi);
					free(temp);
					return;
				}
				else
					temp = temp->next;
			}
			if (strcmp(temp->inf.denumire, denumireDeSters) == 0)
			{
				nodLS* urmator = temp->next;
				nodLS* anterior = temp->prev;
				anterior->next = urmator;
				urmator->prev = anterior;
				free(temp->inf.denumire);
				free(temp->inf.vectPreturi);
				free(temp);
				return;
			}
		}
}

void stergeProdusPretPrag(nodLS** cap, nodLS** coada, float pretPrag)
{
	if (mediePreturi((*cap)->inf.vectPreturi, (*cap)->inf.nrPreturi) >= pretPrag)
	{
		nodLS* temp = *cap;
		*cap = (*cap)->next;
		/*if (*cap != NULL)
			(*cap)->prev = NULL;*/
		(*cap)->prev = *coada;
		(*coada)->next = *cap;
		free(temp->inf.denumire);
		free(temp->inf.vectPreturi);
		free(temp);
		return;
	}
	else
		if (mediePreturi((*coada)->inf.vectPreturi, (*coada)->inf.nrPreturi) >= pretPrag)
		{
			nodLS* temp = *coada;
			*coada = (*coada)->prev;
			/*if (*coada != NULL)
				(*coada)->next = NULL;*/
			(*coada)->next = *cap;
			(*cap)->prev = *coada;
			free(temp->inf.denumire);
			free(temp->inf.vectPreturi);
			free(temp);
			return;
		}
		else
		{
			nodLS* temp = *cap;
			while (temp->next != *cap)
			{
				if (mediePreturi(temp->inf.vectPreturi, temp->inf.nrPreturi) >= pretPrag)
				{
					nodLS* urmator = temp->next;
					nodLS* anterior = temp->prev;
					anterior->next = urmator;
					urmator->prev = anterior;
					free(temp->inf.denumire);
					free(temp->inf.vectPreturi);
					free(temp);
					return;
				}
				else
					temp = temp->next;
			}
			if (mediePreturi(temp->inf.vectPreturi, temp->inf.nrPreturi) >= pretPrag)
			{
				nodLS* urmator = temp->next;
				nodLS* anterior = temp->prev;
				anterior->next = urmator;
				urmator->prev = anterior;
				free(temp->inf.denumire);
				free(temp->inf.vectPreturi);
				free(temp);
				return;
			}
		}
}

void main()
{
	int nrProd;
	char buffer[20];
	nodLS* capLS = NULL, * coada = NULL;
	produs p;

	FILE* f = fopen("fisier.txt", "r");
	fscanf(f, "%d", &nrProd);
	for (int i = 0; i < nrProd; i++)
	{
		fscanf(f, "%d", &p.cod);
		fscanf(f, "%s", buffer);
		p.denumire = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
		strcpy(p.denumire, buffer);
		fscanf(f, "%d", &p.nrPreturi);
		p.vectPreturi = (float*)malloc(p.nrPreturi * sizeof(float));
		for (int j = 0; j < p.nrPreturi; j++)
			fscanf(f, "%f", &p.vectPreturi[j]);
		capLS = inserareNod1(capLS, &coada, p);
		//inserareNod2(&capLS, p);
		//capLS = inserareNod3(capLS, p);
		free(p.denumire);
		free(p.vectPreturi);
	}
	fclose(f);

	traversareLS(capLS);
	printf("\n--------------------\n");
	traversareInvers(coada);

	printf("\n--------------------\n");
	produs* vect = (produs*)malloc(nrProd * sizeof(produs));
	int nrElem = 0;
	salvareVectorProduse(capLS, vect, &nrElem, 11.0f);
	for (int i = 0; i < nrElem; i++)
	{
		printf("\nProdusul %s cu codul %d are %d preturi: ",
			vect[i].denumire, vect[i].cod, vect[i].nrPreturi);
		for (int j = 0; j < vect[i].nrPreturi; j++)
			printf("%5.2f, ", vect[i].vectPreturi[j]);
	}
	free(vect);

	printf("\n---------dupa stergere-----------\n");
	//stergeProdusDenumire(&capLS, &coada, "oua");
	stergeProdusPretPrag(&capLS, &coada, 11.0f);
	traversareLS(capLS);
	printf("\n--------------------\n");
	traversareInvers(coada);

	dezalocareLS(capLS);
}