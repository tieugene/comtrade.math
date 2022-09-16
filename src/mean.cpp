#include <vector>
#include <deque>

/**
 * Расчет среднего значения.
 *
 * @param Samples subj.
 * @param Nwind - running window width.
 * @return Running mean.
 */
double *MeanCount(std::vector<double> &Samples, int Nwind) {
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
