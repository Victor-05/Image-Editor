#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// structura pentru imaginile color
struct pixel {
		int r;
		int g;
		int b;
	};

// structura pentru imaginile in tonuri de gri
struct colt {
	int x;
	int y;
};

// functie pentru eliberarea imaginilor color
void eliberare_matrice_pixel(int lin, struct pixel ***imagine)
{
	if (!*imagine) {
		return;
	}
	for (int i = 0; i < lin; i++) {
		free((*imagine)[i]);

	}
	free(*imagine);
	*imagine = NULL;
}

// functie pentru alocarea imaginilor in tonuri de gri
void alocare_matrice_int(int ***matrice, int lin, int col)
{
	int i;
	*matrice = (int **)calloc(lin, sizeof(int *));
	if (!*matrice) {
		printf("Eroare");
	}
	for (i = 0; i < lin; i++) {
		(*matrice)[i] = (int *)calloc(col, sizeof(int));
		if (!(*matrice)[i]) {
			printf("Eroare la alocare %d", i);
		}

	}
}

// functie pentru salvarea imaginilor prelucrate
void SAVE(char *fisier, int **imagine, struct pixel **imagine_culori,
		  int linii, int coloane, char *p, char *tip)
{
	int i, j;
	fisier[strcspn(fisier, "\n")] = '\0';
	// salvarea fisierelor text
	if (p) {
		FILE *save = fopen(fisier, "wb");
			if (strcmp("P2", tip) == 0 || strcmp("P5", tip) == 0) {
				fprintf(save, "P2\n%d %d\n255\n", coloane, linii);
				for (i = 0 ; i < linii; i++) {
					for (j = 0; j < coloane; j++) {
						fprintf(save, "%d ", imagine[i][j]);

					}
						fprintf(save, "\n");
				}
				fprintf(save, "\n");
			} else if (strcmp("P3", tip) == 0 || strcmp("P6", tip) == 0) {
				int i, j;
				fprintf(save, "P3\n%d %d\n255\n", coloane, linii);
				for (i = 0; i < linii; i++) {
					for (j = 0; j < coloane; j++) {
						fprintf(save, "%d %d", imagine_culori[i][j].r,
								imagine_culori[i][j].g);
						fprintf(save, " %d ", imagine_culori[i][j].b);
					}
					fprintf(save, "\n");
				}

			}
		fclose(save);
		printf("Saved %s\n", fisier);
	} else {
		// salvarea fisierelor binare
		FILE *save = fopen(fisier, "wb");
			if (strcmp("P2", tip) == 0 || strcmp("P5", tip) == 0) {
				fwrite("P5\n", sizeof(char), 3, save);
				fprintf(save, "%d %d", coloane, linii);
				fprintf(save, "\n");
				fwrite("255", sizeof(char), 3, save);
				fprintf(save, "\n");
				for (i = 0 ; i < linii; i++) {
					for (j = 0; j < coloane; j++) {
						unsigned char valoare = (unsigned char)imagine[i][j];
						fwrite(&valoare, sizeof(unsigned char), 1, save);
					}
				}
			} else if (strcmp("P3", tip) == 0 || strcmp("P6", tip) == 0) {
				int i, j;
				fwrite("P6\n", sizeof(char), 3, save);
				fprintf(save, "%d %d\n", coloane, linii);
				fwrite("255\n", sizeof(unsigned char), 4, save);
				for (i = 0; i < linii; i++) {
					for (j = 0; j < coloane; j++) {
						fwrite(&imagine_culori[i][j].r,
							   sizeof(unsigned char), 1, save);
						fwrite(&imagine_culori[i][j].g,
							   sizeof(unsigned char), 1, save);
						fwrite(&imagine_culori[i][j].b,
							   sizeof(unsigned char), 1, save);
					}
				}

			}
			fclose(save);
			printf("Saved %s\n", fisier);
	}
}

// functie pentru eliberarea imaginilor in tonuri de gri
void eliberare_matrice_int(int lin, int ***matrice)
{
	if (!*matrice) {
		return;
	}
	for (int i = 0; i < lin; i++) {
		free((*matrice)[i]);

	}
	free(*matrice);
	*matrice = NULL;
}

// functie pentru alocarea imaginilor color
void alocare_matrice_pixel(struct pixel ***matrice, int lin, int col)
{
	int i;
	*matrice = (struct pixel **)calloc(lin, sizeof(struct pixel *));
	if (!*matrice) {
		printf("Eroare");
	}
	for (i = 0; i < lin; i++) {
		(*matrice)[i] = (struct pixel *)calloc(col, sizeof(struct pixel));
		if (!(*matrice)[i]) {
			printf("Eroare la alocare %d", i);
		}

	}
}

