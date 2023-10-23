#include "polynomial.h"

Polynomial::Polynomial(qsizetype size) :
    arr(QVector<qreal>(size))
{

}

Polynomial::Polynomial(const QVector<qreal>& _arr) :
    arr(_arr)
{
    qsizetype i = arr.size() - 1;
    while(i >= 0 && !arr[i])
        --i;
    arr.resize(i + 1);
}

Polynomial::Polynomial(QVector<qreal>&& _arr) :
    arr(std::move(_arr))
{
    qsizetype i = arr.size() - 1;
    while(i >= 0 && !arr[i])
        --i;
    arr.resize(i + 1);
}

Polynomial Polynomial::operator * (qreal coef) const
{
    auto temp(*this);
    for (qsizetype i = 0; i < arr.size(); i++)
        temp.arr[i] *= coef;
    return temp;
}

Polynomial Polynomial::operator * (const Polynomial& rht) const
{
    qsizetype sizeLft = arr.size();
    qsizetype sizeRht = rht.arr.size();

    QVector<qreal> rst(sizeLft + sizeRht - 1, 0);

    for (qsizetype i = 0; i < sizeLft; i++)
    {
        for (qsizetype j = 0; j < sizeRht; j++)
            rst[i + j] += arr[i] * rht.arr[j];
    }

    return Polynomial(rst);
}

Polynomial Polynomial::operator +(const Polynomial &rht) const
{
    Polynomial temp(arr.size() > rht.arr.size() ? arr.size() : rht.arr.size());
    qsizetype i = -1;
    while (++i, i < arr.size() && i < rht.arr.size())
        temp.arr[i] = arr[i] + rht.arr[i];
    --i;
    while(++i < arr.size())
        temp.arr[i] = arr[i];
    --i;
    while(++i < rht.arr.size())
        temp.arr[i] = rht.arr[i];
    return temp;
}

Polynomial Polynomial::operator -(const Polynomial &rht) const
{
    Polynomial temp(arr.size() > rht.arr.size() ? arr.size() : rht.arr.size());
    qsizetype i = -1;
    while (++i, i < arr.size() && i < rht.arr.size())
        temp.arr[i] = arr[i] - rht.arr[i];
    --i;
    while(++i < arr.size())
        temp.arr[i] = arr[i];
    while(++i < rht.arr.size())
        temp.arr[i] = -rht.arr[i];
    return temp;
}

qreal Polynomial::integral() const
{
    if (arr.empty())
        return -1;

    Polynomial temp = *this * *this;
    qreal res = temp.arr[0];
    qsizetype size = temp.arr.size();
    for(qsizetype i = 1; i < size; i++)
        res += temp.arr[i] / (i + 1);

    return res;
}

qreal Polynomial::integral(const Polynomial& rht) const
{
    if (arr.empty())
        return -1;

    Polynomial temp = *this * rht;
    qreal res = temp.arr[0];
    qsizetype size = temp.arr.size();
    for(qsizetype i = 1; i < size; i++)
        res += temp.arr[i] / (i + 1);

    return res;
}
