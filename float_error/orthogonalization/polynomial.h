#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <QVector>

class Polynomial
{

public:
    QVector<qreal> arr;

private:
    explicit Polynomial(qsizetype);

public:
    explicit Polynomial(const QVector<qreal>& _arr);
    explicit Polynomial(QVector<qreal>&& _arr);
    Polynomial(const Polynomial&) = default;
    ~Polynomial() = default;

    Polynomial operator * (const Polynomial& rht) const;
    Polynomial operator * (qreal coef) const;
    Polynomial operator + (const Polynomial& rht) const;
    Polynomial operator - (const Polynomial& rht) const;

    qreal integral() const;
    qreal integral(const Polynomial& rht) const;
};

#endif // POLYNOMIAL_H
