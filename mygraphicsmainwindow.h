#ifndef MYGRAPHICSMAINWINDOW_H
#define MYGRAPHICSMAINWINDOW_H

#include <QMainWindow>
#include <QDockWidget>
#include <QLabel>
#include <QString>
#include <QMouseEvent>
#include <QDateTime>
#include <QGraphicsEllipseItem>
#include <QGraphicsItem>
#include "graphicslinechart.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MyGraphicsMainWindow; }
QT_END_NAMESPACE

class GraphicsLineChart;

class MyGraphicsMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MyGraphicsMainWindow(QWidget *parent = nullptr);
    ~MyGraphicsMainWindow();


private:
    Ui::MyGraphicsMainWindow *ui;
    void init();

    QLabel m_statusBar;

    //GraphicsLineChart *lineChar;
    //QGraphicsScene  *scene;
signals:
    void signalsQDateTimeranged(QString currentTime);

public slots:
    void slots_mouseMovePoint(QPointF point);
    void slots_startTimer();

    void slot_viewUpdata();
private slots:
    void on_pushButton_2_clicked();
    void on_pushButton_5_clicked();
};
#endif // MYGRAPHICSMAINWINDOW_H