// functie pentru incarcarea imaginilor se verifica tipul de imagine
// apoi aceasta este incarcata in memorie
// incarcare imaginilor de tip P2, format text
// incarcare imaginilor de tip P3, format text
// incarcare imaginilor de tip P5, format binar
// incarcare imaginilor de tip P6, format binar
void LOAD(char *input_file, int ***imagine, struct pixel ***imagine_culori,
		  int *linii, int *coloane, char tip[3], int *incarcata)
{
	input_file[strlen(input_file) - 1] = '\0';
	FILE *in = fopen(input_file, "rb");
	int  i, j, ok;
	char linie[50], *p;
	if (*imagine) {
		eliberare_matrice_int(*linii, &(*imagine));
	}
	if (*imagine_culori) {
		eliberare_matrice_pixel(*linii, &(*imagine_culori));
	}
	if (in) {
		*incarcata = 1;
	} else {
		*incarcata = 0;
		printf("Failed to load %s\n", input_file);
		return;
	}
	while (fgets(linie, sizeof(linie), in)) {
		if (linie[0] >= '0' && linie[0] <= '9') {
			break;
		}
		if (linie[0] == 'P') {
			linie[2] = '\0';
			memmove(tip, linie, strlen(linie) + 1);
		}
	}
	p = strtok(linie, " ");
	*coloane = atoi(p);
	p = strtok(NULL, " ");
	*linii = atoi(p);
	if (strcmp(tip, "P2") == 0) {
		alocare_matrice_int(&(*imagine), *linii, *coloane);
		fscanf(in, "%d", &ok);
		for (i = 0 ; i < *linii; i++) {
			for (j = 0; j < *coloane; j++) {
				fscanf(in, "%d", &(*imagine)[i][j]);
			}
		}
	} else if (strcmp(tip, "P3") == 0) {
		alocare_matrice_pixel(&(*imagine_culori), *linii, *coloane);
		fscanf(in, "%d", &ok);
		for (i = 0; i < *linii; i++) {
			for (j = 0; j < *coloane; j++) {
				int r, g, b;
				fscanf(in, "%d%d%d", &r, &g, &b);
				(*imagine_culori)[i][j].r = r;
				(*imagine_culori)[i][j].g = g;
				(*imagine_culori)[i][j].b = b;
			}
		}
	} else if (strcmp(tip, "P5") == 0) {
		alocare_matrice_int(&(*imagine), *linii, *coloane);
		fread(&ok, sizeof(int), 1, in);
		for (i = 0; i < *linii; i++) {
			for (j = 0; j < *coloane; j++) {
				unsigned char x;
				fread(&x, sizeof(unsigned char), 1, in);
				(*imagine)[i][j] = (int)x;
			}
		}
	} else if (strcmp(tip, "P6") == 0) {
		alocare_matrice_pixel(&(*imagine_culori), *linii, *coloane);
		fread(&ok, sizeof(int), 1, in);
		for (i = 0; i < *linii; i++) {
			for (j = 0; j < *coloane; j++) {
				unsigned char r, g, b;
				fread(&r, sizeof(unsigned char), 1, in);
				fread(&g, sizeof(unsigned char), 1, in);
				fread(&b, sizeof(unsigned char), 1, in);
				(*imagine_culori)[i][j].r = (int)r;
				(*imagine_culori)[i][j].g = (int)g;
				(*imagine_culori)[i][j].b = (int)b;
			}
		}
	}
	printf("Loaded %s\n", input_file);
	fclose(in);
}

// functie care verifica daca sirul p contine un numar sau contine si litere
int verificare_numar(char *p)
{
	if (!p) {
		return 0;
	}
	while (*p) {
		if (*p == '-') {
			p++;
		}
		if (!(*p >= '0' && *p <= '9')) {
			return 0;
		}
		p++;
	}
	return 1;
}

// functie pentru selectarea pixelior pe baza carora se va lucra
// se verifica daca datele de intrare (coordonatele) sunt exact 4
// si daca acestea sunt numere, apoi se verifica daca sunt valide
// in functie de dimensiunea imaginii, urmand sa se modifice
// coordonatelor de lucru
void SELECT(struct colt *colt_sus, struct colt *colt_jos,
			int x1, int y1, int x2, int y2,
			int linii, int coloane, char *p)
{
	int este_numar = 0;
	este_numar = verificare_numar(p);
	if (p) {
		p[strcspn(p, "\n")] = '\0';
	}
	if (p && este_numar) {
		x1 = atoi(p);
	} else {
		printf("Invalid command\n");
		return;
	}
	este_numar = 0;
	p = strtok(NULL, " ");
	if (p) {
		p[strcspn(p, "\n")] = '\0';
	}
	este_numar = verificare_numar(p);

	if (p && este_numar) {
		y1 = atoi(p);
	} else {
		printf("Invalid command\n");
		return;
	}
	este_numar = 0;
	p = strtok(NULL, " ");
	if (p) {
		p[strcspn(p, "\n")] = '\0';
	}
	este_numar = verificare_numar(p);

	if (p && este_numar) {
		x2 = atoi(p);
	} else {
		printf("Invalid command\n");
		return;
	}
	este_numar = 0;
	p = strtok(NULL, " ");
	if (p) {
		p[strcspn(p, "\n")] = '\0';
	}
	este_numar = verificare_numar(p);
	if (p && este_numar) {
		y2 = atoi(p);
	} else {
		printf("Invalid command\n");
		return;
	}
	p = strtok(NULL, " ");
	if (p) {
		printf("Invalid command\n");
		return;
	}
	if ((x1 > coloane || x1 < 0) || (x2 > coloane || x2 < 0) ||
		(y1 > linii || y1 < 0) || (y2 > linii || y2 < 0) ||
		(x1 == x2 || y1 == y2)) {
		printf("Invalid set of coordinates\n");
		return;
	}
	if (x1 < x2) {
		(*colt_sus).x = x1;
		(*colt_jos).x = x2;
	} else {
		(*colt_sus).x = x2;
		(*colt_jos).x = x1;
	}
	if (y1 < y2) {
		(*colt_sus).y = y1;
		(*colt_jos).y = y2;
	} else {
		(*colt_sus).y = y2;
		(*colt_jos).y = y1;
	}
	printf("Selected %d %d %d %d\n", (*colt_sus).x,
		   (*colt_sus).y, (*colt_jos).x, (*colt_jos).y);
}

