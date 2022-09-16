/* TODO:
 * - [x] return new
 * - [x] Sample: vector
 * - [ ] double > float
 * - [ ] SFT: complex
 * - [ ] stdin|argv[1]
 * - [ ] unify functions (wrap)
 */
#include <vector>
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
    "         n   time, ms     sample       mean        RMS    H1, abs              H1, deg              H2, abs    H3, abs    H5, abs\n";
char FMT_MATLAB[] =
    "%10d %10.4f %10.4f %10.4f %10.4f %10.4f %20.4f %20.4f %10.4f %10.4f\n";

double *MeanCount(vector<double>&, int);
double *RMSCount(vector<double>&, int);
int Fourier(vector<double>&, int Nwind, double *OutReal, double *OutImag, int Nharm);

int main() {
  int leng = 1e6;
  vector<double> Samples;
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
    Samples.push_back(atof(s.c_str()));
    countS++;
  }
  file.close();
  // 02. Расчет среднего значения
  auto OutMean = MeanCount(Samples, Nwind);
  // 03. Расчёт действующего значения
  auto OutRMS = RMSCount(Samples, Nwind);
  // 04. Фильтр Фурье
  out = Fourier(Samples, Nwind, OutReal1, OutImag1, 1);
  out = Fourier(Samples, Nwind, OutReal2, OutImag2, 2);
  out = Fourier(Samples, Nwind, OutReal3, OutImag3, 3);
  out = Fourier(Samples, Nwind, OutReal5, OutImag5, 5);

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
