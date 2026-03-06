#include<stdio.h>
#include<malloc.h>

typedef struct
{
	int nrMatricol;
	char* nume;
	int* vectNote;
	int nrNote;
} student;

typedef struct
{
	student inf;
	struct nodStiva* next;
} nodStiva;

nodStiva* initializareNod(student s)
{
	nodStiva* nou = (nodStiva*)malloc(sizeof(nodStiva));
	nou->inf.nrMatricol = s.nrMatricol;
	nou->inf.nume = (char*)malloc((strlen(s.nume) + 1) * sizeof(char));
	strcpy(nou->inf.nume, s.nume);
	nou->inf.nrNote = s.nrNote;
	nou->inf.vectNote = (int*)malloc(nou->inf.nrNote * sizeof(int));
	for (int i = 0; i < nou->inf.nrNote; i++)
		nou->inf.vectNote[i] = s.vectNote[i];
	nou->next = NULL;
	return nou;
}

void push(nodStiva** varf, student s)
{
	nodStiva* nou = initializareNod(s);
	if (*varf == NULL)
		*varf = nou;
	else
	{
		nou->next = *varf;
		*varf = nou;
	}
}

int pop(nodStiva** varf, student* s)
{
	if (*varf == NULL)
		return -1;
	else
	{
		(*s).nrMatricol = (*varf)->inf.nrMatricol;
		(*s).nume = (char*)malloc((strlen((*varf)->inf.nume) + 1) * sizeof(char));
		strcpy((*s).nume, (*varf)->inf.nume);
		(*s).nrNote = (*varf)->inf.nrNote;
		(*s).vectNote = (int*)malloc((*varf)->inf.nrNote * sizeof(int));
		for (int i = 0; i < (*varf)->inf.nrNote; i++)
			(*s).vectNote[i] = (*varf)->inf.vectNote[i];

		nodStiva* temp = *varf;
		*varf = (*varf)->next;
		free(temp->inf.nume);
		free(temp->inf.vectNote);
		free(temp);
		return 0;
	}
}