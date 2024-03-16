#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#define N 256

/// definesc o strcutura pentru imaginile color
typedef struct {
	int rosu;
	int verde;
	int albastru;
} imgcolor;

/// definesc o structura pentru a stoca toate informatiile legate de
///imaginea pe care o proceses
typedef struct {
	int nrlinii;
	int nrcoloane;
	int tip;
	int C_nrcoloane;
	int C_nrlinii;
	int C_tip;
	int x1_selectie;
	int y1_selectie;
	int x2_selectie;
	int y2_selectie;
} info_mat;

/// definesc o structura pt a retine care este zona selectata
typedef struct {
	int coord_x1;
	int coord_y1;
	int coord_x2;
	int coord_y2;
} coord;

/// eliberare_resurse este o functie ce elibereaza tot spatiul alocat pt a
/// retine matricea corespunzatoare imaginei incarcate
/// mat_imgcolor este matricea imaginei daca aceasta este color
/// C_mat_imgcolor este zona selectata pt a face modificari imaginei color
/// mat_imgalbneg este matricea imaginei daca aceasta este grayscale
/// C_mat_imgalbneg este zona selectata pt a face modificari imagini grayscale
/// infom reprezinta toate informatiile necesare pt a procesa imaginea
void eliberare_resurse(imgcolor ***mat_imgcolor, imgcolor ***C_mat_imgcolor,
					   int ***mat_imgalbneg, int ***C_mat_imgalbneg,
					   info_mat *infom)
{
	if (infom->tip == 3 || infom->tip == 6) {
		for (int i = 0; i < infom->nrlinii; i++)
			free((*mat_imgcolor)[i]);
		free(*mat_imgcolor);
		*mat_imgcolor = NULL;
		for (int i = 0; i < infom->C_nrlinii; i++)
			free((*C_mat_imgcolor)[i]);
		free(*C_mat_imgcolor);
		*C_mat_imgcolor = NULL;
	} else {
		for (int i = 0; i < infom->nrlinii; i++)
			free((*mat_imgalbneg)[i]);
		free(*mat_imgalbneg);
		*mat_imgalbneg = NULL;
		for (int i = 0; i < infom->C_nrlinii; i++)
			free((*C_mat_imgalbneg)[i]);
		free(*C_mat_imgalbneg);
		*C_mat_imgalbneg = NULL;
	}
	infom->C_nrcoloane = 0;
	infom->C_nrlinii = 0;
	infom->C_tip = 0;
	infom->nrcoloane = 0;
	infom->nrlinii = 0;
	infom->tip = 0;
	infom->x1_selectie = 0;
	infom->y1_selectie = 0;
	infom->x2_selectie = 0;
	infom->y2_selectie = 0;
}

/// functia alocare_matalbneg aloca resurse pt citirea unei imagini grayscale
/// mat_imgalbneg este matricea ce corespunde imaginei grayscale
/// C_mat_imgalbneg este zona de selectie care initial este toata imaginea
/// lin este numarul de linii
/// col este numarul de coloane
int alocare_matalbneg(int ***mat_imgalbneg, int ***C_mat_imgalbneg, int lin,
					  int col)
{
	*mat_imgalbneg = (int **)malloc(lin * sizeof(int *));
	if (!(*mat_imgalbneg)) {
		printf("Error allocating memory\n");
		return -1;
	}
	*C_mat_imgalbneg = (int **)malloc(lin * sizeof(int *));
	if (!(*C_mat_imgalbneg)) {
		printf("Error allocating memory\n");
		return -1;
	}
	for (int i = 0; i < lin; i++) {
		(*mat_imgalbneg)[i] = (int *)malloc(col * sizeof(int));
		if (!(*mat_imgalbneg)[i]) {
			printf("Error allocating memory\n");
			return -1;
		}
	}
	for (int i = 0; i < lin; i++) {
		(*C_mat_imgalbneg)[i] = (int *)malloc(col * sizeof(int));
		if (!(*C_mat_imgalbneg)[i]) {
			printf("Error allocating memory\n");
			return -1;
		}
	}
	return 0;
}

/// functia alocare_matcolor aloca resurse pt citirea unei imagini color
/// mat_imgcolor este matricea ce corespunde imaginei color
/// C_mat_imgcolor este zona de selectie care initial este toata imaginea
/// lin este numarul de linii
/// col este numarul de coloane
int alocare_matcolor(imgcolor ***mat_imgcolor, imgcolor ***C_mat_imgcolor,
					 int lin, int col)
{
	*mat_imgcolor = (imgcolor **)malloc(lin * sizeof(imgcolor *));
	if (!(*mat_imgcolor)) {
		printf("Error allocating memory\n");
		return -1;
	}
	*C_mat_imgcolor = (imgcolor **)malloc(lin * sizeof(imgcolor *));
	if (!(*C_mat_imgcolor)) {
		printf("Error allocating memory\n");
		return -1;
	}
	for (int i = 0; i < lin; i++) {
		(*mat_imgcolor)[i] = (imgcolor *)malloc(col * sizeof(imgcolor));
		if (!(*mat_imgcolor)[i]) {
			printf("Error allocating memory\n");
			return -1;
		}
	}
	for (int i = 0; i < lin; i++) {
		(*C_mat_imgcolor)[i] = (imgcolor *)malloc
		(col * sizeof(imgcolor));
		if (!(*C_mat_imgcolor)[i]) {
			printf("Error allocating memory\n");
			return -1;
		}
	}
	return 0;
}

/// citirebin_color citeste binar imaginea color
/// mat_imgcolor este matricea ce corespunde imaginei color
/// C_mat_imgcolor este zona de selectie care initial este toata imaginea
/// lin este numarul de linii
/// col este numarul de coloane
/// prin in se citeste din fisier matricea
void citierebin_color(imgcolor ***mat_imgcolor, imgcolor ***C_mat_imgcolor,
					  int lin, int col, FILE *in)
{
	unsigned char aux_citire;
	for (int i = 0; i < lin; i++) {
		for (int j = 0; j < col; j++) {
			fread(&aux_citire, sizeof(unsigned char), 1, in);
			(*mat_imgcolor)[i][j].rosu = aux_citire;
			fread(&aux_citire, sizeof(unsigned char), 1, in);
			(*mat_imgcolor)[i][j].verde = aux_citire;
			fread(&aux_citire, sizeof(unsigned char), 1, in);
			(*mat_imgcolor)[i][j].albastru = aux_citire;
		}
	}
	for (int i = 0; i < lin; i++) {
		for (int j = 0; j < col; j++) {
			(*C_mat_imgcolor)[i][j].rosu = (*mat_imgcolor)[i][j].rosu;
			(*C_mat_imgcolor)[i][j].verde = (*mat_imgcolor)[i][j].verde;
			(*C_mat_imgcolor)[i][j].albastru =
			(*mat_imgcolor)[i][j].albastru;
		}
	}
}

