#include <iostream>
#include <algorithm>
#include <fstream>
#include <time.h>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <complex.h>
#include <cmath>
#include <string>
#include <iomanip>

using namespace std;
int MeanCount(double *Samples, int Nwind, int Len, double *Out)
{
    ///INPUT:
    // Samples - указатель на входной массив отсчётов входного сигнала;
    // Nwind - длина окна наблюдения
    // Len - длина входного массива
    // Out - указатель на выходной массив

    double *Yind = new double[Nwind];

    int indStart = 0;  // индекс первого отсчёта в исходном сигнале (не в окне)
    int Nsamp = 0; // число заполненных отсчётов в текущем окне
    int indStartWind = 0;// индекс первого отсчёта, который нужно заполнять в окне наблюдения
    double sum_ = 0.0;  // урна

    for (int m = 0; m < Len; m++) // для каждого отсчёта в исходном сигнале заполняем окно
    {
        indStart = m - (Nwind - 1);// индекс первого (левого) отсчёта в исходном сигнале, который нужно записать в окно наблюдения
        if (indStart < 0) // если текущий отсчёт m не вышел за Nwind отсчётов, то надо начинать с нулевого отсчёта
        {
            indStart = 0;
        }

        // число заполненных ненулевых отсчётов в окне
        if (m < Nwind - 1) // если окно ещё не заполнилось полностью
        {
            Nsamp = m + 1;
        }
        else
        {
            Nsamp = Nwind;
        }

        // индекс первого ненулевого элемента в окне (окно заполняется с конца)
        indStartWind = Nwind - Nsamp;


        for (int n = 0; n < Nwind; n++)
        {
            Yind[n] = 0.0; // обнуление элементов в окне
        }

        for (int n = 0; n < Nsamp; n++)
        {
            Yind[indStartWind + n] = Samples[indStart + n]; // копирование отсчётов из сигнала в окно наблюдения
        }

        // расчёт (среднего значения)
        sum_ = 0.0;
        for (int n = 0; n < Nwind; n++)
        {
            sum_ = sum_ + Yind[n];
        }
        Out[m] = sum_ / (double)Nwind;

    }

    delete[] Yind;
    return 0;
}
