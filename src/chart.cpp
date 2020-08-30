#include "chart.h"
#include <QtCharts/QAbstractAxis>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QValueAxis>
#include <QtCore>
#include <QtCore/QDebug>
//defines a specific chart type, inherits from the more generalized QChart
Chart::Chart(QGraphicsItem *parent, Qt::WindowFlags wFlags):QChart(QChart::ChartTypeCartesian, parent, wFlags),
    m_series(0),
    m_axisX(new QtCharts::QValueAxis()),
    m_axisY(new QtCharts::QValueAxis()),
    m_step(0),
    m_x(5),
    m_y(1)
{

       m_series = new QtCharts::QSplineSeries(this);
       QPen green(Qt::red);
       green.setWidth(3);
       m_series->setPen(green);
       m_series->append(m_x, m_y);

       addSeries(m_series);

       addAxis(m_axisX,Qt::AlignBottom);
       addAxis(m_axisY,Qt::AlignLeft);
       m_series->attachAxis(m_axisX);
       m_series->attachAxis(m_axisY);
       m_axisX->setTickCount(5);
       m_axisX->setRange(0, 600);
       m_axisY->setRange(0, 500);


}
Chart::~Chart()
{

}

void Chart::handleTimeout(int data)
{
    //updates chart
        m_x += 5;
        m_y = data;
        m_series->append(m_x, m_y);
        if (m_x % 5000 ==5){
            scroll(plotArea().width()/2, 0);
        }
}
