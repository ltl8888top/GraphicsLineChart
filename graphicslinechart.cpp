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

struct Tag_BadParse_result tempdata;

GraphicsLineChart::GraphicsLineChart(QWidget *parent)
    : QGraphicsView(new QGraphicsScene, parent),
      m_coordX(0),
      m_coordY(0),
      m_chart(0),
      m_tooltip(0)
{
    range = 0;
    numId = 0;
    thispage = 1;
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
    axisY->setMin(0);                                    //设置Y轴范围
    axisY->setMax(10);
    //axisX->setTitleText("实时时间");                       //设置X轴名称
    axisY->setLinePenColor(QColor(Qt::darkBlue));        //设置坐标轴颜色样式
    axisY->setGridLineColor(QColor(Qt::darkBlue));
    axisY->setGridLineVisible(false);                    //设置Y轴网格不显示
    axisY->setLinePen(penY);
    axisX->setLinePen(penY);

    series = new QLineSeries;
    series->setName("心率");
//    series->setPointLabelsFormat("心率");

    m_chart->addSeries(series);

    series2 = new QLineSeries;
    series2->setName("血氧");
//    series2->setPointLabelsFormat("血氧");
    m_chart->addSeries(series2);

    m_chart->legend()->setVisible(true);
        //对齐
    m_chart->legend()->setAlignment(Qt::AlignTop);

    m_chart->addAxis(axisX,Qt::AlignBottom);               //设置坐标轴位于chart中的位置
    m_chart->addAxis(axisY,Qt::AlignLeft);

    series->attachAxis(axisX);                           //把数据添加到坐标轴上
    series->attachAxis(axisY);

    series2->attachAxis(axisX);                           //把数据添加到坐标轴上
    series2->attachAxis(axisY);


    //m_chart->createDefaultAxes();//创建轴
    m_chart->setAcceptHoverEvents(true);//接受悬停事件
    //m_chart.point

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

    connect(series2, &QSplineSeries::clicked, this, &GraphicsLineChart::keepCallout);
    connect(series2, &QSplineSeries::hovered, this, &GraphicsLineChart::tooltip);

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
    //m_coordX->setText(QString("X: %1").arg(m_chart->mapToValue(event->pos()).rx()));//坐标显示 类似状态栏(event->pos()).rx()
    m_coordX->setText(QString("X: %1").arg((event->pos()).rx()));//坐标显示
    m_coordY->setText(QString("Y: %1").arg(m_chart->mapToValue(event->pos()).y()));
    //Point.setX(m_chart->mapToValue(event->pos()).rx());
    Point.setX((event->pos()).rx());
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
    numId++;
//    static bool flag;
    qDebug()<<"定时器画线";
    int number,number2;
    thispage=1;
    QDateTime currentTime = QDateTime::currentDateTime();
    //设置坐标轴显示范围

    switch (range) {
    case 0:
        timernum = 60;
        m_chart->axisX()->setMin(QDateTime::currentDateTime().addSecs(-60 * 1));       //系统当前时间的前一秒
        m_chart->axisX()->setMax(QDateTime::currentDateTime().addSecs(0));             //系统当前时间
        break;
    case 1:
        timernum = 60*15;
        m_chart->axisX()->setMin(QDateTime::currentDateTime().addSecs(-60 * 15));       //系统当前时间的前一秒
        m_chart->axisX()->setMax(QDateTime::currentDateTime().addSecs(0));
        break;
    case 2:
        timernum = 60*30;
        m_chart->axisX()->setMin(QDateTime::currentDateTime().addSecs(-60 * 30));       //系统当前时间的前一秒
        m_chart->axisX()->setMax(QDateTime::currentDateTime().addSecs(0));
        break;
    case 3:
        timernum = 60*60;
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
    tempdata.avgValue_xlv = number;


    srand(QTime(0,0,0).secsTo(QTime::currentTime().addSecs(1)));                          //这里生成随机数做测试
    number2 = rand() % 9;
    series2->append(currentTime.toMSecsSinceEpoch(), number2);
    tempdata.avgValue_xyv = number2;


    tempdata.id = numId;
    tempdata.m_dateTime = currentTime.toString("yyyy/M/d H:mm:ss");


    my_data.push_back(tempdata);

    page = my_data.size()/timernum+1;
    emit pageNum(page);
    thispage = page;
    emit thisPageNum(thispage);
}

void GraphicsLineChart::slotsRange(int m_range)
{
    qDebug()<<"范围选择 :"<<m_range;
    range = m_range;
}

void GraphicsLineChart::slotsDatetime(QString timer)
{
    int num=0;
    //清空graphilcsview
    series->clear();
    series2->clear();


//    update();
    QList<Tag_BadParse_result> tdata;

    qDebug()<<"查找数据从"<<timer<<"开始";

    QDateTime temp = QDateTime::fromString(timer,"yyyy/M/d H:mm:ss");

    switch (range) {
    case 0:
        timernum = 60;
        break;
    case 1:
        timernum = 60*15;
        break;
    case 2:
        timernum = 60*30;
        break;
    case 3:
        timernum = 60*60;
        break;
    default:
        break;
    }


    ////QList容器查找。。。timer
    bool flag = false;
    QList<Tag_BadParse_result>::iterator it;
    for(it=my_data.begin();it!=my_data.end();it++)
    {
        if(it->m_dateTime == timer)
            flag = true;
        if(flag == true)
        {
            num++;
            tdata.push_back(*it);
            if(num>timernum) break;                        //找到一页数据后跳出循环
        }
    }

    if(tdata.count() == 0)
    {
        qDebug()<<"此时间没有数据";
        return;
    }

    //将数据写入视图
    for(int i = 0; i<tdata.size(); i++)
    {
        temp = QDateTime::fromString(tdata.at(i).m_dateTime,"yyyy/M/d H:mm:ss");
        switch (range) {
        case 0:
            timernum = 60;
            m_chart->axisX()->setMin(temp.addSecs(-60 * 1));
            m_chart->axisX()->setMax(temp.addSecs(0));
            break;
        case 1:
            timernum = 60*15;
            m_chart->axisX()->setMin(temp.addSecs(-60 * 15));
            m_chart->axisX()->setMax(temp.addSecs(0));
            break;
        case 2:
            timernum = 60*30;
            m_chart->axisX()->setMin(temp.addSecs(-60 * 30));
            m_chart->axisX()->setMax(temp.addSecs(0));
            break;
        case 3:
            timernum = 60*60;
            m_chart->axisX()->setMin(temp.addSecs(-60 * 60));
            m_chart->axisX()->setMax(temp.addSecs(0));
            break;
        default:
            break;
        }
        series->append(temp.toMSecsSinceEpoch(), tdata.at(i).avgValue_xlv);
        series2->append(temp.toMSecsSinceEpoch(), tdata.at(i).avgValue_xyv);
    }
}

void GraphicsLineChart::slotsupPage()
{
    timer->stop();
    if(page<=1 | thispage>=page)
    {
        qDebug()<<"已是第一页";
        return;
    }

    if(numId <= thispage*timernum+timernum)//上一页不够一页
    {
        thispage++;
        if(thispage>=page)thispage=page;
        slotsDatetime(my_data.at(my_data.count()-my_data.count()%timernum).m_dateTime);
    }else
    {
        slotsDatetime(my_data.at(thispage*timernum).m_dateTime);
        thispage++;
        if(thispage>=page)thispage=page;
    }
    emit thisPageNum(thispage);
}

void GraphicsLineChart::slotsDownPage()
{
    timer->stop();
    if(page<=1 | thispage==1)
    {
        qDebug()<<"已是最后一页";
        return;
    }

    if(numId <= thispage*timernum-timernum)//上一页不够一页
    {
        thispage--;
        if(thispage<=1)thispage=1;
        slotsDatetime(my_data.at(0).m_dateTime);
    }else
    {
        thispage--;
        slotsDatetime(my_data.at(thispage*timernum-timernum).m_dateTime);
        if(thispage<=1)thispage=1;
    }

    emit thisPageNum(thispage);
}

void GraphicsLineChart::slotsSetPage(int m_page)
{
    int num ;
    num= m_page-1;
    timer->stop();
    if(num<0 | num>page)
    {
        qDebug()<<"没有此页";
        return;
    }

    if(numId <= num*timernum+timernum)//上一页不够一页
    {
        thispage = m_page;
//        if(thispage>=page)thispage=page;
        slotsDatetime(my_data.at(my_data.count()-my_data.count()%timernum).m_dateTime);
    }
    else
    {
        slotsDatetime(my_data.at(num*timernum).m_dateTime);
        thispage = m_page;
//        if(thispage>=page)thispage=page;
    }
    emit thisPageNum(thispage);
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

