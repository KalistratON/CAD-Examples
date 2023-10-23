#include "orthogonalization.h"

QVector<Polynomial> Orthogonalization::otho(const QVector<Polynomial> &in) const
{
    QVector<Polynomial> out;
    out.push_back(in[0]);

    for(qsizetype i = 1; i < in.size(); i++)
    {
        Polynomial proj { QVector<qreal>() };
        for (qsizetype j = 0; j < i; j++)
            proj = proj + out[j] * (in[i].integral(out[j]) / out[j].integral());;
        out.push_back(in[i] - proj);
    }
    return out;
}
