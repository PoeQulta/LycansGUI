#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "commthread.h"
#include <QMainWindow>
#include <QtCharts/QChart>
#include <QTimer>
#include <QTime>
#include "chart.h"


class QLineSeries;
class QChart;
class QLabel;
class QPushButton;
class QComboBox;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void transaction();
    void showResponse(const QString &s);
    void processError(const QString &s);
    void processTimeout(const QString &s);
    void updateChart();
    void on_pushButton_Payload_clicked();

    void on_pushButton_CDA_clicked();

private:
    Ui::MainWindow *ui;
    void setControlsEnabled(bool enable);
private:
    int transactionCount;
    CommThread thread;
    Chart *chart = new Chart;
    QTimer m_timer;
};

#endif // MAINWINDOW_H
