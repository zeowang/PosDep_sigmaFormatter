#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

extern int ELENUM = 8;
extern int DIM = ELENUM * 3;

int boundarytest(int dim, int val) {
	if (val == 0) {
		return dim;
	}
	else if (val > dim) {
		return 1;
	}
	else return val;
}


int cellsearch(int* CGnum, int* CGinfo, int total_element, int x, int y, int z) {
	int resultcount = 0;
	int rtval = 0;
	for (int i = 0; i < total_element; i++) {
		if (x == *(CGinfo + i * 3) && y == *(CGinfo + i * 3 + 1) && z == *(CGinfo + i * 3 + 2)) {
			rtval = i;
			break;
		}
		else continue;
	}
	return rtval + 1;
}


int intabs(int x) {
	if (x < 0) return -x;
	else if (x > 0) return x;
	else return 0;
}


int condsearch(int* CGnum, int* CGinfo, int Sig_Row, int rtindex[4]) {
	static int Nx = 2;
	static int Ny = 2;
	static int Nz = 2;

	static int N2 = (int)(Nx * Ny);
	int N3 = Nz * N2;

	int CGnumber = (Sig_Row) / 3 + 1;

	int x, y, z;
	x = *(CGinfo + (CGnumber - 1) * 3);
	y = *(CGinfo + (CGnumber - 1) * 3 + 1);
	z = *(CGinfo + (CGnumber - 1) * 3 + 2);

	int newx, newy, newz;
	newx = boundarytest(Nx, x + 1);
	newy = boundarytest(Ny, y + 1);
	newz = boundarytest(Nz, z + 1);

	rtindex[0] = CGnumber;
	rtindex[1] = cellsearch(CGnum, CGinfo, N3, newx, y, z);
	rtindex[2] = cellsearch(CGnum, CGinfo, N3, x, newy, z);
	rtindex[3] = cellsearch(CGnum, CGinfo, N3, x, y, newz);

	return 1;
}


int comp(double* XCompo, double* YCompo, double* ZCompo, double* UMatrix) {
	for (int i = 0; i < ELENUM; i++) {
		for (int j = 0; j < DIM; j++) {
			*(UMatrix + 3 * i * DIM + j) = *(XCompo + i * DIM + j);
			*(UMatrix + (3 * i + 1) * DIM + j) = *(YCompo + i * DIM + j);
			*(UMatrix + (3 * i + 2) * DIM + j) = *(ZCompo + i * DIM + j);
		}
	}


	return 0;
}


int sort(int CGnum[], int n) {
	int temp = 0;
	for (int i = 0; i < n - 1; i++) {
		for (int j = i + 1; j < n; j++) {
			if (CGnum[j] < CGnum[i]) {
				temp = CGnum[i];
				CGnum[i] = CGnum[j];
				CGnum[j] = temp;
			}
		}
	}

	return 0;
}


int datalloc(double* indata, double* otsigma, int row, int CGnum[], int ref[], int n) {
	for (int inx = 0; inx < n; inx++) {
		if ((row/3+1) <= CGnum[inx]) {
			*(otsigma + DIM * row + (CGnum[inx] - 1) * 3 + 0) = *(indata + DIM * (row % 3) + (ref[inx] - 1) * 3 + 0);
			*(otsigma + DIM * row + (CGnum[inx] - 1) * 3 + 1) = *(indata + DIM * (row % 3) + (ref[inx] - 1) * 3 + 1);
			*(otsigma + DIM * row + (CGnum[inx] - 1) * 3 + 2) = *(indata + DIM * (row % 3) + (ref[inx] - 1) * 3 + 2);
		}
		else {
			*(otsigma + DIM * row + (CGnum[inx] - 1) * 3 + 0) = *(indata + (row % 3) + (ref[inx] - 1) * 3 + 0 * DIM);
			*(otsigma + DIM * row + (CGnum[inx] - 1) * 3 + 1) = *(indata + (row % 3) + (ref[inx] - 1) * 3 + 1 * DIM);
			*(otsigma + DIM * row + (CGnum[inx] - 1) * 3 + 2) = *(indata + (row % 3) + (ref[inx] - 1) * 3 + 2 * DIM);
		}
	}

	return 0;
}
