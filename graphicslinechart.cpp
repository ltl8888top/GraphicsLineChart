#include "graphicslinechart.h"
#include <QtGui/QResizeEvent>
#include <QtWidgets/QGraphicsScene>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QSplineSeries>
#include <QtWidgets/QGraphicsTextItem>
#include "callout.h"
#include <QtGui/QMouseEvent>
#include "mygraphicsmainwindow.h"


GraphicsLineChart::GraphicsLineChart(QWidget *parent)
    : QGraphicsView(new QGraphicsScene, parent),
      m_coordX(0),
      m_coordY(0),
      m_chart(0),
      m_tooltip(0)

{
    range=0;
    //connect(MyGraphicsMainWindow,SIGNAL(range(int)),this,)
    setDragMode(QGraphicsView::NoDrag);//鼠标样式 抓手 矩形选框
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//滚动条
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QPen penY(Qt::darkBlue,3,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin);

    //time
    timer = new QTimer(this);


    // chart
    m_chart = new QChart;
    m_chart->setMinimumSize(640, 440);
    m_chart->setTitle("测试");
    m_chart->legend()->hide();

    //轴
    axisX = new QDateTimeAxis();
    //axisX = new QValueAxis();
    axisY = new QValueAxis();
    axisX->setTickCount(10);                             //设置坐标轴格数
    axisY->setTickCount(5);
    axisX->setFormat("hh:mm:ss");                           //设置时间显示格式
    //axisX->setLabelFormat("hhmmss");
    axisY->setMin(0);                                    //设置Y轴范围
    axisY->setMax(10);
    //axisX->setTitleText("实时时间");                       //设置X轴名称
    axisY->setLinePenColor(QColor(Qt::darkBlue));        //设置坐标轴颜色样式
    axisY->setGridLineColor(QColor(Qt::darkBlue));
    axisY->setGridLineVisible(false);                    //设置Y轴网格不显示
    axisY->setLinePen(penY);
    axisX->setLinePen(penY);

    series = new QLineSeries;
//    series->append(1, 3);
//    series->append(4, 5);
//    series->append(5, 4);
//    series->append(7, 1);
//    series->append(11, 2);

    m_chart->addSeries(series);

//    series2 = new QSplineSeries;
//    series2->append(1.6, 1.4);
//    series2->append(2.4, 3.5);
//    series2->append(3.7, 2.5);
//    series2->append(7, 4);
//    series2->append(10, 2);
//    m_chart->addSeries(series2);

//    m_chart->addAxis(axisX,Qt::AlignBottom);               //设置坐标轴位于chart中的位置
//    m_chart->addAxis(axisY,Qt::AlignLeft);

    series->attachAxis(axisX);                           //把数据添加到坐标轴上
    series->attachAxis(axisY);



    //m_chart->createDefaultAxes();//创建轴
    m_chart->setAcceptHoverEvents(true);//接受悬停事件

    setRenderHint(QPainter::Antialiasing);//为QPainter指定任何给定引擎 启用渲染提示
    scene()->addItem(m_chart);//scene()返回指向视图中当前显示的场景的指针。

    m_coordX = new QGraphicsSimpleTextItem(m_chart);//设置项的文本
    QPen pen(QColor(125,123,0));
    m_coordX->setPen(pen);
    m_coordX->setPos(m_chart->size().width()/2 - 50, m_chart->size().height());
    m_coordX->setText("X: ");
    m_coordY = new QGraphicsSimpleTextItem(m_chart);
    m_coordY->setPos(m_chart->size().width()/2 + 50, m_chart->size().height());
    m_coordY->setText("Y: ");

//    connect(series, SIGNAL(clicked), this, SLOT(keepCallout));
//    connect(series, SIGNAL(hovered), this, SLOT(tooltip()));

    connect(series, &QSplineSeries::clicked, this, &GraphicsLineChart::keepCallout);
    connect(series, &QSplineSeries::hovered, this, &GraphicsLineChart::tooltip);

//    connect(series2, &QSplineSeries::clicked, this, &GraphicsLineChart::keepCallout);
//    connect(series2, &QSplineSeries::hovered, this, &GraphicsLineChart::tooltip);

    connect(timer,SIGNAL(timeout()),this,SLOT(DrawLine()));

    this->setMouseTracking(true);
}

void GraphicsLineChart::resizeEvent(QResizeEvent *event)
{
    if (scene()) {
        scene()->setSceneRect(QRect(QPoint(0, 0), event->size()));
         m_chart->resize(event->size());
         m_coordX->setPos(m_chart->size().width()/2 - 50, m_chart->size().height() - 20);
         m_coordY->setPos(m_chart->size().width()/2 + 50, m_chart->size().height() - 20);
         const auto callouts = m_callouts;
         for (Callout *callout : callouts)
             callout->updateGeometry();
    }
    QGraphicsView::resizeEvent(event);
}

