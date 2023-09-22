#include "qplot.h"

QPlot::QPlot(QWidget *parent)
    : QWidget{parent}
{

}

void QPlot::setPoints(const QVector<QPair<qreal, qreal>>& _points)
{
    if(points.size())
        points.clear();
    for (auto& el : _points)
        points.append(QPointF(el.first + 5.0, el.second + 5.0));
}

void QPlot::setParameters(qreal _d, qreal _start, qreal _step)
{
    durance = _d;
    start = _start;
    step = _step;

    setMinimumSize(durance * scale, durance * scale);
    setMaximumSize(durance * scale, durance * scale);
}

void QPlot::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setPen(QPen(Qt::black, 2.5, Qt::SolidLine));

    for (qsizetype j = 0; j < i; j++)
    {
        painter.drawLine(j * step * scale, points[j].x() * scale, (j + 1) * step * scale, points[j + 1].x() * scale);
    }
    i++;
}