// functie pentru taierea imaginilor in tonuri de gri
void CROP_INT(int ***imagine, int linii, struct colt colt_sus,
			  struct colt colt_jos)
{
	int **imagine_utilizabila;
	int nr_linii = colt_jos.y - colt_sus.y;
	int nr_coloane = colt_jos.x - colt_sus.x;
	// copiere imagine
	alocare_matrice_int(&imagine_utilizabila, nr_linii, nr_coloane);
	int i, j;
	for (i = colt_sus.y; i < colt_jos.y; i++) {
		for (j = colt_sus.x ; j < colt_jos.x; j++) {
			imagine_utilizabila[i - colt_sus.y][j - colt_sus.x] =
			(*imagine)[i][j];

		}

	}
	// recontruire imagine dupa crop
	eliberare_matrice_int(linii, &(*imagine));
	alocare_matrice_int(&(*imagine), nr_linii, nr_coloane);
	for (i = 0; i < nr_linii; i++) {
		for (j = 0 ; j < nr_coloane; j++) {
			(*imagine)[i][j] = imagine_utilizabila[i][j];

		}

	}
	eliberare_matrice_int(nr_linii, &imagine_utilizabila);
}

// functie pentru taierea imaginilor color
void CROP_PIXEL(struct pixel ***imagine, int linii, struct colt colt_sus,
				struct colt colt_jos)
{
	struct pixel **imagine_utilizabila;
	int nr_linii = colt_jos.y - colt_sus.y;
	int nr_coloane = colt_jos.x - colt_sus.x;
	// copiere imagine
	alocare_matrice_pixel(&imagine_utilizabila, nr_linii, nr_coloane);
	int i, j;
	for (i = colt_sus.y; i < colt_jos.y; i++) {
		for (j = colt_sus.x ; j < colt_jos.x; j++) {
			imagine_utilizabila[i - colt_sus.y][j - colt_sus.x] =
			(*imagine)[i][j];

		}

	}
	// recontruire imagine dupa crop
	eliberare_matrice_pixel(linii, &(*imagine));
	alocare_matrice_pixel(&(*imagine), nr_linii, nr_coloane);
	for (i = 0; i < nr_linii; i++) {
		for (j = 0 ; j < nr_coloane; j++) {
			(*imagine)[i][j] = imagine_utilizabila[i][j];

		}

	}
	eliberare_matrice_pixel(nr_linii, &imagine_utilizabila);
}

// functie in care se apeleaza taierea imaginilor
void CROP(int ***imagine, struct pixel ***imagine_culori,
		  struct colt *colt_sus, struct colt *colt_jos,
		  int *linii, int *coloane, char tip[3])
{
	int nr_linii = (*colt_jos).y - (*colt_sus).y;
	int nr_coloane = (*colt_jos).x - (*colt_sus).x;
	// taierea imaginilor in funtie de tipul lor (in tonuri de gri
	// sau color)
	if (strcmp(tip, "P2") == 0 || strcmp(tip, "P5") == 0) {
		CROP_INT(&(*imagine), *linii, (*colt_sus), (*colt_jos));
	} else if (strcmp(tip, "P3") == 0 || strcmp(tip, "P6") == 0) {
		CROP_PIXEL(&(*imagine_culori), *linii, (*colt_sus), (*colt_jos));
	}
	// modificarea coordonatelor dupa taiere
	(*colt_sus).x = 0;
	(*colt_sus).y = 0;
	(*colt_jos).y = nr_linii;
	(*colt_jos).x = nr_coloane;
	*linii = nr_linii;
	*coloane = nr_coloane;
	printf("Image cropped\n");

}

// functie pentru rotirea cu 90 de grade a imaginilor in tonuri de gri
void ROTATE_90_INT(int ***imagine, int **imagine_copie, struct colt *colt_sus,
				   struct colt *colt_jos, int nr_linii, int nr_coloane,
				   int selectata, int *linii, int *coloane)
{
	int i, j;
	if (selectata) {
		// alocare matrice noua cu coordonate diferite
		// corespunzatoare rotirii cu 90 de grade
		eliberare_matrice_int(nr_linii, &(*imagine));
		alocare_matrice_int(&(*imagine), nr_coloane, nr_linii);
		for (j = 0; j < nr_coloane; j++) {
			for (i = nr_linii - 1; i > -1; i--) {
				(*imagine)[j][nr_linii - 1 - i] = imagine_copie[i][j];

			}
		}
		int aux;
		aux = (*colt_jos).x;
		(*colt_jos).x = (*colt_jos).y;
		(*colt_jos).y = aux;
		aux = (*colt_sus).x;
		(*colt_sus).x = (*colt_sus).y;
		(*colt_sus).y = aux;
		*linii = (*colt_jos).y - (*colt_sus).y;
		*coloane = (*colt_jos).x - (*colt_sus).x;

	} else {
		for (j = 0; j < nr_coloane; j++) {
			for (i = nr_linii - 1; i > -1; i--) {
				int x;
				x = nr_linii - 1 - i + (*colt_sus).x;
				(*imagine)[j + (*colt_sus).y][x] = imagine_copie[i][j];

			}
		}
	}

}

// functie pentru rotirea cu 90 de grade a imaginilor color
void ROTATE_90_PIXEL(struct pixel ***imagine, struct pixel **imagine_copie,
					 struct colt *colt_sus, struct colt *colt_jos, int nr_linii,
					 int nr_coloane, int selectata, int *linii, int *coloane)
{
	int i, j;
	if (selectata) {
		// alocare matrice noua cu coordonate diferite
		// corespunzatoare rotirii cu 90 de grade
		eliberare_matrice_pixel(nr_linii, &(*imagine));
		alocare_matrice_pixel(&(*imagine), nr_coloane, nr_linii);
		for (j = 0; j < nr_coloane; j++) {
			for (i = nr_linii - 1; i > -1; i--) {
				(*imagine)[j][nr_linii - 1 - i] = imagine_copie[i][j];

			}
		}
		int aux;
		aux = (*colt_jos).x;
		(*colt_jos).x = (*colt_jos).y;
		(*colt_jos).y = aux;
		aux = (*colt_sus).x;
		(*colt_sus).x = (*colt_sus).y;
		(*colt_sus).y = aux;
		*linii = (*colt_jos).y - (*colt_sus).y;
		*coloane = (*colt_jos).x - (*colt_sus).x;

	} else {
		for (j = 0; j < nr_coloane; j++) {
			for (i = nr_linii - 1; i > -1; i--) {
				int x;
				x = nr_linii - 1 - i + (*colt_sus).x;
				(*imagine)[j + (*colt_sus).y][x] = imagine_copie[i][j];

			}
		}
	}

}

