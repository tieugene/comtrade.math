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
#include "mean.h"
#include "rms.h"
#include "sft.h"

#define PI 3.14159265358979

using namespace std;

int main()
{
    int leng = 1e6;
    double* Samples = new double[leng];
    double* OutMean = new double[leng];
    double* OutRMS = new double[leng];
    double* OutReal1 = new double[leng];
    double* OutImag1 = new double[leng];
    double* OutReal2 = new double[leng];
    double* OutImag2 = new double[leng];
    double* OutReal3 = new double[leng];
    double* OutImag3 = new double[leng];
    double* OutReal5 = new double[leng];
    double* OutImag5 = new double[leng];

    double* timeMs = new double[leng];

    string s = "";
    int countS = 0;
    int NstrLen = 0;
    double Samp = 0.0;
    int out = 0;
    double dt = 1000./1200;

    int Nwind = 24; // число отсчётов на период для заданного файла



    // 01. Читаем исходные отсчёты
    ifstream file("InitSamples.txt");
    while(getline(file, s))
    {
        Samples[countS] = atof(s.c_str()); // преобразование в число
        timeMs[countS] = -500.0 + countS*dt; // заполнение массива времени (потому что в исходном файле только отсчёты)

        countS++;

    }
    file.close();


    // 02. Расчет среднего значения
    out = MeanCount(Samples, Nwind, countS, OutMean);

    // 03. Расчёт действующего значения
    out = RMSCount(Samples, Nwind, countS, OutRMS);

    // 04. Фильтр Фурье
    out = Fourier(Samples, Nwind, countS, OutReal1, OutImag1, 1);
    out = Fourier(Samples, Nwind, countS, OutReal2, OutImag2, 2);
    out = Fourier(Samples, Nwind, countS, OutReal3, OutImag3, 3);
    out = Fourier(Samples, Nwind, countS, OutReal5, OutImag5, 5);

    cout << "Mean === " << endl;
    for (int m = 0; m < 30; m++)
    {
        cout << "m + 1 = " << m + 1<< "; absA[m] = " <<  cabs(OutReal1[m] + OutImag1[m]*I) << endl;
    }

    ofstream FILEout_;
    FILEout_.open("OutSamples.txt");

    FILEout_ <<  "n, time,ms, sample, mean, RMS, abs(1Harm), angle(1Harm), deg., abs(2Harm), abs(3Harm), abs(5Harm)" << endl;

    for (int n = 0;  n < countS; n++)
    {
        FILEout_ <<  n + 1 << ", " << timeMs[n] << ", " << Samples[n] << ", " << OutMean[n] << ", " <<
            OutRMS[n] << ", " << cabs(OutReal1[n] + OutImag1[n]*I) << ", " << carg(OutReal1[n] + OutImag1[n]*I)*180.0/PI << ", "
                 <<  cabs(OutReal2[n] + OutImag2[n]*I) << ", "  << cabs(OutReal3[n] + OutImag3[n]*I) << ", " << cabs(OutReal5[n] + OutImag5[n]*I) <<  endl;
    }

    FILEout_.close();

    delete [] Samples;
    delete [] OutMean;
    delete [] OutRMS;
    delete [] OutReal1;
    delete [] OutImag1;
    delete [] OutReal2;
    delete [] OutImag2;
    delete [] OutReal3;
    delete [] OutImag3;
    delete [] OutReal5;
    delete [] OutImag5;
    delete[] timeMs;

    return 0;
}