/// citirebin_albneg citeste binar imaginea grayscale
/// mat_imgalbneg este matricea ce corespunde imaginei grayscale
/// C_mat_imgalbneg este zona de selectie care initial este toata imaginea
/// lin este numarul de linii
/// col este numarul de coloane
/// prin in se citeste din fisier matricea
void citirebin_albneg(int ***mat_imgalbneg, int ***C_mat_imgalbneg,
					  int lin, int col, FILE *in)
{
	unsigned char aux_citire;
	for (int i = 0; i < lin; i++) {
		for (int j = 0; j < col; j++) {
			fread(&aux_citire, sizeof(unsigned char), 1, in);
			(*mat_imgalbneg)[i][j] = aux_citire;
		}
	}
	for (int i = 0; i < lin; i++)
		for (int j = 0; j < col; j++)
			(*C_mat_imgalbneg)[i][j] = (*mat_imgalbneg)[i][j];
}

/// citireascii_color citeste matricea imaginii color in format ascii
/// mat_imgcolor este matricea ce corespunde imaginei color
/// C_mat_imgcolor este zona de selectie care initial este toata imaginea
/// lin este numarul de linii
/// col este numarul de coloane
/// prin in se citeste din fisier matricea
void citireascii_color(imgcolor ***mat_imgcolor, imgcolor ***C_mat_imgcolor,
					   int lin, int col, FILE *in)
{
	for (int i = 0; i < lin; i++) {
		for (int j = 0; j < col; j++) {
			fscanf(in, "%d", &(*mat_imgcolor)[i][j].rosu);
			fscanf(in, "%d", &(*mat_imgcolor)[i][j].verde);
			fscanf(in, "%d", &(*mat_imgcolor)[i][j].albastru);
		}
	}
	for (int i = 0; i < lin; i++) {
		for (int j = 0; j < col; j++) {
			(*C_mat_imgcolor)[i][j].rosu = (*mat_imgcolor)[i][j].rosu;
			(*C_mat_imgcolor)[i][j].verde = (*mat_imgcolor)[i][j].verde;
			(*C_mat_imgcolor)[i][j].albastru =
			(*mat_imgcolor)[i][j].albastru;
		}
	}
}

/// citireascii-albneg citeste matricea imaginii grayscale in format ascii
/// mat_imgalbneg este matricea ce corespunde imaginei grayscale
/// C_mat_imgalbneg este zona de selectie care initial este toata imaginea
/// lin este numarul de linii
/// col este numarul de coloane
/// prin in se citeste din fisier matricea
void citireascii_albneg(int ***mat_imgalbneg, int ***C_mat_imgalbneg,
						int lin, int col, FILE *in)
{
	for (int i = 0; i < lin; i++)
		for (int j = 0; j < col; j++)
			fscanf(in, "%d", &(*mat_imgalbneg)[i][j]);
	for (int i = 0; i < lin; i++)
		for (int j = 0; j < col; j++)
			(*C_mat_imgalbneg)[i][j] = (*mat_imgalbneg)[i][j];
}

/// incarcare_imagine citeste tipul imaginii, dimensiunea ei si apoi memoreaza
/// in matricea corespondenta tipului de imagine color sau grayscale
/// infom memoreaza toate datele necesare procesariii ulterioare a imaginii
/// si salvarea acesteia
/// comanda reprezinta comanda introdusa pe tota linia curenta
int incarcare_imagine(imgcolor ***mat_imgcolor, imgcolor ***C_mat_imgcolor,
					  int ***mat_imgalbneg, int ***C_mat_imgalbneg,
					  info_mat *infom, char comanda[N])
{
	char *fisier = strtok(comanda, " ");
	fisier = strtok(NULL, " ");
	if (!fisier || strtok(NULL, " ")) {
		printf("Invalid command\n");
	} else {
		if (infom->tip != 0)
			eliberare_resurse(mat_imgcolor, C_mat_imgcolor, mat_imgalbneg,
							  C_mat_imgalbneg, infom);
		FILE *in = fopen(fisier, "r");
		if (!in) {
			printf("Failed to load %s\n", fisier);
			return -1;
		}
		char magic_word[3]; int lin, col; char aux_comentarii;
		fscanf(in, "%s", magic_word);
		fread(&aux_comentarii, sizeof(unsigned char), 1, in);
		fread(&aux_comentarii, sizeof(unsigned char), 1, in);
		while (aux_comentarii == '#') {
			while (aux_comentarii != '\n')
				fread(&aux_comentarii, sizeof(unsigned char), 1, in);
			fread(&aux_comentarii, sizeof(unsigned char), 1, in);
		}
		fseek(in, -1, SEEK_CUR);
		fscanf(in, "%d", &col);
		fscanf(in, "%d", &lin);
		int tip = magic_word[1] - '0';
		if (tip == 2 || tip == 3 || tip == 5 || tip == 6) {
			int variatie;
			fscanf(in, "%d", &variatie);
		}
		fread(&aux_comentarii, sizeof(unsigned char), 1, in);
		fread(&aux_comentarii, sizeof(unsigned char), 1, in);
		while (aux_comentarii == '#') {
			while (aux_comentarii != '\n')
				fread(&aux_comentarii, sizeof(unsigned char), 1, in);
			fread(&aux_comentarii, sizeof(unsigned char), 1, in);
		}
		fseek(in, -1, SEEK_CUR);
		if (tip == 1 || tip == 2 || tip == 4 || tip == 5)
			alocare_matalbneg(mat_imgalbneg, C_mat_imgalbneg, lin, col);
		if (tip == 3 || tip == 6)
			alocare_matcolor(mat_imgcolor, C_mat_imgcolor, lin, col);
		if (tip == 1 || tip == 2)
			citireascii_albneg(mat_imgalbneg, C_mat_imgalbneg, lin, col, in);
		if (tip == 3)
			citireascii_color(mat_imgcolor, C_mat_imgcolor, lin, col, in);
		if (tip == 4 || tip == 5)
			citirebin_albneg(mat_imgalbneg, C_mat_imgalbneg, lin, col, in);
		if (tip == 6)
			citierebin_color(mat_imgcolor, C_mat_imgcolor, lin, col, in);
		infom->nrcoloane = col;
		infom->nrlinii = lin;
		infom->tip = tip;
		infom->C_nrcoloane = col;
		infom->C_nrlinii = lin;
		infom->C_tip = tip;
		infom->x1_selectie = 0;
		infom->y1_selectie = 0;
		infom->x2_selectie = col;
		infom->y2_selectie = lin;
		printf("Loaded %s\n", fisier);
		fclose(in);
		return 0;
	}
	return 0;
}

