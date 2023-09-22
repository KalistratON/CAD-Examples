#ifndef QPLOT_H
#define QPLOT_H

#include <QWidget>
#include <QPainter>
#include <QVector>
#include <QPointF>

class QPlot : public QWidget
{
    Q_OBJECT

    QVector<QPointF> points;
    qsizetype i = 1;
    uint scale = 25;

    qreal durance = 0;
    qreal start = 0;
    qreal step = 0;

public:
    explicit QPlot(QWidget *parent = nullptr);
    void setPoints(const QVector<QPair<qreal, qreal>>&);
    void setParameters(qreal, qreal, qreal);

protected:
    void paintEvent(QPaintEvent *event);

private:

};

#endif // QPLOT_H
