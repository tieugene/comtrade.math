"""Comtrade math: demo original math operations"""
import sys
import math
import cmath
import numpy as np

DT = 1000. / 1200.0  # samples time step
NWIND = 24  # samples per period
HEAD_MATLAB =\
    "         n   time, ms     sample       mean        RMS"\
    "    H1, abs              H1, deg              H2, abs    H3, abs    H5, abs"
FMT_MATLAB =\
    "%10d %10.4f %10.4f %10.4f %10.4f %10.4f %20.4f %20.4f %10.4f %10.4f"


def mean(a: np.array, n: int, w: int) -> float:
    return np.sum(a[max(n + 1 - w, 0):n + 1]) / w


def rms(a: np.array, n: int, w: int) -> float:
    return np.sqrt(sum(np.array(a[max(n + 1 - w, 0):n + 1]) ** 2) / w)


def sft(a: np.array, n: int, w: int, h: int) -> complex:
    if n < (w - 1):
        return 0+0j
    win = a[n+1-w:n+1]
    mult_2 = 2 / w / math.sqrt(2)
    mult_1 = 2 * math.pi * h / w
    arg = n * mult_1
    sum_r = sum_i = 0.0
    for i, v in enumerate(win):
        arg += mult_1  # arg = (n + 1 + i) * mult_1
        sum_r += v * math.sin(arg)
        sum_i += v * math.cos(arg)
    return complex(sum_r * mult_2, sum_i * mult_2)


def main():
    if len(sys.argv) != 2:
        sys.exit(f"Usage: {sys.argv[0]} <infile> [> outfile]")
    # 1. Load data
    data = []
    for line in open(sys.argv[1], 'rt').readlines():
        data.append(float(line.strip()))
    data = np.array(data)
    # 2-3. Calc and print result
    for i in range(len(data)):
        print(FMT_MATLAB % (
            i+1,
            -500.0 + i * DT,
            data[i],
            mean(data, i, NWIND),
            rms(data, i, NWIND),
            # (h1 := cmath.polar(sft(data, i, NWIND, 1)))[0],  # .cmathpolar() == (.abs(), .phase())
            # math.degrees(h1[1]),
            abs(h1 := sft(data, i, NWIND, 1)),
            math.degrees(cmath.phase(h1)),
            abs(sft(data, i, NWIND, 2)),
            abs(sft(data, i, NWIND, 3)),
            abs(sft(data, i, NWIND, 5))
        ))


if __name__ == '__main__':
    main()
