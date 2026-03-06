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
	struct nodLS* next, * prev;
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
	nou->next = nou;
	nou->prev = nou;
	return nou;
}

nodLS* inserareNod1(nodLS* capLS, nodLS** coada, student s)
{
	nodLS* nou = initializareNod(s);
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
	while (temp->next != capLS)
	{
		printf("\nStudentul %s cu nr. matricol %d are %d note: ",
			temp->inf.nume, temp->inf.nrMatricol, temp->inf.nrNote);
		for (int i = 0; i < temp->inf.nrNote; i++)
			printf("%d, ", temp->inf.vectNote[i]);
		temp = temp->next;
	}
	printf("\nStudentul %s cu nr. matricol %d are %d note: ",
		temp->inf.nume, temp->inf.nrMatricol, temp->inf.nrNote);
	for (int i = 0; i < temp->inf.nrNote; i++)
		printf("%d, ", temp->inf.vectNote[i]);
}

void traversareInvers(nodLS* coada)
{
	nodLS* temp = coada;
	while (temp->prev != coada)
	{
		printf("\nStudentul %s cu nr. matricol %d are %d note: ",
			temp->inf.nume, temp->inf.nrMatricol, temp->inf.nrNote);
		for (int i = 0; i < temp->inf.nrNote; i++)
			printf("%d, ", temp->inf.vectNote[i]);
		temp = temp->prev;
	}
	printf("\nStudentul %s cu nr. matricol %d are %d note: ",
		temp->inf.nume, temp->inf.nrMatricol, temp->inf.nrNote);
	for (int i = 0; i < temp->inf.nrNote; i++)
		printf("%d, ", temp->inf.vectNote[i]);
}

void dezalocareLS(nodLS* capLS)
{
	nodLS* temp = capLS;
	while (temp->next!=capLS)
	{
		nodLS* temp2 = temp->next;
		free(temp->inf.nume);
		free(temp->inf.vectNote);
		free(temp);
		temp = temp2;
	}
	free(temp->inf.nume);
	free(temp->inf.vectNote);
	free(temp);
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
	while (temp->next!=capLS)
	{
		if (medieNote(temp->inf.vectNote, temp->inf.nrNote) >= mediePrag)
		{
			vect[*nrElem] = temp->inf;
			(*nrElem)++;
		}
		temp = temp->next;
	}
	if (medieNote(temp->inf.vectNote, temp->inf.nrNote) >= mediePrag)
	{
		vect[*nrElem] = temp->inf;
		(*nrElem)++;
	}
}

void stergeStudentNume(nodLS** cap, nodLS** coada, char* numeDeSters)
{
	if (strcmp((*cap)->inf.nume, numeDeSters) == 0)
	{
		nodLS* temp = *cap;
		*cap = (*cap)->next;
		/*if (*cap != NULL)
			(*cap)->prev = NULL;*/
		(*cap)->prev = *coada;
		(*coada)->next = *cap;
		free(temp->inf.nume);
		free(temp->inf.vectNote);
		free(temp);
		return;
	}
	else
		if (strcmp((*coada)->inf.nume, numeDeSters) == 0)
		{
			nodLS* temp = *coada;
			*coada = (*coada)->prev;
			/*if (*coada != NULL)
				(*coada)->next = NULL;*/
			(*coada)->next = *cap;
			(*cap)->prev = *coada;
			free(temp->inf.nume);
			free(temp->inf.vectNote);
			free(temp);
			return;
		}
		else
		{
			nodLS* temp = *cap;
			while (temp->next != *cap)
			{
				if (strcmp(temp->inf.nume, numeDeSters) == 0)
				{
					nodLS* urmator = temp->next;
					nodLS* anterior = temp->prev;
					anterior->next = urmator;
					urmator->prev = anterior;
					free(temp->inf.nume);
					free(temp->inf.vectNote);
					free(temp);
					return;
				}
				else
					temp = temp->next;
			}
			if (strcmp(temp->inf.nume, numeDeSters) == 0)
			{
				nodLS* urmator = temp->next;
				nodLS* anterior = temp->prev;
				anterior->next = urmator;
				urmator->prev = anterior;
				free(temp->inf.nume);
				free(temp->inf.vectNote);
				free(temp);
				return;
			}
		}
}

void stergeStudentMediePrag(nodLS** cap, nodLS** coada, float mediePrag)
{
	if (medieNote((*cap)->inf.vectNote, (*cap)->inf.nrNote) >= mediePrag)
	{
		nodLS* temp = *cap;
		*cap = (*cap)->next;
		(*cap)->prev = *coada;
		(*coada)->next = *cap;
		free(temp->inf.nume);
		free(temp->inf.vectNote);
		free(temp);
		return;
	}
		else
		{
			nodLS* temp = *cap;
			while (temp->next != *cap)
			{
				if (medieNote(temp->inf.vectNote, temp->inf.nrNote) >= mediePrag)
				{
					nodLS* urmator = temp->next;
					nodLS* anterior = temp->prev;
					anterior->next = urmator;
					urmator->prev = anterior;
					free(temp->inf.nume);
					free(temp->inf.vectNote);
					free(temp);
					return;
				}
				else
					temp = temp->next;
			}
			if (medieNote(temp->inf.vectNote, temp->inf.nrNote) >= mediePrag)
			{
				nodLS* urmator = temp->next;
				nodLS* anterior = temp->prev;
				anterior->next = urmator;
				urmator->prev = anterior;
				*coada = anterior;
				free(temp->inf.nume);
				free(temp->inf.vectNote);
				free(temp);
				return;
			}
		}
}

void main()
{
	int nrStud;
	nodLS* capLS = NULL, * coada = NULL;
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
		capLS = inserareNod1(capLS, &coada, s);
		//inserareNod2(&capLS, s);
		//capLS = inserareInceput(capLS, s);
		free(s.nume);
		free(s.vectNote);
	}
	fclose(f);

	traversareLS(capLS);
	printf("\n------------------------\n");
	traversareInvers(coada);

	printf("\n------------vector------------\n");
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

	printf("\n-----------dupa stergere-------------\n");
	//stergeStudentNume(&capLS, &coada, "Gigel");
	stergeStudentMediePrag(&capLS, &coada, 8.0f);
	traversareLS(capLS);
	printf("\n------------------------\n");
	traversareInvers(coada);

	dezalocareLS(capLS);
}