/// selectare_totala selecteaza zona de procesare ca fiind toata imaginea
/// astfel, C_mat_imgcolor sau C_mat_imgalbneg devin toata imaginea, iar
/// datele lor stocate in infom seschimba
int selectare_totala(imgcolor ***mat_imgcolor, imgcolor ***C_mat_imgcolor,
					 int ***mat_imgalbneg, int ***C_mat_imgalbneg,
					 info_mat *infom)
{
	if (infom->tip == 3 || infom->tip == 6) {
		for (int i = 0; i < infom->C_nrlinii; i++)
			free((*C_mat_imgcolor)[i]);
		free(*C_mat_imgcolor);
		*C_mat_imgcolor = NULL;
		*C_mat_imgcolor = (imgcolor **)malloc
		(infom->nrlinii * sizeof(imgcolor *));
		if (!(*C_mat_imgcolor)) {
			printf("Error allocating memory\n");
			return -1;
		}
		for (int i = 0; i < infom->nrlinii; i++) {
			(*C_mat_imgcolor)[i] = (imgcolor *)malloc
			(infom->nrcoloane * sizeof(imgcolor));
			if (!(*C_mat_imgcolor)[i]) {
				printf("Error allocating memory\n");
				return -1;
			}
		}
		for (int i = 0; i < infom->nrlinii; i++) {
			for (int j = 0; j < infom->nrcoloane; j++) {
				(*C_mat_imgcolor)[i][j].rosu = (*mat_imgcolor)[i][j].rosu;
				(*C_mat_imgcolor)[i][j].verde = (*mat_imgcolor)[i][j].verde;
				(*C_mat_imgcolor)[i][j].albastru = (*mat_imgcolor)[i][j].
				albastru;
			}
		}
	} else {
		for (int i = 0; i < infom->C_nrlinii; i++)
			free((*C_mat_imgalbneg)[i]);
		free(*C_mat_imgalbneg);
		*C_mat_imgalbneg = NULL;
		*C_mat_imgalbneg = (int **)malloc(infom->nrlinii * sizeof(int *));
		if (!(*C_mat_imgalbneg)) {
			printf("Error allocating memory\n");
			return -1;
		}
		for (int i = 0; i < infom->nrlinii; i++) {
			(*C_mat_imgalbneg)[i] = (int *)malloc
			(infom->nrcoloane * sizeof(int));
			if (!(*C_mat_imgalbneg)[i]) {
				printf("Error allocating memory\n");
					return -1;
			}
		}
		for (int i = 0; i < infom->nrlinii; i++)
			for (int j = 0; j < infom->nrcoloane; j++)
				(*C_mat_imgalbneg)[i][j] = (*mat_imgalbneg)[i][j];
	}
	infom->C_nrcoloane = infom->nrcoloane;
	infom->C_nrlinii = infom->nrlinii;
	infom->x1_selectie = 0;
	infom->y1_selectie = 0;
	infom->x2_selectie = infom->nrcoloane;
	infom->y2_selectie = infom->nrlinii;
	printf("Selected ALL\n");
	return 0;
}

/// selectare_zona_color selecteaza conform coordonatelor introduse, stocate in
/// prealabil in zona, zona de selectie pt imaginea color
/// zona are toate colturile zonei de selectie
int selectare_zona_color(imgcolor ***mat_imgcolor, imgcolor ***C_mat_imgcolor,
						 info_mat *infom, coord zona)
{
	for (int i = 0; i < infom->C_nrlinii; i++)
		free((*C_mat_imgcolor)[i]);
	free(*C_mat_imgcolor);
	*C_mat_imgcolor = NULL;
	*C_mat_imgcolor = (imgcolor **)malloc
	((zona.coord_y2 - zona.coord_y1) * sizeof(imgcolor *));
	if (!(*C_mat_imgcolor)) {
		printf("Error allocating memory\n");
		return -1;
	}
	for (int i = 0; i < (zona.coord_y2 - zona.coord_y1); i++) {
		(*C_mat_imgcolor)[i] = (imgcolor *)malloc
		((zona.coord_x2 - zona.coord_x1) * sizeof(imgcolor));
		if (!(*C_mat_imgcolor)[i]) {
			printf("Error allocating memory\n");
			return -1;
		}
	}
	int linie = 0, coloana = 0;
	for (int i = zona.coord_y1; i < zona.coord_y2; i++) {
		coloana = 0;
		for (int j = zona.coord_x1; j < zona.coord_x2; j++) {
			(*C_mat_imgcolor)[linie][coloana].rosu = (*mat_imgcolor)[i][j].
			rosu; (*C_mat_imgcolor)[linie][coloana].verde = (*mat_imgcolor)
			[i][j].verde; (*C_mat_imgcolor)[linie][coloana].albastru =
			(*mat_imgcolor)[i][j].albastru;
			coloana++;
		}
		linie++;
	}
	return 0;
}

/// selectare_zona_albneg selecteaza conform coordonatelor introduse, stocate in
/// prealabil in zona, zona de selectie pt imaginea grayscale
/// zona are toate colturile zonei de selectie
int selectare_zona_albneg(int ***mat_imgalbneg, int ***C_mat_imgalbneg,
						  info_mat *infom, coord zona)
{
	for (int i = 0; i < infom->C_nrlinii; i++)
		free((*C_mat_imgalbneg)[i]);
	free(*C_mat_imgalbneg);
	*C_mat_imgalbneg = NULL;
	*C_mat_imgalbneg = (int **)malloc
	((zona.coord_y2 - zona.coord_y1) * sizeof(int *));
	if (!(*C_mat_imgalbneg)) {
		printf("Error allocating memory\n");
		return -1;
	}
	for (int i = 0; i < (zona.coord_y2 - zona.coord_y1); i++) {
		(*C_mat_imgalbneg)[i] = (int *)malloc
		((zona.coord_x2 - zona.coord_x1) * sizeof(int));
		if ((*C_mat_imgalbneg)[i] == NULL) {
			printf("Error allocating memory\n");
			return -1;
		}
	}
	int linie = 0, coloana = 0;
	for (int i = zona.coord_y1; i < zona.coord_y2; i++) {
		coloana = 0;
		for (int j = zona.coord_x1; j < zona.coord_x2; j++) {
			(*C_mat_imgalbneg)[linie][coloana] = (*mat_imgalbneg)[i][j];
			coloana++;
		}
		linie++;
	}
	return 0;
}

