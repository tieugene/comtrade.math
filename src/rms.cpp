#include <cmath>
#include <vector>

double *RMSCount(std::vector<double> &Samples, int Nwind) {
  /// INPUT:
  // Samples - указатель на входной массив отсчётов входного сигнала;
  // Nwind - длина окна наблюдения
  // Len - длина входного массива
  // Out - указатель на выходной массив
  double *Yind = new double[Nwind]; // массив для хранения отсчётов в текущем окне наблюдения
  int indStart = 0; // индекс первого отсчёта в исходном сигнале (не в окне)
  int Nsamp = 0; // число заполненных отсчётов в текущем окне число заполненных отсчётов в текущем окне
  int indStartWind = 0; // индекс первого отсчёта, который нужно заполнять в окне наблюдения
  double sum_ = 0.0; // урна

  auto Len = Samples.size();
  auto Out = new double[Len];
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
    // расчёт (действующего значения)
    sum_ = 0.0;
    for (int n = 0; n < Nwind; n++)
      sum_ = sum_ + Yind[n] * Yind[n];
    Out[m] = sqrt(sum_ / (double)Nwind);
  }
  delete[] Yind;
  return Out;
}
