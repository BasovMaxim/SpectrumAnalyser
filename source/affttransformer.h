#ifndef AFTTTRANSFORMER_H
#define AFTTTRANSFORMER_H

#include <qmath.h>
#include <QVector>

const double TwoPi = 6.283185307179586;

class AFftTransformer
{
public:
    AFftTransformer();
    ~AFftTransformer();

    // Fast Furier transform algoritm.
    // Source: https://ru.wikibooks.org/wiki/Реализации_алгоритмов/Быстрое_преобразование_Фурье
    static void fft(const QVector<double> &in, int len, QVector<double> &out);
};

#endif // AFTTTRANSFORMER_H
