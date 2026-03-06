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
	struct nodStiva* next;
} nodStiva;

nodStiva* initializareNod(produs p)
{
	nodStiva* nou = (nodStiva*)malloc(sizeof(nodStiva));
	nou->inf.cod = p.cod;
	nou->inf.denumire = (char*)malloc((strlen(p.denumire) + 1) *
		sizeof(char));
	strcpy(nou->inf.denumire, p.denumire);
	nou->inf.nrPreturi = p.nrPreturi;
	nou->inf.vectPreturi = (float*)malloc(nou->inf.nrPreturi *
		sizeof(float));
	for (int i = 0; i < nou->inf.nrPreturi; i++)
		nou->inf.vectPreturi[i] = p.vectPreturi[i];
	nou->next = NULL;
	return nou;
}

void push(nodStiva** varf, produs p)
{
	nodStiva* nou = initializareNod(p);
	if (*varf == NULL)
		*varf = nou;
	else
	{
		nou->next = *varf;
		*varf = nou;
	}
}

int pop(nodStiva** varf, produs* p)
{
	if (*varf == NULL)
		return -1;
	else
	{
		(*p).cod = (*varf)->inf.cod;
		(*p).denumire = (char*)malloc((strlen((*varf)->inf.denumire) + 1) *
			sizeof(char));
		strcpy((*p).denumire, (*varf)->inf.denumire);
		(*p).nrPreturi = (*varf)->inf.nrPreturi;
		(*p).vectPreturi = (float*)malloc((*varf)->inf.nrPreturi *
			sizeof(float));
		for (int i = 0; i < (*varf)->inf.nrPreturi; i++)
			(*p).vectPreturi[i] = (*varf)->inf.vectPreturi[i];
		nodStiva* temp = *varf;
		*varf = (*varf)->next;
		free(temp->inf.denumire);
		free(temp->inf.vectPreturi);
		free(temp);
		return 0;
	}
}

void dezalocareStiva(nodStiva* varf)
{
	produs p;
	while (pop(varf, &p) == 0)
	{
		free(p.denumire);
		free(p.vectPreturi);
	}

}
