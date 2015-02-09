#include "affttransformer.h"

AFftTransformer::AFftTransformer()
{

}

AFftTransformer::~AFftTransformer()
{

}

void AFftTransformer::fft(const QVector<double> &in, int len, QVector<double> &out)
{
        // AVal - массив анализируемых данных, Nvl - длина массива должна быть кратна степени 2.
        // FTvl - массив полученных значений, Nft - длина массива должна быть равна Nvl.

        int i, j, n, m, Mmax, Istp;
        double Tmpr, Tmpi, Wtmp, Theta;
        double Wpr, Wpi, Wr, Wi;
        QVector<double> Tmvl;

        n = len * 2;
        Tmvl.resize(n);

        for (i = 0; i < n; i += 2) {
            Tmvl[i] = 0;
            Tmvl[i+1] = in.at(i / 2);
        }

        i = 1; j = 1;
        while (i < n) {
            if (j > i) {
                Tmpr = Tmvl[i]; Tmvl[i] = Tmvl[j]; Tmvl[j] = Tmpr;
                Tmpr = Tmvl[i+1]; Tmvl[i+1] = Tmvl[j+1]; Tmvl[j+1] = Tmpr;
            }
            i = i + 2; m = len;
            while ((m >= 2) && (j > m)) {
                j = j - m; m = m >> 1;
            }
            j = j + m;
        }

        Mmax = 2;
        while (n > Mmax) {
            Theta = -TwoPi / Mmax; Wpi = qSin(Theta);
            Wtmp = sin(Theta / 2); Wpr = Wtmp * Wtmp * 2;
            Istp = Mmax * 2; Wr = 1; Wi = 0; m = 1;

            while (m < Mmax) {
                i = m; m = m + 2; Tmpr = Wr; Tmpi = Wi;
                Wr = Wr - Tmpr * Wpr - Tmpi * Wpi;
                Wi = Wi + Tmpr * Wpi - Tmpi * Wpr;

                while (i < n) {
                    j = i + Mmax;
                    Tmpr = Wr * Tmvl[j] - Wi * Tmvl[j-1];
                    Tmpi = Wi * Tmvl[j] + Wr * Tmvl[j-1];

                    Tmvl[j] = Tmvl[i] - Tmpr; Tmvl[j-1] = Tmvl[i-1] - Tmpi;
                    Tmvl[i] = Tmvl[i] + Tmpr; Tmvl[i-1] = Tmvl[i-1] + Tmpi;
                    i = i + Istp;
                }
            }

            Mmax = Istp;
        }

        int fftOutBufferSize = out.size();

        for (i = 0; i < fftOutBufferSize; i++) {
            j = i * 2; out[i] = sqrt(pow(Tmvl[j],2) + pow(Tmvl[j+1],2));
        }
}
