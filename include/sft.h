#ifndef FOURIER_H_INCLUDED
#define FOURIER_H_INCLUDED

#include <iostream>
#include <algorithm>
#include <fstream>
#include <time.h>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <complex.h>
#include <cmath>
#include <string>
#include <iomanip>

using namespace std;
int Fourier(double *Samples, int Nwind, int Len, double *OutReal, double *OutImag, int Nharm);

#endif // FOURIER_H_INCLUDED
