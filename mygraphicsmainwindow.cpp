#include "mygraphicsmainwindow.h"
#include "ui_mygraphicsmainwindow.h"
#include <QPoint>

class GraphicsLineChart;
MyGraphicsMainWindow::MyGraphicsMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MyGraphicsMainWindow)
{
    ui->setupUi(this);
//    QRectF rect = ui->graphicsView->rect();
//    scene=new QGraphicsScene(rect); //创建QGraphicsScene
//    ui->graphicsView->setScene(scene);
    connect(ui->graphicsView,SIGNAL(mouseMovePoint(QPointF)),this,SLOT(slots_mouseMovePoint(QPointF)));
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(slots_startTimer()));
    connect(ui->graphicsView,SIGNAL(viewUpdata()),this,SLOT(slot_viewUpdata()));
    connect(ui->comboBox,SIGNAL(activated(int)),ui->graphicsView,SLOT(slotsRange(int)));
    connect(this,SIGNAL(signalsQDateTimeranged(QString)),ui->graphicsView,SLOT(slotsDatetime(QString)));


    init();
}

MyGraphicsMainWindow::~MyGraphicsMainWindow()
{
    delete ui;
}

void MyGraphicsMainWindow::init()
{
    m_statusBar.setText("X:0.0  Y:0.0");
    ui->statusbar->addWidget(&m_statusBar);
    ui->dockWidget->setToolTip(tr("操作"));

    ui->dockWidget->setFeatures(QDockWidget::DockWidgetMovable);//样式
    ui->dockWidget->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);//左侧和右侧
    ui->dateTimeEdit->setCalendarPopup(true);
    ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());
    //ui->timeEdit->setCalendarPopup(true);

}

void MyGraphicsMainWindow::slots_mouseMovePoint(QPointF point)
{
    QString strPoint = QString("view  X:%1  Y:%2").arg(point.x()).arg(point.y());
    m_statusBar.setText(strPoint);



}

void MyGraphicsMainWindow::slots_startTimer()
{

    //添加一个椭圆
//        QGraphicsEllipseItem   *item=new QGraphicsEllipseItem(-50,-30,100,60);
//        item->setFlags(QGraphicsItem::ItemIsMovable
//                       | QGraphicsItem::ItemIsSelectable
//                       | QGraphicsItem::ItemIsFocusable);
//        item->setBrush(QBrush(Qt::blue)); //填充颜色
//        item->setZValue(1); //用于叠放顺序
//        item->setPos(-50+(rand() % 100),-50+(rand() % 100)); //初始位置
//        item->setData(1,0);  //自定义数据，ItemId键
//        item->setData(2,"椭圆"); //自定义数据，ItemDesciption键

//        scene->addItem(item);
//        scene->clearSelection();
//        item->setSelected(true);
    //lineChar = new GraphicsLineChart(ui->graphicsView);
    //lineChar->start();
    qDebug()<<"开始接收";
    ui->graphicsView->start();
}

void MyGraphicsMainWindow::slot_viewUpdata()
{
    qDebug()<<"viewUpdata";
    ui->graphicsView->update();
    //scene();

}


void MyGraphicsMainWindow::on_pushButton_2_clicked()
{
    ui->graphicsView->stop();
}


void MyGraphicsMainWindow::on_pushButton_5_clicked()
{
    emit signalsQDateTimeranged(ui->dateTimeEdit->dateTime().toString("yyyy/M/d H:mm:ss"));
    qDebug()<<ui->dateTimeEdit->dateTime().toString("yyyy/M/d H:mm:ss");
}