/// selectare_zona selecteaza zona de lucru in functie de ce este introdus dupa
/// textul select, respectiv ALL pt toata imaginea sau o serie de 4 valori ce
/// reprezinta cele patru colturi ale zonei de selectie
/// comanda este comanda introdusa de utilizator pe tot randul
int selectare_zona(imgcolor ***mat_imgcolor, imgcolor ***C_mat_imgcolor,
				   int ***mat_imgalbneg, int ***C_mat_imgalbneg,
				   info_mat *infom, char comanda[N])
{
	if (infom->tip == 0) {
		printf("No image loaded\n");
		return 0;
	}
	char C_comanda[N]; strcpy(C_comanda, comanda); char *verif =
	strtok(C_comanda, " "); verif = strtok(NULL, " "); verif =
	strtok(NULL, " "); verif = strtok(NULL, " "); verif =
	strtok(NULL, " "); verif = strtok(NULL, " "); char *coordonata =
	strtok(comanda, " "); coordonata = strtok(NULL, " ");
	if (!coordonata) {
		printf("Invalid command\n"); return 0;
	}
	if (strcmp(coordonata, "ALL") == 0) {
		selectare_totala(mat_imgcolor, C_mat_imgcolor, mat_imgalbneg,
						 C_mat_imgalbneg, infom); return 0;
	} else if (!verif) {
		int x1 = atoi(coordonata);
		if (x1 == 0 && coordonata[0] != '0') {
			printf("Invalid command\n"); return 0;
		}
		coordonata = strtok(NULL, " ");
		if (!coordonata) {
			printf("Invalid command\n"); return 0;
		}
		int y1 = atoi(coordonata);
		if (y1 == 0 && coordonata[0] != '0') {
			printf("Invalid command\n"); return 0;
		}
		coordonata = strtok(NULL, " ");
		if (!coordonata) {
			printf("Invalid command\n"); return 0;
		}
		int x2 = atoi(coordonata);
		if (x2 == 0 && coordonata[0] != '0') {
			printf("Invalid command\n"); return 0;
		}
		coordonata = strtok(NULL, " ");
		if (!coordonata) {
			printf("Invalid command\n"); return 0;
		}
		int y2 = atoi(coordonata);
		if (y2 == 0 && coordonata[0] != '0') {
			printf("Invalid command\n"); return 0;
		}
		if (strtok(NULL, " ")) {
			printf("Invalid command\n"); return 0;
		}
		if (x1 > x2) {
			int aux = x1; x1 = x2; x2 = aux;
		}
		if (y1 > y2) {
			int aux = y1; y1 = y2; y2 = aux;
		}
		if ((x1 < 0 || x1 > infom->nrcoloane) || (x2 < 0 || x2 >
		infom->nrcoloane) || (y1 < 0 || y1 > infom->nrlinii) ||
		(y2 < 0 || y2 > infom->nrlinii || (x1 == x2 || y1 == y2))) {
			printf("Invalid set of coordinates\n"); return 0;
		}
		coord zona; zona.coord_x1 = x1; zona.coord_y1 = y1;
		zona.coord_x2 = x2; zona.coord_y2 = y2;
		if (infom->tip == 3 || infom->tip == 6)
			selectare_zona_color(mat_imgcolor, C_mat_imgcolor, infom, zona);
		else
			selectare_zona_albneg(mat_imgalbneg, C_mat_imgalbneg, infom, zona);
		infom->C_nrcoloane = x2 - x1; infom->C_nrlinii = y2 - y1;
		infom->x1_selectie = x1; infom->y1_selectie = y1;
		infom->x2_selectie = x2; infom->y2_selectie = y2;
		printf("Selected %d %d %d %d\n", x1, y1, x2, y2); return 0;
	}
	printf("Invalid command\n"); return 0;
}

/// egalizare_imagine realizeaza egalizea imaginii grayscale introdusa
/// folosind toata imaginea tinand cont de formula pt fiecare pixel
/// din imagine facnd suma aparitie tuturor culorior mai aporpiate de
/// de negru decat cea curenta si impartind la suprafata totala
/// comanda reprezinta toata comanda introdusa
void egalizare_imagine(int ***mat_imgalbneg, int ***C_mat_imgalbneg,
					   info_mat *infom, char comanda[N])
{
	char *verificare = strtok(comanda, " ");
	verificare = strtok(NULL, " ");
	if (infom->tip == 0) {
		printf("No image loaded\n");
	} else if (infom->tip == 3 || infom->tip == 6) {
		printf("Black and white image needed\n");
	} else if (infom->tip == 1 || infom->tip == 4) {
		printf("Invalid command\n");
	} else if (!verificare) {
		int *valori = (int *)malloc(256 * sizeof(int));
		for (int i = 0; i < 256; i++)
			valori[i] = 0;
		for (int i = 0; i < infom->nrlinii; i++)
			for (int j = 0; j < infom->nrcoloane; j++)
				valori[(*mat_imgalbneg)[i][j]]++;
		for (int i = 0; i < infom->nrlinii; i++) {
			for (int j = 0; j < infom->nrcoloane; j++) {
				int aux = (*mat_imgalbneg)[i][j];
				double s = 0;
				while (aux >= 0) {
					s = s + valori[aux];
					aux--;
				}
				s = s * 255 / (infom->nrlinii * infom->nrcoloane);
				s = round(s);
				if (s > 255)
					s = 255;
				if (s < 0)
					s = 0;
				(*mat_imgalbneg)[i][j] = s;
			}
		}
		free(valori);
		int linie = 0, coloana = 0;
		for (int i = infom->y1_selectie; i < infom->y2_selectie; i++) {
			coloana = 0;
			for (int j = infom->x1_selectie; j < infom->x2_selectie; j++) {
				(*C_mat_imgalbneg)[linie][coloana] = (*mat_imgalbneg)[i][j];
				coloana++;
			}
		linie++;
		}
		printf("Equalize done\n");
	} else {
		printf("Invalid command\n");
	}
}

