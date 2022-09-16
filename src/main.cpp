/* TODO:
 * - [x] return new
 * - [x] Sample: vector
 * - [x] ~~double > float~~
 * - [x] stdin|argv[1]
 * - [ ] SFT: complex
 * - [ ] unify functions (wrap)
 * - [ ] FIXME: handle 'not argv[1] nor stdin'
 */
#include <vector>
#include <algorithm>
#include <cmath>
#include <complex>
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
complex<double> *Fourier(vector<double>&, int Nwind, int Nharm);

int main(int argc, char *argv[]) {
  istream *infile_ptr;
  ifstream infile;
  vector<double> Samples;
  string s = "";
  int countS = 0;
  int NstrLen = 0;
  double Samp = 0.0;
  int out = 0;
  double dt = 1000. / 1200;
  int Nwind = 24; // samples per period

  // 00. Prepare input
  if (argc < 2)
      infile_ptr = &cin;
  else {
      infile.open(argv[1], ios::in);
      if (!infile.is_open()) {
          cerr << "Can't open hash file: " << argv[1] << endl;
          exit(EXIT_FAILURE);
      }
      infile_ptr = &infile;
  }
  // 01. Читаем исходные отсчёты
  while (!getline(*infile_ptr, s).eof()) {
    Samples.push_back(atof(s.c_str()));
    countS++;
  }
  if (!countS) {
      cerr << "No input data" << endl;
      exit(EXIT_FAILURE);
  }
  // 02. Расчет среднего значения
  auto OutMean = MeanCount(Samples, Nwind);
  // 03. Расчёт действующего значения
  auto OutRMS = RMSCount(Samples, Nwind);
  // 04. Фильтр Фурье
  auto OutSFT1 = Fourier(Samples, Nwind, 1);
  auto OutSFT2 = Fourier(Samples, Nwind, 2);
  auto OutSFT3 = Fourier(Samples, Nwind, 3);
  auto OutSFT5 = Fourier(Samples, Nwind, 5);

  printf("%s", HEAD_MATLAB);
  for (int n = 0; n < countS; n++)
    printf(FMT_MATLAB, n + 1, -500.0 + n * dt, Samples[n], OutMean[n], OutRMS[n],
        abs(OutSFT1[n]),
        arg(OutSFT1[n]) * 180.0 / PI,
        abs(OutSFT2[n]),
        abs(OutSFT3[n]),
        abs(OutSFT5[n]));
  return 0;
}
