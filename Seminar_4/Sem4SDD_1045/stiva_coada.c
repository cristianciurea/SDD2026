#include<stdio.h>
#include<malloc.h>

typedef struct
{
	int zi, luna, an;
} dataScadenta;

typedef struct
{
	dataScadenta data;
	char* furnizor;
	float sumaPlata;
} factura;

typedef struct
{
	factura inf;
	struct nodStiva* next;
} nodStiva;

typedef struct
{
	factura inf;
	struct nodCoada* next;
} nodCoada;

nodStiva* initializareNodStiva(factura f)
{
	nodStiva* nou = (nodStiva*)malloc(sizeof(nodStiva));
	nou->inf.data.zi = f.data.zi;
	nou->inf.data.luna = f.data.luna;
	nou->inf.data.an = f.data.an;
	nou->inf.furnizor = (char*)malloc((strlen(f.furnizor) + 1)
		* sizeof(char));
	strcpy(nou->inf.furnizor, f.furnizor);
	nou->inf.sumaPlata = f.sumaPlata;
	nou->next = NULL;
	return nou;
}

nodCoada* initializareNodCoada(factura f)
{
	nodCoada* nou = (nodCoada*)malloc(sizeof(nodCoada));
	nou->inf.data.zi = f.data.zi;
	nou->inf.data.luna = f.data.luna;
	nou->inf.data.an = f.data.an;
	nou->inf.furnizor = (char*)malloc((strlen(f.furnizor) + 1)
		* sizeof(char));
	strcpy(nou->inf.furnizor, f.furnizor);
	nou->inf.sumaPlata = f.sumaPlata;
	nou->next = NULL;
	return nou;
}

void push(nodStiva** varf, factura f)
{
	nodStiva* nou = initializareNodStiva(f);
	if (*varf == NULL)
		*varf = nou;
	else
	{
		nou->next = *varf;
		*varf = nou;
	}
}

int pop(nodStiva** varf, factura* f)
{
	if (*varf == NULL)
		return -1;
	else
	{
		(*f).data.zi = (*varf)->inf.data.zi;
		(*f).data.luna = (*varf)->inf.data.luna;
		(*f).data.an = (*varf)->inf.data.an;
		(*f).furnizor = (char*)malloc((strlen((*varf)->inf.furnizor) + 1) *
			sizeof(char));
		strcpy((*f).furnizor, (*varf)->inf.furnizor);
		(*f).sumaPlata = (*varf)->inf.sumaPlata;

		nodStiva* temp = *varf;
		*varf = (*varf)->next;
		free(temp->inf.furnizor);
		free(temp);
		return 0;
	}
}

void put(nodCoada** prim, nodCoada** ultim, factura f)
{
	nodCoada* nou = initializareNodCoada(f);
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

int get(nodCoada** prim, nodCoada** ultim, factura* f)
{
	if (*prim != NULL && *ultim != NULL)
	{
		(*f).data.zi = (*prim)->inf.data.zi;
		(*f).data.luna = (*prim)->inf.data.luna;
		(*f).data.an = (*prim)->inf.data.an;
		(*f).furnizor = (char*)malloc((strlen((*prim)->inf.furnizor) + 1) *
			sizeof(char));
		strcpy((*f).furnizor, (*prim)->inf.furnizor);
		(*f).sumaPlata = (*prim)->inf.sumaPlata;

		nodCoada* temp = *prim;
		*prim = (*prim)->next;
		free(temp->inf.furnizor);
		free(temp);
		return 0;
	}
	if (*prim == NULL)
	{
		*ultim = NULL;
		return -1;
	}
}

void afisareStiva(nodStiva* varf)
{
	nodStiva* temp = varf;
	while (temp)
	{
		printf("\nData: %d/%d/%d, Furnizor: %s, Suma plata: %5.2f",
			temp->inf.data.zi, temp->inf.data.luna,
			temp->inf.data.an,
			temp->inf.furnizor, temp->inf.sumaPlata);
		temp = temp->next;
	}
}

void afisareCoada(nodCoada* prim)
{
	nodCoada* temp = prim;
	while (temp)
	{
		printf("\nData: %d/%d/%d, Furnizor: %s, Suma plata: %5.2f",
			temp->inf.data.zi, temp->inf.data.luna,
			temp->inf.data.an,
			temp->inf.furnizor, temp->inf.sumaPlata);
		temp = temp->next;
	}
}

void dezalocareStiva(nodStiva* varf)
{
	factura f;
	while (pop(&varf, &f) == 0)
		free(f.furnizor);
}

void dezalocareCoada(nodCoada* prim, nodCoada* ultim)
{
	factura f;
	while (get(&prim, &ultim, &f) == 0)
		free(f.furnizor);
}

void conversieStivaCoada(nodStiva* varf, nodCoada** prim,
	nodCoada** ultim)
{
	factura f;
	while (pop(&varf, &f) == 0)
	{
		put(prim, ultim, f);
		free(f.furnizor);
	}
}

void salvareFacturiVector(nodStiva* varf, factura* vect,
	int* nr, float prag)
{
	factura f;
	while (pop(&varf, &f) == 0)
	{
		if (f.sumaPlata >= prag)
		{
			vect[*nr] = f;
			(*nr)++;
		}
	}
}

void main()
{
	int nrFacturi;
	nodStiva* varf = NULL;
	nodCoada* prim = NULL, * ultim = NULL;
	char buffer[20];
	factura fact;
	FILE* f = fopen("fisier.txt", "r");
	fscanf(f, "%d", &nrFacturi);
	for (int i = 0; i < nrFacturi; i++)
	{
		fscanf(f, "%d", &fact.data.zi);
		fscanf(f, "%d", &fact.data.luna);
		fscanf(f, "%d", &fact.data.an);
		fscanf(f, "%s", buffer);
		fact.furnizor = (char*)malloc((strlen(buffer) + 1) *
			sizeof(char));
		strcpy(fact.furnizor, buffer);
		fscanf(f, "%f", &fact.sumaPlata);
		push(&varf, fact);
		//put(&prim, &ultim, fact);
		free(fact.furnizor);
	}
	fclose(f);
	
	printf("\n--------Stiva----------\n");
	afisareStiva(varf);

	conversieStivaCoada(varf, &prim, &ultim);

	printf("\n--------Coada----------\n");
	afisareCoada(prim);

	printf("\n------------------\n");
	/*factura* vect = (factura*)malloc(nrFacturi * sizeof(factura));
	int nr = 0;
	salvareFacturiVector(varf, vect, &nr, 116.0f);
	for(int i=0;i<nr;i++)
		printf("\nData: %d/%d/%d, Furnizor: %s, Suma plata: %5.2f",
			vect[i].data.zi, vect[i].data.luna,
			vect[i].data.an,
			vect[i].furnizor, vect[i].sumaPlata);
	for (int i = 0; i < nr; i++)
		free(vect[i].furnizor);
	free(vect);*/

	//dezalocareStiva(varf);

	dezalocareCoada(prim, ultim);
}