/// functia redimensionare_imagine ia zona de selectie a imagini si o copiaza in
/// imaginea originala, schimband totodata dimensiounea imaginii originale cu
/// zona selectata
/// comanda este toata comanda introdusa pe toata linie
int redimensionare_imagine(imgcolor ***mat_imgcolor, imgcolor ***C_mat_imgcolor,
						   int ***mat_imgalbneg, int ***C_mat_imgalbneg,
						   info_mat *infom, char comanda[N])
{
	if (infom->tip == 0) {
		printf("No image loaded\n");
		return 0;
	}
	char *verificare = strtok(comanda, " ");
	verificare = strtok(NULL, " ");
	if (verificare) {
		printf("Invalid command\n");
		return 0;
	}
	if (infom->tip == 3 || infom->tip == 6) {
		for (int i = 0; i < infom->nrlinii; i++)
			free((*mat_imgcolor)[i]);
		free(*mat_imgcolor);
		*mat_imgcolor = NULL;
		*mat_imgcolor = (imgcolor **)malloc
		(infom->C_nrlinii * sizeof(imgcolor *));
		if (!(*mat_imgcolor)) {
			printf("Error allocating memory\n");
			return -1;
		}
		for (int i = 0; i < infom->C_nrlinii; i++) {
			(*mat_imgcolor)[i] = (imgcolor *)malloc
			(infom->C_nrcoloane * sizeof(imgcolor));
			if (!(*mat_imgcolor)[i]) {
				printf("Error allocating memory\n");
				return -1;
			}
		}
		for (int i = 0; i < infom->C_nrlinii; i++) {
			for (int j = 0; j < infom->C_nrcoloane; j++) {
				(*mat_imgcolor)[i][j].rosu = (*C_mat_imgcolor)[i][j].rosu;
				(*mat_imgcolor)[i][j].verde = (*C_mat_imgcolor)[i][j].verde;
				(*mat_imgcolor)[i][j].albastru = (*C_mat_imgcolor)[i][j].
				albastru;
			}
		}
	} else {
		for (int i = 0; i < infom->nrlinii; i++)
			free((*mat_imgalbneg)[i]);
		free(*mat_imgalbneg);
		*mat_imgalbneg = NULL;
		*mat_imgalbneg = (int **)malloc(infom->C_nrlinii * sizeof(int *));
		if (!(*mat_imgalbneg)) {
			printf("Error allocating memory\n");
			return -1;
		}
		for (int i = 0; i < infom->C_nrlinii; i++) {
			(*mat_imgalbneg)[i] = (int *)malloc
			(infom->C_nrcoloane * sizeof(int));
			if (!(*mat_imgalbneg)[i]) {
				printf("Error allocating memory\n");
				return -1;
			}
		}
		for (int i = 0; i < infom->C_nrlinii; i++)
			for (int j = 0; j < infom->C_nrcoloane; j++)
				(*mat_imgalbneg)[i][j] = (*C_mat_imgalbneg)[i][j];
	}
	infom->nrcoloane = infom->C_nrcoloane;
	infom->nrlinii = infom->C_nrlinii;
	infom->x1_selectie = 0;
	infom->y1_selectie = 0;
	infom->x2_selectie = infom->nrcoloane;
	infom->y2_selectie = infom->nrlinii;
	printf("Image cropped\n");
	return 0;
}

/// aplicare_edge va aplica nucleul corespunzator edge(matrice) pe toata zona de
/// selectie daca asta este posibil si muchiile zonei de selectie nu se afla
/// pe laturile imaginei originale
int aplicare_edge(imgcolor ***mat_imgcolor, imgcolor ***C_mat_imgcolor,
				  info_mat *infom)
{
	for (int i = 0; i < infom->C_nrlinii; i++) {
		for (int j = 0; j < infom->C_nrcoloane; j++) {
			if ((infom->x1_selectie + j + 1) < infom->nrcoloane &&
				(infom->y1_selectie + i + 1) < infom->nrlinii &&
				(infom->x1_selectie + j) > 0 && (infom->y1_selectie + i) > 0) {
				double s = 0;
				for (int h = i - 1; h <= i + 1; h++)
					for (int k = j - 1; k <= j + 1; k++)
						s = s - (*mat_imgcolor)[infom->y1_selectie + h]
						[infom->x1_selectie + k].rosu;
				s = s + 9 * (*mat_imgcolor)[infom->y1_selectie + i]
				[infom->x1_selectie + j].rosu;
				s = round(s);
				if (s > 255)
					s = 255;
				if (s < 0)
					s = 0;
				(*C_mat_imgcolor)[i][j].rosu = s;
				s = 0;
				for (int h = i - 1; h <= i + 1; h++)
					for (int k = j - 1; k <= j + 1; k++)
						s = s - (*mat_imgcolor)[infom->y1_selectie + h]
						[infom->x1_selectie + k].verde;
				s = s + 9 * (*mat_imgcolor)[infom->y1_selectie + i]
				[infom->x1_selectie + j].verde;
				s = round(s);
				if (s > 255)
					s = 255;
				if (s < 0)
					s = 0;
				(*C_mat_imgcolor)[i][j].verde = s;
				s = 0;
				for (int h = i - 1; h <= i + 1; h++)
					for (int k = j - 1; k <= j + 1; k++)
						s = s - (*mat_imgcolor)[infom->y1_selectie + h]
						[infom->x1_selectie + k].albastru;
				s = s + 9 * (*mat_imgcolor)[infom->y1_selectie + i]
				[infom->x1_selectie + j].albastru;
				s = round(s);
				if (s > 255)
					s = 255;
				if (s < 0)
					s = 0;
				(*C_mat_imgcolor)[i][j].albastru = s;
			}
		}
	}
	return 0;
}

/// aplicare_sharpen va aplica nucleul corespunzator sharpen(matrice) pe toata
/// zona de selectie daca asta este posibil si muchiile zonei de selectie nu
/// se afla pe laturile imaginii originale
int aplicare_sharpen(imgcolor ***mat_imgcolor, imgcolor ***C_mat_imgcolor,
					 info_mat *infom)
{
	for (int i = 0; i < infom->C_nrlinii; i++) {
		for (int j = 0; j < infom->C_nrcoloane; j++) {
			if ((infom->x1_selectie + j + 1) < infom->nrcoloane &&
				(infom->y1_selectie + i + 1) < infom->nrlinii &&
				(infom->x1_selectie + j) > 0 && (infom->y1_selectie + i) > 0) {
				double s = 0;
				s = s - (*mat_imgcolor)[infom->y1_selectie + i - 1]
				[infom->x1_selectie + j].rosu - (*mat_imgcolor)
				[infom->y1_selectie + i + 1][infom->x1_selectie + j].rosu -
				(*mat_imgcolor)[infom->y1_selectie + i]
				[infom->x1_selectie + j - 1].rosu - (*mat_imgcolor)
				[infom->y1_selectie + i][infom->x1_selectie + j + 1].rosu;
				s = s + 5 * (*mat_imgcolor)[infom->y1_selectie + i]
				[infom->x1_selectie + j].rosu; s = round(s);
				if (s > 255)
					s = 255;
				if (s < 0)
					s = 0;
				(*C_mat_imgcolor)[i][j].rosu = s; s = 0;
				s = s - (*mat_imgcolor)[infom->y1_selectie + i - 1]
				[infom->x1_selectie + j].verde - (*mat_imgcolor)
				[infom->y1_selectie + i + 1][infom->x1_selectie + j].verde -
				(*mat_imgcolor)[infom->y1_selectie + i]
				[infom->x1_selectie + j - 1].verde - (*mat_imgcolor)
				[infom->y1_selectie + i][infom->x1_selectie + j + 1].verde;
				s = s + 5 * (*mat_imgcolor)[infom->y1_selectie + i]
				[infom->x1_selectie + j].verde; s = round(s);
				if (s > 255)
					s = 255;
				if (s < 0)
					s = 0;
				(*C_mat_imgcolor)[i][j].verde = s; s = 0;
				s = s - (*mat_imgcolor)[infom->y1_selectie + i - 1]
				[infom->x1_selectie + j].albastru - (*mat_imgcolor)
				[infom->y1_selectie + i + 1][infom->x1_selectie + j].albastru -
				(*mat_imgcolor)[infom->y1_selectie + i]
				[infom->x1_selectie + j - 1].albastru - (*mat_imgcolor)
				[infom->y1_selectie + i][infom->x1_selectie + j + 1].albastru;
				s = s + 5 * (*mat_imgcolor)[infom->y1_selectie + i]
				[infom->x1_selectie + j].albastru; s = round(s);
				if (s > 255)
					s = 255;
				if (s < 0)
					s = 0;
				(*C_mat_imgcolor)[i][j].albastru = s;
			}
		}
	}
	return 0;
}