// functie in care se apeleaza rotirea cu 90 de grade a imaginilor
void ROTATE_90(int ***imagine, struct pixel ***imagine_culori,
			   struct colt *colt_sus, struct colt *colt_jos, char tip[3],
			   int *linii, int *coloane)
{
	// verifica tipul de imagine pentru care se efectueaza rotirea
	// in tonuri de gri sau color
	int i, j;
	int nr_linii = (*colt_jos).y - (*colt_sus).y;
	int nr_coloane = (*colt_jos).x - (*colt_sus).x;
	if (nr_linii != nr_coloane &&
		!(nr_linii == *linii && nr_coloane == *coloane)) {
		printf("The selection must be square\n");
		return;
	}
	int selectata = 0;
	if (nr_linii == *linii && nr_coloane == *coloane) {
		selectata = 1;
	}
	if (strcmp(tip, "P1") == 0 || strcmp(tip, "P2") == 0 ||
		strcmp(tip, "P4") == 0 || strcmp(tip, "P5") == 0) {
		int **imagine_copie;
		alocare_matrice_int(&imagine_copie, nr_linii, nr_coloane);
		for (i = (*colt_sus).y; i < (*colt_jos).y; i++) {
			for (j = (*colt_sus).x; j < (*colt_jos).x; j++) {
				imagine_copie[i - (*colt_sus).y][j - (*colt_sus).x] =
				(*imagine)[i][j];

			}
		}
		ROTATE_90_INT(&(*imagine), imagine_copie, &(*colt_sus),
					  &(*colt_jos), nr_linii, nr_coloane, selectata,
					  &(*linii), &(*coloane));
	} else if (strcmp(tip, "P3") == 0 || strcmp(tip, "P6") == 0) {
		struct pixel **imagine_copie;
		alocare_matrice_pixel(&imagine_copie, nr_linii, nr_coloane);
		for (i = (*colt_sus).y; i < (*colt_jos).y; i++) {
			for (j = (*colt_sus).x; j < (*colt_jos).x; j++) {
				imagine_copie[i - (*colt_sus).y][j - (*colt_sus).x] =
				(*imagine_culori)[i][j];

			}
		}
		ROTATE_90_PIXEL(&(*imagine_culori), imagine_copie, &(*colt_sus),
						&(*colt_jos), nr_linii, nr_coloane, selectata,
						&(*linii), &(*coloane));

	}
}

// functia care roteste in functie de unghiul cerut
void ROTATE(int ***imagine, struct pixel ***imagine_culori,
			struct colt *colt_sus, struct colt *colt_jos, char *p,
			char tip[3], int *linii, int *coloane)
{
	int unghi;
	p = strtok(NULL, " ");
	p[strcspn(p, "\n")] = '\0';
	unghi = atoi(p);
	p[strcspn(p, "\n")] = '\0';
	// verificarea unghiului si rotirea cu 90 de grade de cate ori este
	// nevoie pentru a realiza o rotire corecta
	if (unghi == 90 || unghi == -270) {
		ROTATE_90(&(*imagine), &(*imagine_culori), &(*colt_sus),
				  &(*colt_jos), tip, &(*linii), &(*coloane));
	} else if (unghi == 180 || unghi == -180) {
		ROTATE_90(&(*imagine), &(*imagine_culori), &(*colt_sus),
				  &(*colt_jos), tip, &(*linii), &(*coloane));
		ROTATE_90(&(*imagine), &(*imagine_culori), &(*colt_sus),
				  &(*colt_jos), tip, &(*linii), &(*coloane));
	} else if (unghi == 270 || unghi == -90) {
		ROTATE_90(&(*imagine), &(*imagine_culori), &(*colt_sus),
				  &(*colt_jos), tip, &(*linii), &(*coloane));
		ROTATE_90(&(*imagine), &(*imagine_culori), &(*colt_sus),
				  &(*colt_jos), tip, &(*linii), &(*coloane));
		ROTATE_90(&(*imagine), &(*imagine_culori), &(*colt_sus),
				  &(*colt_jos), tip, &(*linii), &(*coloane));
	} else if (unghi == 360 || unghi == -360 || unghi == 0) {

	} else {
		printf("Unsupported rotation angle\n");
		return;
	}
	printf("Rotated %d\n", unghi);
}

// functie pentru popularea unei matrice
void umplere_matrice(struct pixel ***matrice, int linii, int coloane)
{
	int i, j;
	for (i = 0; i < linii; i++) {
		for (j = 0; j < coloane; j++) {
			(*matrice)[i][j] = (struct pixel){0, 0, 0};

		}
	}
}

