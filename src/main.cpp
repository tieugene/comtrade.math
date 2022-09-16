#include <cstdlib>
#include <fstream>
#include <iostream>
#include <vector>
#include <complex>

const double dt = 1000. / 1200;  // samples time step
const int Nwind = 24; // samples per period
char HEAD_MATLAB[] =
    "         n   time, ms     sample       mean        RMS    H1, abs              H1, deg              H2, abs    H3, abs    H5, abs\n";
char FMT_MATLAB[] =
    "%10d %10.4f %10.4f %10.4f %10.4f %10.4f %20.4f %20.4f %10.4f %10.4f\n";

double *MeanCount(std::vector<double>&, int);
double *RMSCount(std::vector<double>&, int);
std::complex<double> *Fourier(std::vector<double>&, int, int);

int main(int argc, char *argv[]) {
  std::istream *infile_ptr;
  std::ifstream infile;
  std::vector<double> Samples;
  std::string s = "";
  int countS = 0;

  // 0. Prepare input
  if (argc < 2)
      infile_ptr = &std::cin;
  else {
      infile.open(argv[1], std::ios::in);
      if (!infile.is_open()) {
          std::cerr << "Can't open input file: " << argv[1] << std::endl;
          exit(EXIT_FAILURE);
      }
      infile_ptr = &infile;
  }
  // 1. Read samples
  for (countS=0; !getline(*infile_ptr, s).eof(); countS++)
    Samples.push_back(atof(s.c_str()));
  if (!countS) {
      std::cerr << "No input data" << std::endl;
      exit(EXIT_FAILURE);
  }
  // 2. Calc required
  auto OutMean = MeanCount(Samples, Nwind);  // 1.1. Расчет среднего значения
  auto OutRMS = RMSCount(Samples, Nwind);    // 1.2. Расчёт действующего значения
  auto OutSFT1 = Fourier(Samples, Nwind, 1); // 1.3. Фильтр Фурье
  auto OutSFT2 = Fourier(Samples, Nwind, 2);
  auto OutSFT3 = Fourier(Samples, Nwind, 3);
  auto OutSFT5 = Fourier(Samples, Nwind, 5);
  // 3. Print results
  printf("%s", HEAD_MATLAB);
  for (int n = 0; n < countS; n++)
    printf(FMT_MATLAB, n + 1, -500.0 + n * dt, Samples[n], OutMean[n], OutRMS[n],
        abs(OutSFT1[n]),
        arg(OutSFT1[n]) * 180.0 / std::numbers::pi,
        abs(OutSFT2[n]),
        abs(OutSFT3[n]),
        abs(OutSFT5[n]));
  return 0;
}