/// aplicare_blur va aplica nucleul corespunzator blur(matrice) pe toata zona de
/// selectie daca asta este posibil si muchiile zonei de selectie nu se afla
/// pe laturile imaginei originale
int aplicare_blur(imgcolor ***mat_imgcolor, imgcolor ***C_mat_imgcolor,
				  info_mat *infom)
{
	for (int i = 0; i < infom->C_nrlinii; i++) {
		for (int j = 0; j < infom->C_nrcoloane; j++) {
			if ((infom->x1_selectie + j + 1) < infom->nrcoloane &&
				(infom->y1_selectie + i + 1) < infom->nrlinii &&
				(infom->x1_selectie + j) > 0 && (infom->y1_selectie + i) > 0) {
				double s = 0;
				for (int h = i - 1; h <= i + 1; h++)
					for (int k = j - 1; k <= j + 1; k++)
						s = s + (*mat_imgcolor)[infom->y1_selectie + h]
						[infom->x1_selectie + k].rosu;
				s = s / 9; s = round(s);
				if (s > 255)
					s = 255;
				if (s < 0)
					s = 0;
				(*C_mat_imgcolor)[i][j].rosu = s; s = 0;
				for (int h = i - 1; h <= i + 1; h++)
					for (int k = j - 1; k <= j + 1; k++)
						s = s + (*mat_imgcolor)[infom->y1_selectie + h]
						[infom->x1_selectie + k].verde;
				s = s / 9; s = round(s);
				if (s > 255)
					s = 255;
				if (s < 0)
					s = 0;
				(*C_mat_imgcolor)[i][j].verde = s; s = 0;
				for (int h = i - 1; h <= i + 1; h++)
					for (int k = j - 1; k <= j + 1; k++)
						s = s + (*mat_imgcolor)[infom->y1_selectie + h]
						[infom->x1_selectie + k].albastru;
				s = s / 9; s = round(s);
				if (s > 255)
					s = 255;
				if (s < 0)
					s = 0;
				(*C_mat_imgcolor)[i][j].albastru = s;
			}
		}
	}
	return 0;
}

/// aplicare_gaussian_blur va aplica nucleul corespunzator gaussian_blur
/// (matrice) pe toata zona de selectie daca asta este posibil si muchiile
/// zonei de selectie nu se afla pe laturile imaginei originale
int aplicare_gaussian_blur(imgcolor ***mat_imgcolor, imgcolor ***C_mat_imgcolor,
						   info_mat *infom)
{
	for (int i = 0; i < infom->C_nrlinii; i++) {
		for (int j = 0; j < infom->C_nrcoloane; j++) {
			if ((infom->x1_selectie + j + 1) < infom->nrcoloane &&
				(infom->y1_selectie + i + 1) < infom->nrlinii &&
				(infom->x1_selectie + j) > 0 && (infom->y1_selectie + i) > 0) {
				double s = 0;
				s = s + 4 * (*mat_imgcolor)[infom->y1_selectie + i]
				[infom->x1_selectie + j].rosu + (*mat_imgcolor)
				[infom->y1_selectie + i - 1][infom->x1_selectie + j - 1].rosu +
				(*mat_imgcolor)[infom->y1_selectie + i - 1]
				[infom->x1_selectie + j + 1].rosu + (*mat_imgcolor)
				[infom->y1_selectie + i + 1][infom->x1_selectie + j + 1].rosu +
				(*mat_imgcolor)[infom->y1_selectie + i + 1][infom->x1_selectie +
				j - 1].rosu + 2 * (*mat_imgcolor)[infom->y1_selectie + i - 1]
				[infom->x1_selectie + j].rosu + 2 * (*mat_imgcolor)
				[infom->y1_selectie + i + 1][infom->x1_selectie + j].rosu + 2 *
				(*mat_imgcolor)[infom->y1_selectie + i][infom->x1_selectie + j +
				1].rosu + 2 * (*mat_imgcolor)[infom->y1_selectie + i]
				[infom->x1_selectie + j - 1].rosu; s = s / 16; s = round(s);
				if (s > 255)
					s = 255;
				if (s < 0)
					s = 0;
				(*C_mat_imgcolor)[i][j].rosu = s; s = 0;
				s = s + 4 * (*mat_imgcolor)[infom->y1_selectie + i]
				[infom->x1_selectie + j].verde + (*mat_imgcolor)
				[infom->y1_selectie + i - 1][infom->x1_selectie + j - 1].verde +
				(*mat_imgcolor)[infom->y1_selectie + i - 1]
				[infom->x1_selectie + j + 1].verde + (*mat_imgcolor)
				[infom->y1_selectie + i + 1][infom->x1_selectie + j + 1].verde +
				(*mat_imgcolor)[infom->y1_selectie + i + 1][infom->x1_selectie +
				j - 1].verde + 2 * (*mat_imgcolor)[infom->y1_selectie + i - 1]
				[infom->x1_selectie + j].verde + 2 * (*mat_imgcolor)
				[infom->y1_selectie + i + 1][infom->x1_selectie + j].verde + 2 *
				(*mat_imgcolor)[infom->y1_selectie + i][infom->x1_selectie + j +
				1].verde + 2 * (*mat_imgcolor)[infom->y1_selectie + i]
				[infom->x1_selectie + j - 1].verde; s = s / 16; s = round(s);
				if (s > 255)
					s = 255;
				if (s < 0)
					s = 0;
				(*C_mat_imgcolor)[i][j].verde = s; s = 0;
				(*C_mat_imgcolor)[i][j].albastru = s; s = 0;
				s = s + 4 * (*mat_imgcolor)[infom->y1_selectie + i]
				[infom->x1_selectie + j].albastru + (*mat_imgcolor)
				[infom->y1_selectie + i - 1][infom->x1_selectie + j - 1].
				albastru + (*mat_imgcolor)[infom->y1_selectie + i - 1]
				[infom->x1_selectie + j + 1].albastru + (*mat_imgcolor)
				[infom->y1_selectie + i + 1][infom->x1_selectie + j + 1].
				albastru + (*mat_imgcolor)[infom->y1_selectie + i + 1]
				[infom->x1_selectie + j - 1].albastru + 2 * (*mat_imgcolor)
				[infom->y1_selectie + i - 1][infom->x1_selectie + j].
				albastru + 2 * (*mat_imgcolor)[infom->y1_selectie + i + 1]
				[infom->x1_selectie + j].albastru + 2 * (*mat_imgcolor)
				[infom->y1_selectie + i][infom->x1_selectie + j + 1].albastru +
				2 * (*mat_imgcolor)[infom->y1_selectie + i]
				[infom->x1_selectie + j - 1].albastru; s = s / 16; s = round(s);
				if (s > 255)
					s = 255;
				if (s < 0)
					s = 0;
				(*C_mat_imgcolor)[i][j].albastru = s;
			}
		}
	}
	return 0;
}

