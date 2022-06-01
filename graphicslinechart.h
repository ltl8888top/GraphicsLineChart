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

struct Tag_BadParse_result;

class GraphicsLineChart : public QGraphicsView
{
    Q_OBJECT
public:
   GraphicsLineChart(QWidget *parent = 0);

   //开启定时器
   void start();

   //停止定时器
   void stop();


protected:

   //chart 大小跟随Graphics变化
    void resizeEvent(QResizeEvent *event);
    //鼠标移动时间
    void mouseMoveEvent(QMouseEvent *event);

signals:
    //鼠标移动
    void mouseMovePoint(QPointF point);
    //跟新信号
    void viewUpdata();
    //可以显示的页数
    void pageNum(int num);

    //当前页数
    void thisPageNum(int num);

public slots:

    //点击线 一直显示坐标在视图
    void keepCallout();

    //鼠标停留线上 显示坐标
    void tooltip(QPointF point, bool state);

    //定时器画线
    void DrawLine();

    //设置折线图显示范围
    void slotsRange(int m_range);

    //以传入的时间 开始显示到折线图
    void slotsDatetime(QString timer);

    //上一页
    void slotsupPage();

    //下一页
    void slotsDownPage();

    //选择页显示
    void slotsSetPage(int m_page);



private:

    int range;                              //折线图显示范围标志0：1分钟 1：15分钟 2：30分钟 3：1小时

    int page;                               //折线图的页数

    int thispage;                           //当前折线图的页数

    int numId;                              //接收到的数据个数ID每次+1

    int timernum;                           //每页显示数据数量

    QTimer *timer;                          //定时器
    QLineSeries *series;                    //线
    QLineSeries *series2;


    QGraphicsSimpleTextItem *m_coordX;      //在Graphics上显示文字(坐标)
    QGraphicsSimpleTextItem *m_coordY;

    QDateTimeAxis *axisX;                    //轴(X轴显示时间)
    QValueAxis *m_axisX;
    QValueAxis *axisY;                       //Y轴数值

    QChart *m_chart;                         //Chart线图
    Callout *m_tooltip;                      //QGraphicsItem(图形项) 提供了文字接口

    QList<Callout *> m_callouts;             //

    QList<Tag_BadParse_result> my_data;     //数据存放List


};

struct Tag_BadParse_result
{
    int id;                 //每次接收的数据id

    QString m_dateTime;     //接收时间

    int avgValue_xlv;       //数据值

    int avgValue_xyv;       //数据值
    //...

    Tag_BadParse_result()
    {
        id = 0;
        m_dateTime = "";
        avgValue_xlv = 0;
    }
};




#endif // GRAPHICSLINECHART_H
