#ifndef ORTHOGONALIZATION_H
#define ORTHOGONALIZATION_H

#include <QVector>
#include "polynomial.h"

class Orthogonalization
{
public:
    Orthogonalization() = default;
    QVector<Polynomial> otho(const QVector<Polynomial>&) const;
};

#endif // ORTHOGONALIZATION_H