/// aplicare_filtru va aplica filtrul ales peste selectie si apoi va copia
/// elementele selectie pe pozitiile corespunzatoare
/// comanda este toata comanda introdusa pe toata linie
int aplicare_filtru(imgcolor ***mat_imgcolor, imgcolor ***C_mat_imgcolor,
					info_mat *infom, char comanda[N])
{
	if (infom->tip == 0) {
		printf("No image loaded\n");
		return 0;
	}
	char *tip_filtru = strtok(comanda, " ");
	tip_filtru = strtok(NULL, " ");
	if (strtok(NULL, " ")) {
		printf("Invalid command\n");
		return 0;
	}
	if (!tip_filtru) {
		printf("Invalid command\n");
		return 0;
	}
	if (strcmp(tip_filtru, "EDGE") != 0 && strcmp(tip_filtru, "SHARPEN") != 0 &&
	    strcmp(tip_filtru, "BLUR") != 0 &&
		strcmp(tip_filtru, "GAUSSIAN_BLUR") != 0) {
		printf("APPLY parameter invalid\n");
		return 0;
	}
	if (infom->tip != 3 && infom->tip != 6) {
		printf("Easy, Charlie Chaplin\n");
		return 0;
	}
	if (strcmp(tip_filtru, "EDGE") == 0)
		aplicare_edge(mat_imgcolor, C_mat_imgcolor, infom);
	else if (strcmp(tip_filtru, "SHARPEN") == 0)
		aplicare_sharpen(mat_imgcolor, C_mat_imgcolor, infom);
	else if (strcmp(tip_filtru, "BLUR") == 0)
		aplicare_blur(mat_imgcolor, C_mat_imgcolor, infom);
	else if (strcmp(tip_filtru, "GAUSSIAN_BLUR") == 0)
		aplicare_gaussian_blur(mat_imgcolor, C_mat_imgcolor, infom);
	int linie = 0, coloana = 0;
	for (int i = infom->y1_selectie; i < infom->y2_selectie; i++) {
		coloana = 0;
		for (int j = infom->x1_selectie; j < infom->x2_selectie; j++) {
			(*mat_imgcolor)[i][j] = (*C_mat_imgcolor)[linie][coloana];
			coloana++;
		}
		linie++;
	}
	printf("APPLY %s done\n", tip_filtru);
	return 0;
}

/// calculare_histograma va calcula histograma imaginei alb negru
/// numarand numarul de aparitii al fiecarei valori in matrice si
/// impartind intervalul 0 256 in cate binuri sunt necesare
/// cu un maxim de stelute calculat pt a determina numarul de stelute
/// al fiecarui interval
/// comanda este toata comanda introdusa pe toata linie
int calculare_histograma(int ***mat_imgalbneg, info_mat *infom, char comanda[N])
{
	if (infom->tip == 0) {
		printf("No image loaded\n");
		return 0;
	}
	char *coordonate = strtok(comanda, " ");
	coordonate = strtok(NULL, " ");
	if (!coordonate) {
		printf("Invalid command\n");
		return 0;
	}
	int x = atoi(coordonate);
	if (x == 0 && coordonate[0] != '0') {
		printf("Invalid command\n");
		return 0;
	}
	coordonate = strtok(NULL, " ");
	if (!coordonate) {
		printf("Invalid command\n");
		return 0;
	}
	int y = atoi(coordonate);
	if (y == 0 && coordonate[0] != '0') {
		printf("Invalid command\n");
		return 0;
	}
	if (strtok(NULL, " ")) {
		printf("Invalid command\n");
		return 0;
	}
	int cy = y;
	while (cy % 2 == 0)
		cy /= 2;
	if (cy != 1) {
		printf("Invalid set of parameters\n");
		return 0;
	}
	if (infom->tip == 3 || infom->tip == 6) {
		printf("Black and white image needed\n");
		return 0;
	}
	int *valori = (int *)malloc(256 * sizeof(int));
	for (int i = 0; i < 256; i++)
		valori[i] = 0;
	for (int i = 0; i < infom->nrlinii; i++)
		for (int j = 0; j < infom->nrcoloane; j++)
			valori[(*mat_imgalbneg)[i][j]]++;
	int *frec_val = (int *)malloc((y + 1) * sizeof(int));
	for (int i = 0; i < y; i++)
		frec_val[i] = 0;
	int index = 0;
	for (int i = 0; i < y; i++) {
		for (int j = 0; j < (256 / y); j++) {
			frec_val[i] += valori[index];
			index++;
		}
	}
	int maxim = -1;
	for (int i = 0; i < y; i++)
		if (maxim < frec_val[i])
			maxim = frec_val[i];
	for (int i = 0; i < y; i++)
		frec_val[i] = frec_val[i] * x / maxim;
	for (int i = 0; i < y; i++) {
		printf("%d\t|\t", frec_val[i]);
		while (frec_val[i] > 0) {
			printf("*");
			frec_val[i]--;
		}
		printf("\n");
	}
	free(valori);
	free(frec_val);
	return 0;
}