// functie care calculeaza inmultirea unei matrice 3 x 3 din cadrul imaginii
// cu matricea kernel
void calculare_apply(struct pixel ***imagine_culori,
					 struct pixel **matrice, float kernel[3][3], int y, int x,
					 int i, int j)
{
	(*imagine_culori)[y][x].r = kernel[0][0] * matrice[i][j].r +
								kernel[0][1] * matrice[i][j + 1].r +
								kernel[0][2] * matrice[i][j + 2].r +
								kernel[1][0] * matrice[i + 1][j].r +
								kernel[1][1] * matrice[i + 1][j + 1].r +
								kernel[1][2] * matrice[i + 1][j + 2].r +
								kernel[2][0] * matrice[i + 2][j].r +
								kernel[2][1] * matrice[i + 2][j + 1].r +
								kernel[2][2] * matrice[i + 2][j + 2].r;
	(*imagine_culori)[y][x].g = kernel[0][0] * matrice[i][j].g +
								kernel[0][1] * matrice[i][j + 1].g +
								kernel[0][2] * matrice[i][j + 2].g +
								kernel[1][0] * matrice[i + 1][j].g +
								kernel[1][1] * matrice[i + 1][j + 1].g +
								kernel[1][2] * matrice[i + 1][j + 2].g +
								kernel[2][0] * matrice[i + 2][j].g +
								kernel[2][1] * matrice[i + 2][j + 1].g +
								kernel[2][2] * matrice[i + 2][j + 2].g;
	(*imagine_culori)[y][x].b = kernel[0][0] * matrice[i][j].b +
								kernel[0][1] * matrice[i][j + 1].b +
								kernel[0][2] * matrice[i][j + 2].b +
								kernel[1][0] * matrice[i + 1][j].b +
								kernel[1][1] * matrice[i + 1][j + 1].b +
								kernel[1][2] * matrice[i + 1][j + 2].b +
								kernel[2][0] * matrice[i + 2][j].b +
								kernel[2][1] * matrice[i + 2][j + 1].b +
								kernel[2][2] * matrice[i + 2][j + 2].b;
	if ((*imagine_culori)[y][x].r > 255) {
		(*imagine_culori)[y][x].r = 255;
	}
	if ((*imagine_culori)[y][x].g > 255) {
		(*imagine_culori)[y][x].g = 255;
	}
	if ((*imagine_culori)[y][x].b > 255) {
		(*imagine_culori)[y][x].b = 255;
	}
	if ((*imagine_culori)[y][x].r < 0) {
		(*imagine_culori)[y][x].r = 0;
	}
	if ((*imagine_culori)[y][x].g < 0) {
		(*imagine_culori)[y][x].g = 0;
	}
	if ((*imagine_culori)[y][x].b < 0) {
		(*imagine_culori)[y][x].b = 0;
	}
}

// functie care aplica unei imaginii efectul de EDGE
void APPLY_EDGE(struct pixel ***imagine_culori, struct colt colt_sus,
				struct colt colt_jos, int linii, int coloane)
{
	// definirea unui kernel
	float kernel[3][3] = {
		{-1, -1, -1},
		{-1,  8, -1},
		{-1, -1, -1}
	};
	int i, j;
	struct pixel **matrice = NULL;
	int nr_linii = colt_jos.y - colt_sus.y + 2;
	int nr_coloane = colt_jos.x - colt_sus.x + 2;
	alocare_matrice_pixel(&matrice, nr_linii, nr_coloane);
	umplere_matrice(&matrice, nr_linii, nr_coloane);
	int inceput_i, sfarsit_i, inceput_j, sfarsit_j;
	if (colt_sus.y - 1 >= 0) {
		inceput_i = 0;
	} else {
		inceput_i = 1;
	}
	if (colt_sus.x - 1 >= 0) {
		inceput_j = 0;
	} else {
		inceput_j = 1;
	}
	if (colt_jos.y + 1 <= linii) {
		sfarsit_i = nr_linii - 2;
	} else {
		sfarsit_i = nr_linii - 3;
	}
	if (colt_jos.x + 1 <= coloane) {
		sfarsit_j = nr_coloane - 2;
	} else {
		sfarsit_j = nr_coloane - 3;
	}
	for (i = colt_sus.y - 1; i < colt_jos.y + 1; i++) {
		for (j = colt_sus.x - 1 ; j < colt_jos.x + 1; j++) {
			int ii = 0, jj = 0;
			if (i == -1) {
				ii = 1;
			}
			if (j == -1) {
				jj = 1;
			}
			if (i == linii) {
				ii = -1;
			}
			if (j == coloane) {
				jj = -1;
			}
			matrice[i - colt_sus.y + 1][j - colt_sus.x + 1] =
			(*imagine_culori)[i + ii][j + jj];

		}
	}
	// se calculeaza inmmultirea kernelului cu sectiuni din imagine
	for (i = inceput_i; i < sfarsit_i; i++) {
		for (j = inceput_j; j < sfarsit_j; j++) {
			int y, x;
			y = i + colt_sus.y;
			x = j + colt_sus.x;
			calculare_apply(&(*imagine_culori), matrice,
							kernel, y, x, i, j);
		}
	}
	eliberare_matrice_pixel(nr_linii, &matrice);
	printf("APPLY EDGE done\n");
}

// pentru urmatoarele 3 functii (cele cu apply) se realizeaza acelasi lucru
// ca in functia de mai sus, difera doar kernelul

