#include <algorithm>
#include <cmath>
#include <complex.h>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <time.h>

using namespace std;
#define PI numbers::pi
char HEAD_MATLAB[] =
    "         n   time, ms     sample       mean        RMS    H1, abs         "
    "     H1, deg              H2, abs    H3, abs    H5, abs\n";
char FMT_MATLAB[] =
    "%10d %10.4f %10.4f %10.4f %10.4f %10.4f %20.4f %20.4f %10.4f %10.4f\n";

int MeanCount(double *Samples, int Nwind, int Len, double *Out);
int RMSCount(double *Samples, int Nwind, int Len, double *Out);
int Fourier(double *Samples, int Nwind, int Len, double *OutReal,
            double *OutImag, int Nharm);

int main() {
  int leng = 1e6;
  double *Samples = new double[leng];
  double *OutMean = new double[leng];
  double *OutRMS = new double[leng];
  double *OutReal1 = new double[leng];
  double *OutImag1 = new double[leng];
  double *OutReal2 = new double[leng];
  double *OutImag2 = new double[leng];
  double *OutReal3 = new double[leng];
  double *OutImag3 = new double[leng];
  double *OutReal5 = new double[leng];
  double *OutImag5 = new double[leng];
  string s = "";
  int countS = 0;
  int NstrLen = 0;
  double Samp = 0.0;
  int out = 0;
  double dt = 1000. / 1200;
  int Nwind = 24; // samples per period

  // 01. Читаем исходные отсчёты
  ifstream file("InitSamples.txt");
  while (getline(file, s)) {
    Samples[countS] = atof(s.c_str());
    countS++;
  }
  file.close();

  // 02. Расчет среднего значения
  out = MeanCount(Samples, Nwind, countS, OutMean);
  // 03. Расчёт действующего значения
  out = RMSCount(Samples, Nwind, countS, OutRMS);
  // 04. Фильтр Фурье
  out = Fourier(Samples, Nwind, countS, OutReal1, OutImag1, 1);
  out = Fourier(Samples, Nwind, countS, OutReal2, OutImag2, 2);
  out = Fourier(Samples, Nwind, countS, OutReal3, OutImag3, 3);
  out = Fourier(Samples, Nwind, countS, OutReal5, OutImag5, 5);

  printf("%s", HEAD_MATLAB);
  for (int n = 0; n < countS; n++)
    printf(FMT_MATLAB, n + 1, -500.0 + n * dt, Samples[n], OutMean[n],
           OutRMS[n], cabs(OutReal1[n] + OutImag1[n] * I),
           carg(OutReal1[n] + OutImag1[n] * I) * 180.0 / PI,
           cabs(OutReal2[n] + OutImag2[n] * I),
           cabs(OutReal3[n] + OutImag3[n] * I),
           cabs(OutReal5[n] + OutImag5[n] * I));
  return 0;
}
