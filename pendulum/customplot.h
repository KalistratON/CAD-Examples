#ifndef CUSTOMPLOT_H
#define CUSTOMPLOT_H

#include <QWidget>

namespace Ui {
class CustomPlot;
}

class CustomPlot : public QWidget
{
    Q_OBJECT

public:
    explicit CustomPlot(QWidget *parent = nullptr);
    ~CustomPlot();

private:
    Ui::CustomPlot *ui;
};

#endif // CUSTOMPLOT_H
