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
	struct nodLS* next;
} nodLS;

nodLS* initializareNod(student s)
{
	nodLS* nou = (nodLS*)malloc(sizeof(nodLS));
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

nodLS* inserareNod1(nodLS* capLS, student s)
{
	nodLS* nou = initializareNod(s);
	if (capLS == NULL)
		capLS = nou;
	else
	{
		nodLS* temp = capLS;
		while (temp->next != NULL)
			temp = temp->next;
		temp->next = nou;
	}
	return capLS;
}

void inserareNod2(nodLS** capLS, student s)
{
	nodLS* nou = initializareNod(s);
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

nodLS* inserareInceput(nodLS* capLS, student s)
{
	nodLS* nou = initializareNod(s);
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
	while (temp != NULL)
	{
		printf("\nStudentul %s cu nr. matricol %d are %d note: ",
			temp->inf.nume, temp->inf.nrMatricol, temp->inf.nrNote);
		for (int i = 0; i < temp->inf.nrNote; i++)
			printf("%d, ", temp->inf.vectNote[i]);
		temp = temp->next;
	}
}

void dezalocareLS(nodLS* capLS)
{
	nodLS* temp = capLS;
	while (temp)
	{
		nodLS* temp2 = temp->next;
		free(temp->inf.nume);
		free(temp->inf.vectNote);
		free(temp);
		temp = temp2;
	}
}

float medieNote(int* note, int nrNote)
{
	float suma = 0.0f;
	for (int i = 0; i < nrNote; i++)
		suma += note[i];
	return suma / nrNote;
}

void salvareStudentiVector(nodLS* capLS, student* vect, 
	int* nrElem, float mediePrag)
{
	nodLS* temp = capLS;
	while (temp)
	{
		if (medieNote(temp->inf.vectNote, temp->inf.nrNote) >= mediePrag)
		{
			vect[*nrElem] = temp->inf;
			(*nrElem)++;
		}
		temp = temp->next;
	}
}

void main()
{
	int nrStud;
	nodLS* capLS = NULL;
	char buffer[20];
	student s;
	FILE* f = fopen("fisier.txt", "r");
	fscanf(f, "%d", &nrStud);
	for (int i = 0; i < nrStud; i++)
	{
		fscanf(f, "%d", &s.nrMatricol);
		fscanf(f, "%s", buffer);
		s.nume = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
		strcpy(s.nume, buffer);
		fscanf(f, "%d", &s.nrNote);
		s.vectNote = (int*)malloc(s.nrNote * sizeof(int));
		for (int i = 0; i < s.nrNote; i++)
			fscanf(f, "%d", &s.vectNote[i]);
		//capLS = inserareNod1(capLS, s);
		//inserareNod2(&capLS, s);
		capLS = inserareInceput(capLS, s);
		free(s.nume);
		free(s.vectNote);
	}
	fclose(f);

	traversareLS(capLS);

	printf("\n------------------------\n");
	student* vect = (student*)malloc(nrStud * sizeof(student));
	int nrElem = 0;
	salvareStudentiVector(capLS, vect, &nrElem, 8.0f);
	for (int i = 0; i < nrElem; i++)
	{
		printf("\nStudentul %s cu nr. matricol %d are %d note: ",
			vect[i].nume, vect[i].nrMatricol, vect[i].nrNote);
		for (int j = 0; j < vect[i].nrNote; j++)
			printf("%d, ", vect[i].vectNote[j]);
	}
	free(vect);

	dezalocareLS(capLS);
}