/// salvare_imagine va salva imaginea in formatul dorit ascii sau binar
/// indiferent de tipul inial al imaginii
/// in functie de tipul imaginii(color sau grayscale) se va salva matricea
/// mat_imgcolor sau mat_imgalbneg tinand cont de dimensiunile acestora si
/// tipul lor din infom si de tipul lor
/// comanda este toata comanda introdusa pe toata linie
int salvare_imagine(imgcolor ***mat_imgcolor, int ***mat_imgalbneg,
					info_mat *infom, char comanda[N])
{
	if (infom->tip == 0) {
		printf("No image loaded\n");
		return 0;
	}
	char *fisier = strtok(comanda, " "); fisier = strtok(NULL, " ");
	if (!fisier) {
		printf("Invalid command\n");
		return 0;
	}
	char *tipul_salvarii = strtok(NULL, " ");
	if (!tipul_salvarii) {
		FILE *out = fopen(fisier, "wb");
		if (!out) {
			printf("Error opening file");
			return 1;
		}
		if (infom->tip <= 3)
			fprintf(out, "P%d\n", infom->tip + 3);
		else
			fprintf(out, "P%d\n", infom->tip);
		fprintf(out, "%d %d\n", infom->nrcoloane, infom->nrlinii);
		fprintf(out, "255\n");
		if (infom->tip == 3 || infom->tip == 6) {
			for (int i = 0; i < infom->nrlinii; i++) {
				for (int j = 0; j < infom->nrcoloane; j++) {
					fwrite(&(*mat_imgcolor)[i][j].rosu,
						   sizeof(unsigned char), 1, out);
					fwrite(&(*mat_imgcolor)[i][j].verde,
						   sizeof(unsigned char), 1, out);
					fwrite(&(*mat_imgcolor)[i][j].albastru,
						   sizeof(unsigned char), 1, out);
				}
			}
		} else {
			for (int i = 0; i < infom->nrlinii; i++) {
				for (int j = 0; j < infom->nrcoloane; j++) {
					fwrite(&(*mat_imgalbneg)[i][j], sizeof(unsigned char),
						   1, out);
				}
			}
		}
		fclose(out); printf("Saved %s\n", fisier);
	} else if (strcmp(tipul_salvarii, "ascii") == 0) {
		FILE *out = fopen(fisier, "w");
		if (!out) {
			printf("Error opening file");
			return 1;
		}
		if (infom->tip > 3)
			fprintf(out, "P%d\n", infom->tip - 3);
		else
			fprintf(out, "P%d\n", infom->tip);
		fprintf(out, "%d %d\n", infom->nrcoloane, infom->nrlinii);
		fprintf(out, "255\n");
		if (infom->tip == 3 || infom->tip == 6) {
			for (int i = 0; i < infom->nrlinii; i++) {
				for (int j = 0; j < infom->nrcoloane; j++) {
					fprintf(out, "%d ", (*mat_imgcolor)[i][j].rosu);
					fprintf(out, "%d ", (*mat_imgcolor)[i][j].verde);
					fprintf(out, "%d ", (*mat_imgcolor)[i][j].albastru);
				}
				fprintf(out, "\n");
			}
		} else {
			for (int i = 0; i < infom->nrlinii; i++) {
				for (int j = 0; j < infom->nrcoloane; j++)
					fprintf(out, "%d ", (*mat_imgalbneg)[i][j]);
				fprintf(out, "\n");
			}
		}
		fclose(out);
		printf("Saved %s\n", fisier);
	} else {
		printf("Invalid command\n");
	}
	return 0;
}

/// initializez matricile imaginilor cu NULL si toate datele despre matrice 0
/// astfel incat ca atunci cand vreau sa introduc o matrice noua verific daca
/// datele curente sunt 0(respectiv tipul) si aloc urmatoarea matrice
/// in functie de comanda introdusa se va face operatia dorita daca formatul
/// comenzii a fost respectat cu strictete pe o singura linie
int main(void)
{
	imgcolor **mat_imgcolor = NULL; imgcolor **C_mat_imgcolor = NULL;
	int **mat_imgalbneg = NULL; int **C_mat_imgalbneg = NULL;
	char comanda[N]; info_mat infom; infom.C_nrcoloane = 0; infom.C_nrlinii = 0;
	infom.C_tip = 0; infom.nrcoloane = 0; infom.nrlinii = 0; infom.tip = 0;
	infom.x1_selectie = 0; infom.y1_selectie = 0; infom.x2_selectie = 0;
	infom.y2_selectie = 0;
	while (1) {
		fgets(comanda, N, stdin);
		int len = strlen(comanda);
		if (len > 0 && comanda[len - 1] == '\n')
			comanda[len - 1] = '\0';
		char cop_comanda[N]; strcpy(cop_comanda, comanda);
		char *tip_comanda = strtok(cop_comanda, " ");
		if (tip_comanda) {
			if (strcmp(tip_comanda, "LOAD") == 0) {
				incarcare_imagine(&mat_imgcolor, &C_mat_imgcolor,
								  &mat_imgalbneg, &C_mat_imgalbneg, &infom,
								  comanda);
			} else if (strcmp(tip_comanda, "SELECT") == 0) {
				selectare_zona(&mat_imgcolor, &C_mat_imgcolor, &mat_imgalbneg,
							   &C_mat_imgalbneg, &infom, comanda);
			} else if (strcmp(tip_comanda, "EQUALIZE") == 0) {
				egalizare_imagine(&mat_imgalbneg, &C_mat_imgalbneg, &infom,
								  comanda);
			} else if (strcmp(tip_comanda, "CROP") == 0) {
				redimensionare_imagine(&mat_imgcolor, &C_mat_imgcolor,
									   &mat_imgalbneg, &C_mat_imgalbneg,
									   &infom, comanda);
			} else if (strcmp(tip_comanda, "APPLY") == 0) {
				aplicare_filtru(&mat_imgcolor, &C_mat_imgcolor, &infom,
								comanda);
			} else if (strcmp(tip_comanda, "HISTOGRAM") == 0) {
				calculare_histograma(&mat_imgalbneg, &infom, comanda);
			} else if (strcmp(tip_comanda, "SAVE") == 0) {
				salvare_imagine(&mat_imgcolor, &mat_imgalbneg, &infom, comanda);
			} else if (strcmp(comanda, "EXIT") == 0) {
				if (mat_imgcolor || mat_imgalbneg) {
					eliberare_resurse(&mat_imgcolor, &C_mat_imgcolor,
									  &mat_imgalbneg, &C_mat_imgalbneg,
									  &infom);
				} else {
					printf("No image loaded\n");
				}
				break;
			} else if (1) {
				printf("Invalid command\n");
			}
		} else {
			printf("Invalid command\n");
		}
	}
	return 0;
}
