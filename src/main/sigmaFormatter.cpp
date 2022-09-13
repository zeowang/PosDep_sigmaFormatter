#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Aff_ChemCG_Related.h"

#define INFILE "feed.txt"
#define OUTFILE "out.txt"

int main(void) {
	FILE* inFile = NULL;

	inFile = fopen(INFILE, "r");
	if (inFile == NULL) {
		printf("Error, Check Input File.\n");
		exit(0);
	}
	else {
		double* indata = (double*)malloc(sizeof(double) * 3 * DIM);
		if (indata == NULL) abort();
		int ind = 1;

		for (int i = 0; i < 3 * DIM && ind == 1; i++) {
			ind = fscanf(inFile, "%lf", (indata + i));
		}
		
		static int Nx = 2;
		static int Ny = 2;
		static int Nz = 2;

		static int N2 = (int)(Nx * Ny);
		int N3 = Nz * N2;
		
		int* CGnum = (int*)malloc(sizeof(int) * N3);
		int* CGinfo = (int*)malloc(3 * sizeof(int) * N3);

		if (!CGnum || !CGinfo) {
			abort();
		}

		int i = 0;
		while (i < N3) {

			*(CGnum + i) = i + 1;
			*(CGinfo + 3 * i) = (i + 1) % Nx;
			*(CGinfo + 3 * i + 1) = (i / Nx + 1) % Ny;
			*(CGinfo + 3 * i + 2) = i / N2 + 1;
			if (*(CGinfo + 3 * i + 1) == 0) {
				*(CGinfo + 3 * i + 1) = Ny;
			}
			if (*(CGinfo + 3 * i) == 0) {
				*(CGinfo + 3 * i) = Nx;
			}

			i++;
		}
		i = 0;
		
		int CGnumber[4] = { 0 };
		condsearch(CGnum, CGinfo, 4, CGnumber);

		//for (int i = 0; i < 4; i++) {
		//	printf("%d\t", CGnumber[i]);
		//}
		//printf("\n");

		double* otsigma = (double*)malloc(DIM * DIM * sizeof(double));
		if (!otsigma)abort();

		for (int i = 0; i < DIM * DIM; i++) {
			*(otsigma + i) = 0;
		}

		int numRef[4] = { 0 };
		condsearch(CGnum, CGinfo, 1, numRef);


		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < DIM; j++) {
				condsearch(CGnum, CGinfo, j, CGnumber);
				datalloc(indata, otsigma, j, CGnumber, numRef, 4);
			}
			for (int j = 0; j < 4; j++) {
				CGnumber[j] = 0;
			}
		}

		double val = 0;

		for (int i = 0; i < DIM; i++) {
			for (int j = 0; j < DIM; j++) {
				val = *(otsigma + i * DIM + j);
				if (val == 0) {
					printf("%7.1lf ", val);
				}
				else {
					printf("%7.4lf ", val);
				}
				if ((j + 1) % 3 == 0)printf(" |");
			}
			printf("\n");
			if ((i + 1) % 3 == 0)printf("\n\n");
		}


	}

	system("PAUSE");

	return 0;
}