#include "mainwindow.h"
#include "polynomial.h"
#include "orthogonalization.h"
#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    //QApplication a(argc, argv);
    //MainWindow w;
    //w.show();
    //return a.exec();

    QFile file("output.txt");
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream stream(&file);


    QVector<Polynomial> poly;
    qsizetype size = 100;
    for(qsizetype i = 0; i < size; i++)
    {
        QVector<qreal> temp(i + 1);
        temp[i] = 1;
        poly.push_back(Polynomial(temp));
    }

    Orthogonalization obj;
    bool flag = true;
    poly = obj.otho(poly);

    qreal lim = 1;
    qreal norm = poly[0].integral();
    for (auto& p : poly[0].arr)
        stream << p << ' ';
    for (size_t i = poly[0].arr.size(); i < size; i++)
        stream << 0 << ' ';
    stream << '\n';
    for(qsizetype i = 1; i < poly.size(); i++)
    {
        for (auto& p : poly[i].arr)
            stream << p << ' ';
        for (size_t j = poly[i].arr.size(); j < size; j++)
            stream << 0 << ' ';
        stream << '\n';

        qreal norm_current = poly[i].integral();
        if (norm < norm_current && flag) {
            qDebug() << "Рост нормы : " << i << " : " << norm << " : " << norm_current;
            flag = false;
        }
        else
            norm = norm_current;
        for(qsizetype j = 0; j < i; j++)
        {
            qreal error = qAbs(poly[i].integral(poly[j]));

            if(error >= lim) {
                qDebug() << "Максимальная ошибка : " << i << " : " << j << " : " << error;
                return 0;
            }
        }
    }
    return 0;
}
