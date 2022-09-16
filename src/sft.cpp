#include <vector>
#include <deque>
#include <complex>

/**
 * Расчёт ортогональных составляющих.
 * @note определяется фаза синусоиды входного сигнала, а не косинусоиды.
 *
 * @param Samples subj.
 * @param Nwind Running window width.
 * @param Nharm Harmonic number.
 * @return ?.
 */
std::complex<double> *Fourier(std::vector<double> &Samples, int Nwind, int Nharm) {
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
