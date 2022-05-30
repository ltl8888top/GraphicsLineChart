#ifndef GRAPHICSLINECHART_H
#define GRAPHICSLINECHART_H


#include <QtWidgets/QGraphicsView>
#include <QtCharts/QChartGlobal>
#include <QLineSeries>
#include <QDateTimeAxis>
#include <QValueAxis>
#include <QPoint>
#include <QObject>
#include <QDateTime>
#include <QTimer>

QT_BEGIN_NAMESPACE
class QGraphicsScene;
class QMouseEvent;
class QResizeEvent;
QT_END_NAMESPACE

QT_BEGIN_NAMESPACE
class QChart;
QT_END_NAMESPACE

class Callout;

QT_USE_NAMESPACE

class GraphicsLineChart : public QGraphicsView
{
    Q_OBJECT
public:
   GraphicsLineChart(QWidget *parent = 0);

   void start();//开启定时器

   void stop();//停止定时器


protected:
    void resizeEvent(QResizeEvent *event);
    void mouseMoveEvent(QMouseEvent *event);



signals:
    void mouseMovePoint(QPointF point); //鼠标移动

    void viewUpdata();

public slots:
    void keepCallout();
    void tooltip(QPointF point, bool state);

    void DrawLine();                     //定时器画线

    void slotsRange(int m_range);

    void slotsDatetime(QString timer);



private:

    int range;                              //折线图显示范围

    QTimer *timer;                           //计时器
    QLineSeries *series;
    //QLineSeries *series2;


    QGraphicsSimpleTextItem *m_coordX;
    QGraphicsSimpleTextItem *m_coordY;

    QDateTimeAxis *axisX;                    //轴
    QValueAxis *m_axisX;
    QValueAxis *axisY;

    QChart *m_chart;
    Callout *m_tooltip;

    QList<Callout *> m_callouts;

    QMap<QString,int> my_data;




};

#endif // GRAPHICSLINECHART_H