// functie care aplica unei imaginii efectul de SHARPEN
void APPLY_SHARPEN(struct pixel ***imagine_culori, struct colt colt_sus,
				   struct colt colt_jos, int linii, int coloane)
{
	float kernel[3][3] = {
		{0, -1, 0},
		{-1,  5, -1},
		{0, -1, 0}
	};
	int i, j;
	struct pixel **matrice = NULL;
	int nr_linii = colt_jos.y - colt_sus.y + 2;
	int nr_coloane = colt_jos.x - colt_sus.x + 2;
	alocare_matrice_pixel(&matrice, nr_linii, nr_coloane);
	umplere_matrice(&matrice, nr_linii, nr_coloane);
	int inceput_i, sfarsit_i, inceput_j, sfarsit_j;
	if (colt_sus.y - 1 >= 0) {
		inceput_i = 0;
	} else {
		inceput_i = 1;
	}
	if (colt_sus.x - 1 >= 0) {
		inceput_j = 0;
	} else {
		inceput_j = 1;
	}
	if (colt_jos.y + 1 <= linii) {
		sfarsit_i = nr_linii - 2;
	} else {
		sfarsit_i = nr_linii - 3;
	}
	if (colt_jos.x + 1 <= coloane) {
		sfarsit_j = nr_coloane - 2;
	} else {
		sfarsit_j = nr_coloane - 3;
	}
	for (i = colt_sus.y - 1; i < colt_jos.y + 1; i++) {
		for (j = colt_sus.x - 1 ; j < colt_jos.x + 1; j++) {
			int ii = 0, jj = 0;
			if (i == -1) {
				ii = 1;
			}
			if (j == -1) {
				jj = 1;
			}
			if (i == linii) {
				ii = -1;
			}
			if (j == coloane) {
				jj = -1;
			}
			matrice[i - colt_sus.y + 1][j - colt_sus.x + 1] =
			(*imagine_culori)[i + ii][j + jj];
		}
	}
	for (i = inceput_i; i < sfarsit_i; i++) {
		for (j = inceput_j; j < sfarsit_j; j++) {
			int y, x;
			y = i + colt_sus.y;
			x = j + colt_sus.x;
			calculare_apply(&(*imagine_culori), matrice,
							kernel, y, x, i, j);
		}
	}
	eliberare_matrice_pixel(nr_linii, &matrice);
	printf("APPLY SHARPEN done\n");
}

// functie care aplica unei imaginii efectul de BLUR
void APPLY_BLUR(struct pixel ***imagine_culori, struct colt colt_sus,
				struct colt colt_jos, int linii, int coloane)
{
	float kernel[3][3] = {
		{(float)1 / 9, (float)1 / 9, (float)1 / 9},
		{(float)1 / 9,  (float)1 / 9, (float)1 / 9},
		{(float)1 / 9, (float)1 / 9, (float)1 / 9}
	};
	int i, j;
	struct pixel **matrice = NULL;
	int nr_linii = colt_jos.y - colt_sus.y + 2;
	int nr_coloane = colt_jos.x - colt_sus.x + 2;
	alocare_matrice_pixel(&matrice, nr_linii, nr_coloane);
	umplere_matrice(&matrice, nr_linii, nr_coloane);
	int inceput_i, sfarsit_i, inceput_j, sfarsit_j;
	if (colt_sus.y - 1 >= 0) {
		inceput_i = 0;
	} else {
		inceput_i = 1;
	}
	if (colt_sus.x - 1 >= 0) {
		inceput_j = 0;
	} else {
		inceput_j = 1;
	}
	if (colt_jos.y + 1 <= linii) {
		sfarsit_i = nr_linii - 2;
	} else {
		sfarsit_i = nr_linii - 3;
	}
	if (colt_jos.x + 1 <= coloane) {
		sfarsit_j = nr_coloane - 2;
	} else {
		sfarsit_j = nr_coloane - 3;
	}
	for (i = colt_sus.y - 1; i < colt_jos.y + 1; i++) {
		for (j = colt_sus.x - 1 ; j < colt_jos.x + 1; j++) {
			int ii = 0, jj = 0;
			if (i == -1) {
				ii = 1;
			}
			if (j == -1) {
				jj = 1;
			}
			if (i == linii) {
				ii = -1;
			}
			if (j == coloane) {
				jj = -1;
			}
			matrice[i - colt_sus.y + 1][j - colt_sus.x + 1] =
			(*imagine_culori)[i + ii][j + jj];

		}
	}
	for (i = inceput_i; i < sfarsit_i; i++) {
		for (j = inceput_j; j < sfarsit_j; j++) {
			int y, x;
			y = i + colt_sus.y;
			x = j + colt_sus.x;
			calculare_apply(&(*imagine_culori), matrice,
							kernel, y, x, i, j);
		}
	}
	eliberare_matrice_pixel(nr_linii, &matrice);
	printf("APPLY BLUR done\n");
}

// functie care aplica unei imaginii efectul de GAUSSIAN_BLUR
void APPLY_GAUSSIAN_BLUR(struct pixel ***imagine_culori, struct colt colt_sus,
						 struct colt colt_jos, int linii, int coloane)
{
	float kernel[3][3] = {
		{(float)1 / 16, (float)1 / 8, (float)1 / 16},
		{(float)1 / 8,  (float)1 / 4, (float)1 / 8},
		{(float)1 / 16, (float)1 / 8, (float)1 / 16}
	};
	int i, j;
	struct pixel **matrice = NULL;
	int nr_linii = colt_jos.y - colt_sus.y + 2;
	int nr_coloane = colt_jos.x - colt_sus.x + 2;
	alocare_matrice_pixel(&matrice, nr_linii, nr_coloane);
	umplere_matrice(&matrice, nr_linii, nr_coloane);
	int inceput_i, sfarsit_i, inceput_j, sfarsit_j;
	if (colt_sus.y - 1 >= 0) {
		inceput_i = 0;
	} else {
		inceput_i = 1;
	}
	if (colt_sus.x - 1 >= 0) {
		inceput_j = 0;
	} else {
		inceput_j = 1;
	}
	if (colt_jos.y + 1 <= linii) {
		sfarsit_i = nr_linii - 2;
	} else {
		sfarsit_i = nr_linii - 3;
	}
	if (colt_jos.x + 1 <= coloane) {
		sfarsit_j = nr_coloane - 2;
	} else {
		sfarsit_j = nr_coloane - 3;
	}
	for (i = colt_sus.y - 1; i < colt_jos.y + 1; i++) {
		for (j = colt_sus.x - 1 ; j < colt_jos.x + 1; j++) {
			int ii = 0, jj = 0;
			if (i == -1) {
				ii = 1;
			}
			if (j == -1) {
				jj = 1;
			}
			if (i == linii) {
				ii = -1;
			}
			if (j == coloane) {
				jj = -1;
			}
			matrice[i - colt_sus.y + 1][j - colt_sus.x + 1] =
			(*imagine_culori)[i + ii][j + jj];

		}
	}
	for (i = inceput_i; i < sfarsit_i; i++) {
		for (j = inceput_j; j < sfarsit_j; j++) {
			int y, x;
			y = i + colt_sus.y;
			x = j + colt_sus.x;
			calculare_apply(&(*imagine_culori), matrice,
							kernel, y, x, i, j);
		}
	}
	eliberare_matrice_pixel(nr_linii, &matrice);
	printf("APPLY GAUSSIAN_BLUR done\n");
}

