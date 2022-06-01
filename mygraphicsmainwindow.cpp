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
    connect(ui->graphicsView,SIGNAL(pageNum(int)),this,SLOT(slots_page(int)));
    //上一页
    connect(this,SIGNAL(upPage()),ui->graphicsView,SLOT(slotsupPage()));
    //下一页
    connect(this,SIGNAL(downPage()),ui->graphicsView,SLOT(slotsDownPage()));
    //当前页
    connect(ui->graphicsView,SIGNAL(thisPageNum(int)),this,SLOT(slots_thisPage(int)));
    //选择页面显示
    connect(this,SIGNAL(thisSetPage(int)),ui->graphicsView,SLOT(slotsSetPage(int)));
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

    qDebug()<<"开始接收";
    ui->graphicsView->start();
}

void MyGraphicsMainWindow::slots_page(int num)
{
    page = num;
    qDebug()<<QString::number(num)<<"---------------";
    ui->label_3->setText(QString::number(num));
}

void MyGraphicsMainWindow::slots_thisPage(int num)
{
    ui->label_5->setText(QString::number(num));
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


void MyGraphicsMainWindow::on_pushButton_3_clicked()
{
    emit upPage();
}


void MyGraphicsMainWindow::on_pushButton_4_clicked()
{
    emit downPage();
}


void MyGraphicsMainWindow::on_comboBox_activated(int index)
{

}


void MyGraphicsMainWindow::on_pushButton_6_clicked()
{
   emit thisSetPage(ui->lineEdit->text().toInt());
}


void MyGraphicsMainWindow::on_lineEdit_editingFinished()
{
    emit thisSetPage(ui->lineEdit->text().toInt());
}

