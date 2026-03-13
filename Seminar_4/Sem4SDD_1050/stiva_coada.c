#include<stdio.h>
#include<malloc.h>

typedef struct
{
	int zi, luna, an;
} dataTransmitere;

typedef struct
{
	dataTransmitere data;
	char* destinatar;
	float dimensiune;
} mesaj;

typedef struct
{
	mesaj inf;
	struct nodStiva* next;
} nodStiva;

typedef struct
{
	mesaj inf;
	struct nodCoada* next;
} nodCoada;

nodStiva* initializareNodStiva(mesaj m)
{
	nodStiva* nou = (nodStiva*)malloc(sizeof(nodStiva));
	nou->inf.data.zi = m.data.zi;
	nou->inf.data.luna = m.data.luna;
	nou->inf.data.an = m.data.an;
	nou->inf.destinatar = (char*)malloc((strlen(m.destinatar) + 1) *
		sizeof(char));
	strcpy(nou->inf.destinatar, m.destinatar);
	nou->inf.dimensiune = m.dimensiune;
	nou->next = NULL;
	return nou;
}

nodCoada* initializareNodCoada(mesaj m)
{
	nodCoada* nou = (nodCoada*)malloc(sizeof(nodCoada));
	nou->inf.data.zi = m.data.zi;
	nou->inf.data.luna = m.data.luna;
	nou->inf.data.an = m.data.an;
	nou->inf.destinatar = (char*)malloc((strlen(m.destinatar) + 1) *
		sizeof(char));
	strcpy(nou->inf.destinatar, m.destinatar);
	nou->inf.dimensiune = m.dimensiune;
	nou->next = NULL;
	return nou;
}

void push(nodStiva** varf, mesaj m)
{
	nodStiva* nou = initializareNodStiva(m);
	if (*varf == NULL)
		*varf = nou;
	else
	{
		nou->next = *varf;
		*varf = nou;
	}
}

int pop(nodStiva** varf, mesaj* m)
{
	if (*varf == NULL)
		return -1;
	else
	{
		(*m).data.zi = (*varf)->inf.data.zi;
		(*m).data.luna = (*varf)->inf.data.luna;
		(*m).data.an = (*varf)->inf.data.an;
		(*m).destinatar = (char*)malloc((strlen((*varf)->inf.destinatar) + 1) *
			sizeof(char));
		strcpy((*m).destinatar, (*varf)->inf.destinatar);
		(*m).dimensiune = (*varf)->inf.dimensiune;

		nodStiva* temp = *varf;
		*varf = (*varf)->next;
		free(temp->inf.destinatar);
		free(temp);
		return 0;
	}
}

void afisareStiva(nodStiva* varf)
{
	nodStiva* temp = varf;
	while (temp)
	{
		printf("\nData: %d/%d/%d, Destinatar: %s, Dimensiune: %5.2f",
			temp->inf.data.zi, temp->inf.data.luna,
			temp->inf.data.an, temp->inf.destinatar,
			temp->inf.dimensiune);
		temp = temp->next;
	}
}

void dezalocareStiva(nodStiva* varf)
{
	mesaj m;
	while (pop(&varf, &m) == 0)
		free(m.destinatar);
}

void salvareMesajeVector(nodStiva* varf, mesaj* vect, int* nr,
	float dimPrag)
{
	mesaj m;
	while (pop(&varf, &m) == 0)
	{
		if (m.dimensiune >= dimPrag)
		{
			vect[*nr] = m;
			(*nr)++;
		}
	}
}

void put(nodCoada** prim, nodCoada** ultim, mesaj m)
{
	nodCoada* nou = initializareNodCoada(m);
	if (*prim == NULL || *ultim == NULL)
	{
		*prim = nou;
		*ultim = nou;
	}
	else
	{
		(*ultim)->next = nou;
		*ultim = nou;
	}
}

int get(nodCoada** prim, nodCoada** ultim, mesaj* m)
{
	if (*prim != NULL && *ultim != NULL)
	{
		(*m).data.zi = (*prim)->inf.data.zi;
		(*m).data.luna = (*prim)->inf.data.luna;
		(*m).data.an = (*prim)->inf.data.an;
		(*m).destinatar = (char*)malloc((strlen((*prim)->inf.destinatar) + 1) *
			sizeof(char));
		strcpy((*m).destinatar, (*prim)->inf.destinatar);
		(*m).dimensiune = (*prim)->inf.dimensiune;

		nodCoada* temp = *prim;
		*prim = (*prim)->next;
		free(temp->inf.destinatar);
		free(temp);
		return 0;
	}
	if (*prim == NULL)
	{
		*ultim = NULL;
		return -1;
	}
}

void afisareCoada(nodCoada* prim)
{
	nodCoada* temp = prim;
	while (temp)
	{
		printf("\nData: %d/%d/%d, Destinatar: %s, Dimensiune: %5.2f",
			temp->inf.data.zi, temp->inf.data.luna,
			temp->inf.data.an, temp->inf.destinatar,
			temp->inf.dimensiune);
		temp = temp->next;
	}
}

void dezalocareCoada(nodCoada* prim, nodCoada* ultim)
{
	mesaj m;
	while (get(&prim, &ultim, &m) == 0)
		free(m.destinatar);
}

void conversieStivaCoada(nodStiva* varf, nodCoada** prim,
	nodCoada** ultim)
{
	mesaj m;
	while (pop(&varf, &m) == 0)
	{
		put(prim, ultim, m);
		free(m.destinatar);
	}
}

void main()
{
	int nrMesaje;
	nodStiva* varf = NULL;
	nodCoada* prim = NULL, * ultim = NULL;
	mesaj m;
	char buffer[20];
	FILE* f = fopen("fisier.txt", "r");
	fscanf(f, "%d", &nrMesaje);
	for (int i = 0; i < nrMesaje; i++)
	{
		fscanf(f, "%d", &m.data.zi);
		fscanf(f, "%d", &m.data.luna);
		fscanf(f, "%d", &m.data.an);
		fscanf(f, "%s", buffer);
		m.destinatar = (char*)malloc((strlen(buffer) + 1) *
			sizeof(char));
		strcpy(m.destinatar, buffer);
		fscanf(f, "%f", &m.dimensiune);
		//inserare stiva
		push(&varf, m);
		//inserare coada
		//put(&prim, &ultim, m);
		free(m.destinatar);
	}
	fclose(f);

	printf("\n-----------Stiva--------------\n");
	afisareStiva(varf);

	printf("\n-----------Coada--------------\n");
	conversieStivaCoada(varf, &prim, &ultim);
	afisareCoada(prim);

	printf("\n------------Vector-------------\n");
	/*mesaj* vect = (mesaj*)malloc(nrMesaje * sizeof(mesaj));
	int nr = 0;
	salvareMesajeVector(varf, vect, &nr, 150.0f);
	for (int i = 0; i < nr; i++)
		printf("\nData: %d/%d/%d, Destinatar: %s, Dimensiune: %5.2f",
			vect[i].data.zi, vect[i].data.luna,
			vect[i].data.an, vect[i].destinatar,
			vect[i].dimensiune);
	for (int i = 0; i < nr; i++)
		free(vect[i].destinatar);
	free(vect);*/

	//dezalocareStiva(varf);

	dezalocareCoada(prim, ultim);
}