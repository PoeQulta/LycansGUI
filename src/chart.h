#ifndef CHART_H
#define CHART_H

#include <QtCharts/QChart>
#include <QtCore/QTimer>

QT_CHARTS_BEGIN_NAMESPACE
class QSplineSeries;
class QValueAxis;
QT_CHARTS_END_NAMESPACE


class Chart : public QtCharts::QChart{
    Q_OBJECT

public:
    Chart(QGraphicsItem *parent = nullptr, Qt::WindowFlags wFlags = {});
        virtual ~Chart();
public slots:
    void handleTimeout(int data);
private:

    QtCharts::QSplineSeries *m_series;
    QStringList m_titles;
    QtCharts::QValueAxis *m_axisX;
    QtCharts::QValueAxis *m_axisY;
    qreal m_step;
    int m_x;
    int m_y;
};

#endif // CHART_H