void GraphicsLineChart::mouseMoveEvent(QMouseEvent *event)
{
    QPointF Point;

//    qDebug()<<"rx:  "<<m_chart->mapToValue(event->pos()).rx();
//    qDebug()<<"x:  "<<m_chart->mapToValue(event->pos()).x();
    m_coordX->setText(QString("X: %1").arg(m_chart->mapToValue(event->pos()).rx()));//坐标显示 类似状态栏
    m_coordY->setText(QString("Y: %1").arg(m_chart->mapToValue(event->pos()).y()));
    Point.setX(m_chart->mapToValue(event->pos()).rx());
    Point.setY(m_chart->mapToValue(event->pos()).y());
    emit mouseMovePoint(Point);
    QGraphicsView::mouseMoveEvent(event);//鼠标悬浮形成方框显示坐标
}

void GraphicsLineChart::keepCallout()
{
    m_callouts.append(m_tooltip);
    m_tooltip = new Callout(m_chart);
}

void GraphicsLineChart::tooltip(QPointF point, bool state)
{
    if (m_tooltip == 0)
        m_tooltip = new Callout(m_chart);

    if (state) {
        m_tooltip->setText(QString("X: %1 \nY: %2 ").arg(point.x()).arg(point.y()));
        m_tooltip->setAnchor(point);
        m_tooltip->setZValue(11);
        m_tooltip->updateGeometry();
        m_tooltip->show();
    } else {
        m_tooltip->hide();
    }
}

void GraphicsLineChart::DrawLine()
{
//    static bool flag;
    qDebug()<<"定时器画线";
    int number;
    QDateTime currentTime = QDateTime::currentDateTime();
    //设置坐标轴显示范围

    switch (range) {
    case 0:
        m_chart->axisX()->setMin(QDateTime::currentDateTime().addSecs(-60 * 1));       //系统当前时间的前一秒
        m_chart->axisX()->setMax(QDateTime::currentDateTime().addSecs(0));             //系统当前时间
        break;
    case 1:
        m_chart->axisX()->setMin(QDateTime::currentDateTime().addSecs(-60 * 15));       //系统当前时间的前一秒
        m_chart->axisX()->setMax(QDateTime::currentDateTime().addSecs(0));
        break;
    case 2:
        m_chart->axisX()->setMin(QDateTime::currentDateTime().addSecs(-60 * 30));       //系统当前时间的前一秒
        m_chart->axisX()->setMax(QDateTime::currentDateTime().addSecs(0));
        break;
    case 3:
        m_chart->axisX()->setMin(QDateTime::currentDateTime().addSecs(-60 * 60));       //系统当前时间的前一秒
        m_chart->axisX()->setMax(QDateTime::currentDateTime().addSecs(0));
        break;
    default:
        break;
    }



    srand(QTime(0,0,0).secsTo(QTime::currentTime()));                          //这里生成随机数做测试
    number = rand() % 9;
    //增加新的点到曲线末端
    series->append(currentTime.toMSecsSinceEpoch(), number);
    //series->append(currentTime.toString("hhmmss").toInt(), number);

    //qDebug()<<".toMSecsSinceEpoch()"<<currentTime.toMSecsSinceEpoch();
    my_data.insert(currentTime.toString("yyyy/M/d H:mm:ss"),number);

}

void GraphicsLineChart::slotsRange(int m_range)
{
    qDebug()<<"范围选择 :"<<m_range;
    range = m_range;
}

void GraphicsLineChart::slotsDatetime(QString timer)
{
    //清空graphilcsview
    series->clear();
    update();
    m_axisX = new QValueAxis();
    m_axisX->setMin(0);                                    //设置Y轴范围
    m_axisX->setMax(60);
    //series->attachAxis(m_axisY);



    m_chart->createDefaultAxes();//创建轴

    qDebug()<<"查找数据从"<<timer<<"开始";

    int timernum = 0;
    int num=0;

    QDateTime temp = QDateTime::fromString(timer,"yyyy/M/d H:mm:ss");
    switch (range) {
    case 0:
        timernum = 60;
//        m_chart->axisX()->setMin(temp.addSecs(0));       //系统当前时间的前一秒
//        m_chart->axisX()->setMax(temp.addSecs(-60 * 1));
        break;
    case 1:
        timernum = 60*15;
//        m_chart->axisX()->setMin(temp.addSecs(-60 * 15));       //系统当前时间的前一秒
//        m_chart->axisX()->setMax(temp.addSecs(0));
        break;
    case 2:
        timernum = 60*30;
//        m_chart->axisX()->setMin(temp.addSecs(-60 * 30));       //系统当前时间的前一秒
//        m_chart->axisX()->setMax(temp.addSecs(0));
        break;
    case 3:
        timernum = 60*60;
//        m_chart->axisX()->setMin(temp.addSecs(-60 * 60));       //系统当前时间的前一秒
//        m_chart->axisX()->setMax(temp.addSecs(0));
        break;
    default:
        break;
    }
    //QMap容器查找。。。timer
    QMap<QString,int> ::iterator i;
    i = my_data.find(timer);
    if(i == my_data.end())
    {
        qDebug()<<"查找错误";
    }

    for(; i!=my_data.end(); i++)
    {
        num++;
        temp = QDateTime::fromString(i.key(),"yyyy/M/d H:mm:ss");
        series->append(temp.toMSecsSinceEpoch(), i.value());

        if(num>timernum){
         emit viewUpdata();
         return;
        }

    }
    emit viewUpdata();
}

void GraphicsLineChart::start()
{
    qDebug()<<"开启定时器---------------";

    timer->start(1000);
}

void GraphicsLineChart::stop()
{
    timer->stop();
}

