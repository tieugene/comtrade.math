#include <algorithm>
#include <cmath>
#include <vector>
#include <complex>

/// Расчёт ортогональных составляющих.
/// Примечание: определяется фаза синусоиды входного сигнала (а не косинусоиды)
std::complex<double> *Fourier(std::vector<double> &Samples, int Nwind, int Nharm) {
  /// INPUT:
  // Samples - указатель на входной массив отсчётов входного сигнала;
  // Nwind - длина окна наблюдения
  // Nharm - номер расчитываемой гармоники

  double *Yind = new double[Nwind]; // массив для хранения отсчётов в окне наблюдения
  double *sin_ = new double[Nwind]; // массив для хранения отсчётов ортогон. составляющей для реальной части
  double *cos_ = new double[Nwind]; // - // - для мнимой
  int indStart = 0; // индекс первого отсчёта в исходном сигнале (не в окне)
  int Nsamp = 0;    // расчёт (действующего значения)
  int indStartWind = 0; // индекс первого отсчёта, который нужно заполнять в окне наблюдения
  double sum_ = 0.0; // урна

  auto Len = Samples.size();
  auto Out = new std::complex<double>[Len];
  for (int m = 0; m < Len; m++) // для каждого отсчёта в исходном сигнале заполняем окно
  {
    indStart = m - (Nwind - 1); // индекс первого (левого) отсчёта в исходном сигнале, который нужно записать в окно наблюдения
    if (indStart < 0) // если текущий отсчёт m не вышел за Nwind отсчётов, то надо начинать с нулевого отсчёта
      indStart = 0;
    // число заполненных ненулевых отсчётов в окне
    if (m < Nwind - 1) // если окно ещё не заполнилось полностью
      Nsamp = m + 1;
    else
      Nsamp = Nwind; // если заполнилось полностью
    // индекс первого ненулевого элемента в окне (окно заполняется с конца)
    indStartWind = Nwind - Nsamp;
    for (int n = 0; n < Nwind; n++) // обнуление элементов в окне
      Yind[n] = 0.0;
    for (int n = 0; n < Nsamp; n++) // копирование отсчётов из сигнала в окно наблюдения
      Yind[indStartWind + n] = Samples[indStart + n];
    // ортогональные составляющие: обнуление перед каждой итерацией цикла
    for (int n = 0; n < Nwind; n++)
      sin_[n] = cos_[n] = 0.0;
    for (int n = 0; n < Nwind; n++) {
      auto arg = 2 * std::numbers::pi * Nharm * (m + 1 + n) / Nwind;
      sin_[n] = sin(arg); // для реальной части (определяем фазу синусоиды, а не косинусоиды)
      cos_[n] = cos(arg); // для мнимой: если бы определяли фазу косинусоиды, то было бы наоборот
    }
    sum_ = 0.0; // реальная часть
    for (int n = 0; n < Nwind; n++)
      sum_ += Yind[n] * sin_[n];
    auto real = 2 * sum_ / Nwind / sqrt(2); // sqrt(2): действующее значение гармоники, а не амплитудное
    sum_ = 0.0; // мнимая часть
    for (int n = 0; n < Nwind; n++)
      sum_ += Yind[n] * cos_[n];
    auto imag = 2 * sum_ / Nwind / sqrt(2); // действующее значение гармоники, а не амплитудное
    Out[m] = std::complex<double>(real, imag);
  }
  delete[] Yind;
  delete[] sin_;
  delete[] cos_;
  return Out;
}
