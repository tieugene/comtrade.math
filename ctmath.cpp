#include <cstdlib>
#include <fstream>
#include <iostream>
#include <vector>
#include <deque>
#include <complex>

const double dt = 1000. / 1200;  // samples time step
const int Nwind = 24; // samples per period
char HEAD_MATLAB[] =
    "         n   time, ms     sample       mean        RMS    H1, abs              H1, deg              H2, abs    H3, abs    H5, abs\n";
char FMT_MATLAB[] =
    "%10d %10.4f %10.4f %10.4f %10.4f %10.4f %20.4f %20.4f %10.4f %10.4f\n";

double *Mean(std::vector<double>&, int);
double *RMS(std::vector<double>&, int);
std::complex<double> *SFT(std::vector<double>&, int, int);

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
  auto OutMean = Mean(Samples, Nwind);   // 1.1. Расчет среднего значения
  auto OutRMS = RMS(Samples, Nwind);     // 1.2. Расчёт действующего значения
  auto OutSFT1 = SFT(Samples, Nwind, 1); // 1.3. Фильтр Фурье
  auto OutSFT2 = SFT(Samples, Nwind, 2);
  auto OutSFT3 = SFT(Samples, Nwind, 3);
  auto OutSFT5 = SFT(Samples, Nwind, 5);
  // 3. Print results
  printf("%s", HEAD_MATLAB);
  for (int n = 0; n < countS; n++)
    printf(FMT_MATLAB, n + 1, -500.0 + n * dt, Samples[n], OutMean[n], OutRMS[n],
        abs(OutSFT1[n]),
        arg(OutSFT1[n]) * 180.0 / std::numbers::pi,
        abs(OutSFT2[n]),
        abs(OutSFT3[n]),
        abs(OutSFT5[n]));
  delete[] OutMean;
  delete[] OutRMS;
  return 0;
}

/**
 * Расчет среднего значения.
 *
 * @param Samples subj.
 * @param Nwind - running window width.
 * @return Running mean.
 */
double *Mean(std::vector<double> &Samples, int Nwind) {
    std::deque<double> Yind(Nwind, 0.0);
    auto Len = Samples.size();
    auto Out = new double[Len];

    for (int m = 0; m < Len; m++) {
        Yind.pop_front();
        Yind.push_back(Samples[m]);
        double sum_ = 0.0;
        for (auto d : Yind)
            sum_ += d;
        Out[m] = sum_ / (double) Nwind;
    }
    return Out;
}

/**
 * Root Mean Square - Расчёт действующего значения.
 *
 * @param Samples subj.
 * @param Nwind - running window width.
 * @return RMS.
 */
double *RMS(std::vector<double> &Samples, int Nwind) {
    std::deque<double> Yind(Nwind, 0.0);
    auto Len = Samples.size();
    auto Out = new double[Len];

    for (int m = 0; m < Len; m++) {
        Yind.pop_front();
        Yind.push_back(Samples[m]);
        double sum_ = 0.0;
        for (auto d : Yind)
            sum_ += pow(d, 2);
        Out[m] = sqrt(sum_ / (double) Nwind);
    }
    return Out;
}

/**
 * Slow Fourier Transformation - Расчёт ортогональных составляющих.
 * @note определяется фаза синусоиды входного сигнала, а не косинусоиды.
 *
 * @param Samples subj.
 * @param Nwind Running window width.
 * @param Nharm Harmonic number.
 * @return ?.
 */
std::complex<double> *SFT(std::vector<double> &Samples, int Nwind, int Nharm) {
    std::deque<double> Yind(Nwind, 0.0);
    auto Len = Samples.size();
    auto Out = new std::complex<double>[Len];

    for (int m = 0; m < Len; m++) {
        Yind.pop_front();
        Yind.push_back(Samples[m]);
        double sum_r = 0.0, sum_i = 0.0; // реальная часть, мнимая часть
        for (int n = 0; n < Nwind; n++) {
            auto arg = 2 * std::numbers::pi * Nharm * (m + 1 + n) / Nwind;
            sum_r += Yind[n] * sin(arg); // для реальной части (определяем фазу синусоиды, а не косинусоиды)
            sum_i += Yind[n] * cos(arg); // для мнимой: если бы определяли фазу косинусоиды, то было бы наоборот
        }
        // sqrt(2): действующее значение гармоники, а не амплитудное
        Out[m] = std::complex<double>(2 * sum_r / Nwind / sqrt(2), 2 * sum_i / Nwind / sqrt(2));
    }
    return Out;
}
