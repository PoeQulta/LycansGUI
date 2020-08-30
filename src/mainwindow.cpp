#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLabel>
#include <QStringList>
#include <QComboBox>
#include <QPushButton>
#include <QGridLayout>
#include "chart.h"
#include <QtCharts/QChartView>
#include <QtSerialPort/QSerialPortInfo>
#include <QGraphicsEllipseItem>
#include <QTime>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
 //sets up GUI data dynamically
    const auto infos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : infos)
        ui->comboPortList->addItem(info.portName());


    chart->setTitle("Aircraft Height");
    chart->legend()->hide();
    chart->setAnimationOptions(QtCharts::QChart::AllAnimations);
    ui->ChartView->setChart(chart);
    ui->ChartView->setRenderHint(QPainter::Antialiasing);
    m_timer.setInterval(1000);
    m_timer.start();
    QFile file ("Data.csv");
        file.remove();
    //sets up event handling
    connect(ui->pushButton_Connect, &QPushButton::clicked, this, &MainWindow::transaction);
    connect(&thread, &CommThread::response, this, &MainWindow::showResponse);
    connect(&thread, &CommThread::error, this, &MainWindow::processError);
    connect(&thread, &CommThread::timeout, this, &MainWindow::processTimeout);
    connect(&m_timer, &QTimer::timeout, this, &MainWindow::updateChart);
}
MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::transaction()
{
    //starts communication Thread
    ui->label_conStatus->setText(tr("Status: Running, connected to port %1.")
                            .arg(ui->comboPortList->currentText()));
       thread.transaction(ui->comboPortList->currentText(),
                          1000,"");
}
void MainWindow::showResponse(const QString &s)
{
    //updates GUI to display new data
    QStringList data = s.split(QRegExp("[\r\n]"),QString::SkipEmptyParts);
    //qDebug() << data;
    setControlsEnabled(true);
    //skips first chunk of data to account for data drop
    if (data.count()>1)
    {
        ui->labelHeight->setText(data[1]);
    }
}
void MainWindow::processError(const QString &s)
{
    //display error message
        setControlsEnabled(true);
        ui->label_conStatus->setText(tr("Status: Not running, %1.").arg(s));
        ui->labelHeight->setText(tr("No traffic."));

}
void MainWindow::updateChart()
{
    //updates chart and adds new data to log file
    chart->handleTimeout(ui->labelHeight->text().toInt());
    QFile data("Data.csv");
    if(data.open(QIODevice::WriteOnly | QIODevice::Append))
    {
        QTextStream output(&data);
        QString time = QTime::currentTime().toString("hh:mm:ss.zzz");
        output << time +";"+ui->labelHeight->text()+";"+" \n" ;
    }
}
void MainWindow::processTimeout(const QString &s)
{
    //displays timeout message
    setControlsEnabled(true);
    ui->label_conStatus->setText(tr("Status: Timeout, %1.").arg(s));
    ui->labelHeight->setText(tr("No traffic."));
}
void MainWindow::setControlsEnabled(bool enable)
{
    //sets interactiveability for blocking and non-blocking code
    ui->pushButton_Connect->setEnabled(enable);
    ui->pushButton_Payload->setEnabled(enable);
    ui->pushButton_CDA->setEnabled(enable);
    ui->comboPortList->setEnabled(enable);
}

void MainWindow::on_pushButton_Payload_clicked()
{
    //handles button event
    thread.release_Payload = true;
    QtCharts::QLineSeries* payloadMark = new QtCharts::QLineSeries();

    payloadMark->append(chart->m_x,0);
    payloadMark->append(chart->m_x,500);
    chart->addSeries(payloadMark);
    payloadMark->attachAxis(chart->axisX());
    payloadMark->attachAxis(chart->axisY());
    QFile data("Data.csv");
    if(data.open(QIODevice::WriteOnly | QIODevice::Append))
    {
        QTextStream output(&data);
        QString time = QTime::currentTime().toString("hh:mm:ss.zzz");
        output << time +";"+ui->labelHeight->text()+";Payload"+" \n" ;
    }
}

void MainWindow::on_pushButton_CDA_clicked()
{
    thread.release_CDA = true;
    QtCharts::QLineSeries* CDAMark = new QtCharts::QLineSeries();

    CDAMark->append(chart->m_x,0);
    CDAMark->append(chart->m_x,500);
    chart->addSeries(CDAMark);
    CDAMark->attachAxis(chart->axisX());
    CDAMark->attachAxis(chart->axisY());
    QFile data("Data.csv");
    if(data.open(QIODevice::WriteOnly | QIODevice::Append))
    {
        QTextStream output(&data);
        QString time = QTime::currentTime().toString("hh:mm:ss.zzz");
        output << time +";"+ui->labelHeight->text()+";CDA"+" \n" ;
    }
}