// functia care realizeaza egalizarea imaginilor in tonuri de gri
void EQUALIZE(int ***imagine, int linii, int coloane, char tip[3])
{
	if (strcmp(tip, "P1") != 0 && strcmp(tip, "P2") != 0 &&
		strcmp(tip, "P4") != 0 && strcmp(tip, "P5") != 0) {
		printf("Black and white image needed\n");
		return;
	}
	int frecventa[256] = {0};
	int i, j, k;
	for (i = 0; i < linii; i++) {
		for (j = 0; j < coloane; j++) {
			frecventa[(*imagine)[i][j]]++;

		}

	}
	for (i = 0; i < linii; i++) {
		for (j = 0; j < coloane; j++) {
			int sum = 0;
			for (k = 0; k < (*imagine)[i][j] + 1; k++) {
				sum = sum + frecventa[k];
			}
			float pixel_nou;
			pixel_nou = (float)sum / (linii * coloane) * 255;
			(*imagine)[i][j] = (int)pixel_nou;

		}

	}
	printf("Equalize done\n");
}

// functia care afiseaza histograma imaginilor in tonuri de gri
void HISTOGRAM(int ***imagine, int linii, int coloane, char tip[3], char *p)
{
	// se verifica daca parametrii comenzii de HISTOGRAM sunt exact 2
	int bin, max_bin, este_numar = 0;
	p = strtok(NULL, " ");
	if (p) {
		p[strcspn(p, "\n")] = '\0';
	}
	este_numar = verificare_numar(p);
	if (p && este_numar) {
		max_bin = atoi(p);
	} else {
		printf("Invalid command\n");
		return;
	}
	este_numar = 0;
	p = strtok(NULL, " ");
	if (p) {
		p[strcspn(p, "\n")] = '\0';
	}
	este_numar = verificare_numar(p);
	if (p && este_numar) {
		bin = atoi(p);
	} else {
		printf("Invalid command\n");
		return;
	}
	p = strtok(NULL, " ");
	if (p) {
		printf("Invalid command\n");
		return;
	}
	if (strcmp(tip, "P1") != 0 && strcmp(tip, "P2") != 0 &&
		strcmp(tip, "P4") != 0 && strcmp(tip, "P5") != 0) {
		printf("Black and white image needed\n");
		return;
	}
	// calcularea efectiva a histogramei corespunzatoare imaginii
	int frecventa[256] = {0}, binuri[256] = {0};
	int i, j;
	for (i = 0; i < linii; i++) {
		for (j = 0; j < coloane; j++) {
			frecventa[(*imagine)[i][j]]++;

		}

	}
	int dimensiune_bin = 256 / bin;
	for (i = 0; i < 256; i++) {
		binuri[i / dimensiune_bin] = binuri[i / dimensiune_bin] + frecventa[i];
	}
	int max_frecventa = binuri[0];
	for (i = 1; i < 256; i++) {
		if (binuri[i] > max_frecventa) {
			max_frecventa = binuri[i];
		}
	}
	// afisarea histogramei
	for (i = 0; i < bin; i++) {
		printf("%d\t|\t", binuri[i] * max_bin / max_frecventa);
		for (j = 0; j < binuri[i] * max_bin / max_frecventa; j++) {
			printf("*");
		}
		printf("\n");
	}

}

// functia de selectare totala a imaginii
void SELECT_ALL(struct colt *colt_sus, struct colt *colt_jos,
				int linii, int coloane)
{
	// modificarea coordonatelor
	(*colt_sus).x = 0;
	(*colt_sus).y = 0;
	(*colt_jos).x = coloane;
	(*colt_jos).y = linii;
}

// functie ce realizeaza eliberarea memoriei utilizate
void EXIT(int ***imagine, struct pixel ***imagine_culori, int linii)
{
	if (*imagine) {
		eliberare_matrice_int(linii, &(*imagine));
	}
	if (*imagine_culori) {
		eliberare_matrice_pixel(linii, &(*imagine_culori));
	}
}

// functie care realizeaza selectia dupa ce
// se verifica daca imaginea este incarcata
void SELECT_incarcata(struct colt *colt_sus, struct colt *colt_jos,
					  int *linii, int *coloane, char *p, int incarcata)
{
	if (incarcata == 0) {
		printf("No image loaded\n");
	} else {
		int x1 = -1, x2 = -1, y1 = -1, y2 = -1;
		p = strtok(NULL, " ");
		if (p[0] != 'A') {
			SELECT(&(*colt_sus), &(*colt_jos), x1, y1, x2, y2,
				   *linii, *coloane, p);
		} else {
			SELECT_ALL(&(*colt_sus), &(*colt_jos), *linii, *coloane);
			printf("Selected ALL\n");
		}
	}
}

