#include <algorithm>
#include <cmath>

using namespace std;
#define PI numbers::pi

/// Расчёт ортогональных составляющих. Примечание: определяется фаза синусоиды
/// входного сигнала (а не косинусоиды)
int Fourier(double *Samples, int Nwind, int Len, double *OutReal,
            double *OutImag, int Nharm) {
  /// INPUT:
  // Samples - указатель на входной массив отсчётов входного сигнала;
  // Nwind - длина окна наблюдения
  // Len - длина входного массива
  // OutReal - указатель на выходной массив реальной части
  // OutImag - указатель на выходной массив мнимой части
  // Nharm - номер расчитываемой гармоники

  double *Yind =
      new double[Nwind]; // массив для хранения отсчётов в окне наблюдения
  double *sin_ = new double[Nwind]; // массив для хранения отсчётов ортогон.
                                    // составляющей для реальной части
  double *cos_ = new double[Nwind]; // - // - для мнимой
  int indStart = 0; // индекс первого отсчёта в исходном сигнале (не в окне)
  int Nsamp = 0;    // расчёт (действующего значения)
  int indStartWind =
      0; // индекс первого отсчёта, который нужно заполнять в окне наблюдения
  double sum_ = 0.0; // урна

  for (int m = 0; m < Len;
       m++) // для каждого отсчёта в исходном сигнале заполняем окно
  {
    indStart =
        m - (Nwind - 1); // индекс первого (левого) отсчёта в исходном сигнале,
                         // который нужно записать в окно наблюдения
    if (indStart < 0) // если текущий отсчёт m не вышел за Nwind отсчётов, то
                      // надо начинать с нулевого отсчёта
      indStart = 0;

    // число заполненных ненулевых отсчётов в окне
    if (m < Nwind - 1) // если окно ещё не заполнилось полностью
    {
      Nsamp = m + 1;
    } else
      Nsamp = Nwind; // если заполнилось полностью

    // индекс первого ненулевого элемента в окне (окно заполняется с конца)
    indStartWind = Nwind - Nsamp;

    for (int n = 0; n < Nwind; n++) // обнуление элементов в окне
      Yind[n] = 0.0;

    for (int n = 0; n < Nsamp;
         n++) // копирование отсчётов из сигнала в окно наблюдения
      Yind[indStartWind + n] = Samples[indStart + n];

    // ортогональные составляющие: обнуление перед каждой итерацией цикла
    for (int n = 0; n < Nwind; n++) {
      sin_[n] = 0.0;
      cos_[n] = 0.0;
    }

    for (int n = 0; n < Nwind; n++) {
      sin_[n] = sin(2 * PI * Nharm * (m + 1 + n) /
                    Nwind); // для реальной части (определяем фазу синусоиды, а
                            // не косинусоиды)
      cos_[n] = cos(2 * PI * Nharm * (m + 1 + n) /
                    Nwind); // для мнимой: если бы определяли фазу косинусоиды,
                            // то было бы наоборот
    }

    sum_ = 0.0; // реальная часть
    for (int n = 0; n < Nwind; n++)
      sum_ = sum_ + Yind[n] * sin_[n];
    OutReal[m] = 2 * sum_ / Nwind;

    sum_ = 0.0; // мнимая часть
    for (int n = 0; n < Nwind; n++)
      sum_ = sum_ + Yind[n] * cos_[n];
    OutImag[m] = 2 * sum_ / Nwind;
    OutReal[m] = OutReal[m] /
                 sqrt(2); // действующее значение гармоники, а не амплитудное
    OutImag[m] = OutImag[m] /
                 sqrt(2); // действующее значение гармоники, а не амплитудное
  }

  delete[] Yind;
  delete[] sin_;
  delete[] cos_;
  return 0;
}
