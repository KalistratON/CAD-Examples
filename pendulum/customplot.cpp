#include "customplot.h"
#include "ui_customplot.h"

CustomPlot::CustomPlot(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CustomPlot)
{
    ui->setupUi(this);
}

CustomPlot::~CustomPlot()
{
    delete ui;
}