// functie care realizeaz salvarea dupa ce
// se verifica daca imaginea este incarcata
void SAVE_incarcata(char *fisier, int **imagine, struct pixel **imagine_culori,
					int linii, int coloane, char *p, char *tip)
{
	p = strtok(NULL, " ");
	memmove(fisier, p, strlen(p) + 1);
	p = strtok(NULL, " ");
	SAVE(fisier, imagine, imagine_culori, linii, coloane, p, tip);
}

// functie care realizeaz aplicarea unui fltru dupa ce se verifica daca
// imaginea este incarcata si se selecteaza filtrul dorit
void APPLY_incarcata(struct pixel ***imagine_culori, struct colt colt_sus,
					 struct colt colt_jos, int linii, int coloane,
					 char tip[3], char *p)
{
	p = strtok(NULL, " ");
		char functie[100];
		if (!p) {
			printf("Invalid command\n");
		} else {
			memmove(functie, p, strlen(p) + 1);
			functie[strcspn(functie, "\n")] = '\0';
		}
		if (strcmp(tip, "P3") != 0 && strcmp(tip, "P6") != 0 && p) {
			printf("Easy, Charlie Chaplin\n");
		} else if (strcmp(functie, "EDGE") == 0) {
			APPLY_EDGE(&(*imagine_culori), colt_sus, colt_jos,
					   linii, coloane);
		} else if (strcmp(functie, "SHARPEN") == 0) {
			APPLY_SHARPEN(&(*imagine_culori), colt_sus, colt_jos,
						  linii, coloane);
		} else if (strcmp(functie, "BLUR") == 0) {
			APPLY_BLUR(&(*imagine_culori), colt_sus, colt_jos,
					   linii, coloane);
		} else if (strcmp(functie, "GAUSSIAN_BLUR") == 0) {
			APPLY_GAUSSIAN_BLUR(&(*imagine_culori), colt_sus, colt_jos,
								linii, coloane);
		} else if (p) {
			printf("APPLY parameter invalid\n");
		}
}

// se apeleaza subprogramele in functie de datele de intrare
int main(void)
{
	int **imagine = NULL, linii, coloane, incarcata = 0;
	char tip[3], actiune[1000] = "", comanda[1000], fisier[100], *p;
	struct colt colt_sus, colt_jos;
	struct pixel **imagine_culori = NULL;
	fgets(comanda, sizeof(comanda), stdin);
	p = strtok(comanda, " ");
	p[strcspn(p, "\n")] = '\0';
	memmove(comanda, p, strlen(p) + 1);
	// cat timp comanda nu este una de "EXIT" codul ruleaza si asteapta
	// input-uri noi, apoi verifica ce tip de comanda este si ruleaza
	// subprogramul necesar realizarii actiunii dorite de utilizator
	while (strcmp(comanda, "EXIT") != 0) {
		memmove(actiune, p, strlen(p) + 1);
		if (strcmp(actiune, "LOAD") == 0) {
			p = strtok(NULL, " ");
			memmove(fisier, p, strlen(p) + 1);
			LOAD(fisier, &imagine, &imagine_culori, &linii,
				 &coloane, tip, &incarcata);
			SELECT_ALL(&colt_sus, &colt_jos, linii, coloane);
		} else if (strcmp(actiune, "SELECT") == 0) {
			SELECT_incarcata(&colt_sus, &colt_jos,
							 &linii, &coloane, p, incarcata);

		} else if (strcmp(actiune, "SAVE") == 0) {
			if (incarcata == 0) {
				printf("No image loaded\n");
			} else {
				SAVE_incarcata(fisier, imagine, imagine_culori,
							   linii, coloane, p, tip);
			}
		} else if (strcmp(comanda, "CROP") == 0) {
			if (incarcata == 0) {
				printf("No image loaded\n");
			} else {
				CROP(&imagine, &imagine_culori, &colt_sus, &colt_jos,
					 &linii, &coloane, tip);
			}
		} else if (strcmp(actiune, "ROTATE") == 0) {
			if (incarcata == 0) {
				printf("No image loaded\n");
			} else {
				ROTATE(&imagine, &imagine_culori, &colt_sus, &colt_jos,
					   p, tip, &linii, &coloane);
			}
		} else if (strcmp(actiune, "APPLY") == 0) {
			if (incarcata == 0) {
				printf("No image loaded\n");
			} else {
				APPLY_incarcata(&imagine_culori, colt_sus, colt_jos,
								linii, coloane, tip, p);
			}
		} else if (strcmp(comanda, "EQUALIZE") == 0) {
			if (incarcata == 0) {
				printf("No image loaded\n");
			} else {
				EQUALIZE(&imagine, linii, coloane, tip);
			}

		} else if (strcmp(comanda, "HISTOGRAM") == 0) {
			if (incarcata == 0) {
				printf("No image loaded\n");
			} else {
				HISTOGRAM(&imagine, linii, coloane, tip, p);
			}
		} else {
			printf("Invalid command\n");
		}
		fgets(comanda, sizeof(comanda), stdin);
		p = strtok(comanda, " ");
		p[strcspn(p, "\n")] = '\0';
		memmove(comanda, p, strlen(p) + 1);
		if (strcmp(comanda, "EXIT") == 0 && incarcata == 0) {
			printf("No image loaded\n");
			break;
		}
	}
	EXIT(&imagine, &imagine_culori, linii);
	return 0;
}
