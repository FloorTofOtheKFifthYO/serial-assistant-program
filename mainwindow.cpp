#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <Qdebug>
#include <QPushButton>
#include <QKeyEvent>
/**
 ***********************************************
 * @brief pro文件也记得修改，加上serialport和3drender
 ***********************************************
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->pushButton_send->setEnabled(false);
    ui->lineEdit_send->setReadOnly(true);
    ui->textEdit_2->setReadOnly(true);
    fresh_coms();
    load();

    ui->go_forward->setStyleSheet("border:2px groove gray;border-radius:10px;padding:2px 4px;");
    ui->go_back->setStyleSheet("border:2px groove gray;border-radius:10px;padding:2px 4px;");
    ui->turn_left->setStyleSheet("border:2px groove gray;border-radius:10px;padding:2px 4px;");
    ui->turn_right->setStyleSheet("border:2px groove gray;border-radius:10px;padding:2px 4px;");
    ui->stop->setStyleSheet("border:2px groove gray;border-radius:10px;padding:2px 4px;");
    ui->up->setStyleSheet("border:2px groove gray;border-radius:10px;padding:2px 4px;");
    ui->down->setStyleSheet("border:2px groove gray;border-radius:10px;padding:2px 4px;");
    //以下设置按钮被选中未选中以及鼠标放置时颜色
    ui->go_forward->setStyleSheet("QPushButton{background-color:black;color: white;   border-radius: 10px;  border: 2px groove gray;border-style: outset;}"
    "QPushButton:hover{background-color:white; color: black;}"
    "QPushButton:pressed{background-color:rgb(85, 170, 255);border-style: inset; }");
    ui->go_back->setStyleSheet("QPushButton{background-color:black;color: white;   border-radius: 10px;  border: 2px groove gray;border-style: outset;}"
    "QPushButton:hover{background-color:white; color: black;}"
    "QPushButton:pressed{background-color:rgb(85, 170, 255);border-style: inset; }");
    ui->turn_left->setStyleSheet("QPushButton{background-color:black;color: white;   border-radius: 10px;  border: 2px groove gray;border-style: outset;}"
    "QPushButton:hover{background-color:white; color: black;}"
    "QPushButton:pressed{background-color:rgb(85, 170, 255);border-style: inset; }");
    ui->turn_right->setStyleSheet("QPushButton{background-color:black;color: white;   border-radius: 10px;  border: 2px groove gray;border-style: outset;}"
    "QPushButton:hover{background-color:white; color: black;}"
    "QPushButton:pressed{background-color:rgb(85, 170, 255);border-style: inset; }");
    ui->stop->setStyleSheet("QPushButton{background-color:black;color: white;   border-radius: 10px;  border: 2px groove gray;border-style: outset;}"
    "QPushButton:hover{background-color:white; color: black;}"
    "QPushButton:pressed{background-color:rgb(85, 170, 255);border-style: inset; }");
    ui->up->setStyleSheet("QPushButton{background-color:black;color: white;   border-radius: 10px;  border: 2px groove gray;border-style: outset;}"
    "QPushButton:hover{background-color:white; color: black;}"
    "QPushButton:pressed{background-color:rgb(85, 170, 255);border-style: inset; }");
    ui->down->setStyleSheet("QPushButton{background-color:black;color: white;   border-radius: 10px;  border: 2px groove gray;border-style: outset;}"
    "QPushButton:hover{background-color:white; color: black;}"
    "QPushButton:pressed{background-color:rgb(85, 170, 255);border-style: inset; }");
}

MainWindow::~MainWindow()
{
    delete ui;
}

/**
 *************************************************
 *@brief  打开串口事件
 *@note   移植可以直接全选复制，记得改控件名称成你的名字
 *************************************************
 */
void MainWindow::on_pushButton_open_clicked()
{
    if(ui->pushButton_open->text()=="打开串口(space)"){
        SecondarySerial=new QSerialPort(this);
        SecondarySerial->setPortName(ui->comboBox_2->currentText());
        SecondarySerial->setBaudRate(ui->comboBox_baudrate_2->currentText().toInt());
        SecondarySerial->setDataBits(QSerialPort::Data8);
        //设置校验位
        SecondarySerial->setParity(QSerialPort::NoParity);
        //设置流控制
        SecondarySerial->setFlowControl(QSerialPort::NoFlowControl);
        //设置停止位
        SecondarySerial->setStopBits(QSerialPort::OneStop);
        SecondarySerial->setReadBufferSize(200000);
        if(SecondarySerial->isOpen())
        {
            QMessageBox msgBox;
            msgBox.setText("打开成功");
            msgBox.exec();
        }
        if(SecondarySerial->open(QIODevice::ReadWrite)){
            ui->pushButton_send->setEnabled(true);
            ui->lineEdit_send->setReadOnly(false);
            ui->pushButton_open->setText("关闭串口");
            ui->comboBox_2->setDisabled(true);
            ui->comboBox_baudrate_2->setDisabled(true);
            QObject::connect(SecondarySerial, &QSerialPort::readyRead, this, &MainWindow::SecondarySerialRecvMsgEvent);
        }
        else
        {
           QMessageBox::about(NULL,"提示","打开串口失败！");
           return ;
        }

       // start_update();
    }else{
        SecondarySerial->close();
        ui->pushButton_open->setText("打开串口(space)");
        ui->pushButton_send->setEnabled(false);
        ui->lineEdit_send->setReadOnly(true);
        ui->comboBox_2->setDisabled(false);
        ui->comboBox_baudrate_2->setDisabled(false);
       // stop_update();
    }
}
/**
 *************************************************
 *@brief  发送按键事件
 *@note   移植可以直接全选复制
 *************************************************
 */
void MainWindow::on_pushButton_send_clicked()
{
    qDebug()<<"send!";
    if(ui->pushButton_open->text()!="关闭串口"){
        QMessageBox msgBox;
          msgBox.setText("请先打开串口!");
          msgBox.exec();
                return ;
    }
    QByteArray s=ui->lineEdit_send->text().toUtf8();
    //qDebug()<<ui->lineEdit_send->text().unicode();
    //QByteArray s=ui->lineEdit_send->text().toLatin1();
    if(ui->checkBox_send_newline->isChecked()){
        s.append('\r');
        s.append('\n');
    }
    SecondarySerial->write(s);
}

void MainWindow::on_pushButton_clear_clicked()
{
    if(ui->pushButton_clear->text()=="清空缓冲区(alt)"){
        ui->textEdit_2->clear();
    }
}
/**
 *************************************************
 *@brief  接收字符
 *@note   移植可以直接全选复制
 *************************************************
 */
QByteArray rx_buffer2;//串口二接受字符存储
void MainWindow::SecondarySerialRecvMsgEvent()
{
    //QByteArray str = QString("你好").toUtf8();


    QByteArray temp2;
    //QScrollBar *scrollbar;
    temp2=SecondarySerial->readAll();
    //qDebug()<<temp2;
    rx_buffer2.append(temp2);
   //rx_cnt+=temp.length();
   // QString str;
    QString text = ui->textEdit_2->toPlainText();
    if(text.length() > 1000)
    {
        text = text.mid(200);
    }
    ui->textEdit_2->setText(text+QString(temp2));
    QScrollBar *scrollbar = ui->textEdit_2->verticalScrollBar();
    scrollbar->setSliderPosition(scrollbar->maximum());
    //ui->textEdit_2->append(temp2);
}
void MainWindow::keyPressEvent(QKeyEvent *event)
{
if(event->key()==Qt::Key_A)
    {
        on_turn_left_clicked();
    }
if(event->key()==Qt::Key_S)
    {
        on_stop_clicked();
    }
if(event->key()==Qt::Key_D)
    {
        on_turn_right_clicked();
    }
if(event->key()==Qt::Key_W)
    {
        on_go_forward_clicked();
    }
if(event->key()==Qt::Key_X)
    {
        on_go_back_clicked();
    }
if(event->key()==Qt::Key_Up)
    {
        on_up_clicked();
    }
if(event->key()==Qt::Key_Down)
    {
        on_down_clicked();
    }
if(event->key()==Qt::Key_Enter || event->key()==Qt::Key_Return)
    {
        on_pushButton_send_clicked();
    }
if(event->key()==Qt::Key_Space)
    {
       on_pushButton_open_clicked();
    }
if(event->key()==Qt::Key_Alt)
    {
       on_pushButton_clear_clicked();
    }
if(event->key()==Qt::Key_1)
    {
       on_sendButton_1_clicked();
    }
if(event->key()==Qt::Key_2)
    {
       on_sendButton_2_clicked();
    }
if(event->key()==Qt::Key_3)
    {
       on_sendButton_3_clicked();
    }
if(event->key()==Qt::Key_4)
    {
       on_sendButton_4_clicked();
    }
if(event->key()==Qt::Key_5)
    {
       on_sendButton_5_clicked();
    }
if(event->key()==Qt::Key_6)
    {
       on_sendButton_6_clicked();
    }
if(event->key()==Qt::Key_7)
    {
       on_sendButton_7_clicked();
    }
}
/**
 *************************************************
 *@brief  按钮事件前进
 *@note   移植可以直接全选复制，发送字母w
 *************************************************
 */
void MainWindow::on_go_forward_clicked()
{
    qDebug()<<"send w!";
    if(ui->pushButton_open->text()!="关闭串口"){
        return ;
    }
    ui->lineEdit_send->setText("w");
    QByteArray s=ui->lineEdit_send->text().toLatin1();
    if(ui->checkBox_send_newline->isChecked()){
        s.append('\r');
        s.append('\n');
    }
    SecondarySerial->write(s);
}
/**
 *************************************************
 *@brief  按钮事件左转
 *@note   移植可以直接全选复制，发送字母a
 *************************************************
 */
void MainWindow::on_turn_left_clicked()
{
    qDebug()<<"send a!";
    if(ui->pushButton_open->text()!="关闭串口"){
        return ;
    }
    ui->lineEdit_send->setText("a");
    QByteArray s=ui->lineEdit_send->text().toLatin1();
    if(ui->checkBox_send_newline->isChecked()){
        s.append('\r');
        s.append('\n');
    }
    SecondarySerial->write(s);
}
/**
 *************************************************
 *@brief  按钮事件停止
 *@note   移植可以直接全选复制，发送字母s
 *************************************************
 */
void MainWindow::on_stop_clicked()
{
    qDebug()<<"send s!";
    if(ui->pushButton_open->text()!="关闭串口"){
        return ;
    }
    ui->lineEdit_send->setText("s");
    QByteArray s=ui->lineEdit_send->text().toLatin1();
    if(ui->checkBox_send_newline->isChecked()){
        s.append('\r');
        s.append('\n');
    }
    SecondarySerial->write(s);
}
/**
 *************************************************
 *@brief  按钮事件右转
 *@note   移植可以直接全选复制，发送字母d
 *************************************************
 */
void MainWindow::on_turn_right_clicked()
{
    qDebug()<<"send d!";
    if(ui->pushButton_open->text()!="关闭串口"){
        return ;
    }
    ui->lineEdit_send->setText("d");
    QByteArray s=ui->lineEdit_send->text().toLatin1();
    if(ui->checkBox_send_newline->isChecked()){
        s.append('\r');
        s.append('\n');
    }
    SecondarySerial->write(s);
}
/**
 *************************************************
 *@brief  按钮事件后退
 *@note   移植可以直接全选复制，发送字母x
 *************************************************
 */
void MainWindow::on_go_back_clicked()
{
    qDebug()<<"send x!";
    if(ui->pushButton_open->text()!="关闭串口"){
        return ;
    }
    ui->lineEdit_send->setText("x");
    QByteArray s=ui->lineEdit_send->text().toLatin1();
    if(ui->checkBox_send_newline->isChecked()){
        s.append('\r');
        s.append('\n');
    }
    SecondarySerial->write(s);
}
/**
 *************************************************
 *@brief  按钮事件上浮
 *@note   移植可以直接全选复制，发送字母q
 *************************************************
 */
void MainWindow::on_up_clicked()
{
    qDebug()<<"send q!";
    if(ui->pushButton_open->text()!="关闭串口"){
        return ;
    }
    ui->lineEdit_send->setText("q");
    QByteArray s=ui->lineEdit_send->text().toLatin1();
    if(ui->checkBox_send_newline->isChecked()){
        s.append('\r');
        s.append('\n');
    }
    SecondarySerial->write(s);
}

/**
 *************************************************
 *@brief  按钮事件下沉
 *@note   移植可以直接全选复制，发送字母e
 *************************************************
 */
void MainWindow::on_down_clicked()
{
    qDebug()<<"send e!";
    if(ui->pushButton_open->text()!="关闭串口"){
        return ;
    }
    ui->lineEdit_send->setText("e");
    QByteArray s=ui->lineEdit_send->text().toLatin1();
    if(ui->checkBox_send_newline->isChecked()){
        s.append('\r');
        s.append('\n');
    }
    SecondarySerial->write(s);
}

void MainWindow::on_sendButton_1_clicked()
{
    qDebug()<<"send!";
    if(ui->pushButton_open->text()!="关闭串口"){
        return ;
    }
    QByteArray s=ui->sendBox_1->text().toLatin1();
    if(ui->checkBox_send_newline->isChecked()){
        s.append('\r');
        s.append('\n');
    }
    SecondarySerial->write(s);
}
void MainWindow::on_sendButton_2_clicked()
{
    qDebug()<<"send!";
    if(ui->pushButton_open->text()!="关闭串口"){
        return ;
    }
    QByteArray s=ui->sendBox_2->text().toLatin1();
    if(ui->checkBox_send_newline->isChecked()){
        s.append('\r');
        s.append('\n');
    }
    SecondarySerial->write(s);
}
void MainWindow::on_sendButton_3_clicked()
{
    qDebug()<<"send!";
    if(ui->pushButton_open->text()!="关闭串口"){
        return ;
    }
    QByteArray s=ui->sendBox_3->text().toLatin1();
    if(ui->checkBox_send_newline->isChecked()){
        s.append('\r');
        s.append('\n');
    }
    SecondarySerial->write(s);
}
void MainWindow::on_sendButton_4_clicked()
{
    qDebug()<<"send!";
    if(ui->pushButton_open->text()!="关闭串口"){
        return ;
    }
    QByteArray s=ui->sendBox_4->text().toLatin1();
    if(ui->checkBox_send_newline->isChecked()){
        s.append('\r');
        s.append('\n');
    }
    SecondarySerial->write(s);
}
void MainWindow::on_sendButton_5_clicked()
{
    qDebug()<<"send!";
    if(ui->pushButton_open->text()!="关闭串口"){
        return ;
    }
    QByteArray s=ui->sendBox_5->text().toLatin1();
    if(ui->checkBox_send_newline->isChecked()){
        s.append('\r');
        s.append('\n');
    }
    SecondarySerial->write(s);
}
void MainWindow::on_sendButton_6_clicked()
{
    qDebug()<<"send!";
    if(ui->pushButton_open->text()!="关闭串口"){
        return ;
    }
    QByteArray s=ui->sendBox_6->text().toLatin1();
    if(ui->checkBox_send_newline->isChecked()){
        s.append('\r');
        s.append('\n');
    }
    SecondarySerial->write(s);
}
void MainWindow::on_sendButton_7_clicked()
{
    qDebug()<<"send!";
    if(ui->pushButton_open->text()!="关闭串口"){
        return ;
    }
    QByteArray s=ui->sendBox_7->text().toLatin1();
    if(ui->checkBox_send_newline->isChecked()){
        s.append('\r');
        s.append('\n');
    }
    SecondarySerial->write(s);
}

void MainWindow::fresh_coms(){
    ui->comboBox_2->clear();
    foreach(const QSerialPortInfo &info,QSerialPortInfo::availablePorts()){
        ui->comboBox_2->addItem(info.portName());
    }
}

void MainWindow::on_pushButton_clicked()
{
    fresh_coms();
}

void MainWindow::save_fast(){
    QSettings * setting=new QSettings("set.ini",QSettings::IniFormat);
    setting->beginGroup("Fast");
    setting->setValue("edit"+QString::number(1),ui->sendBox_1->text());
    setting->setValue("edit"+QString::number(2),ui->sendBox_2->text());
    setting->setValue("edit"+QString::number(3),ui->sendBox_3->text());
    setting->setValue("edit"+QString::number(4),ui->sendBox_4->text());
    setting->setValue("edit"+QString::number(5),ui->sendBox_5->text());
    setting->setValue("edit"+QString::number(6),ui->sendBox_6->text());
    setting->setValue("edit"+QString::number(7),ui->sendBox_7->text());
    setting->endGroup();
}

void MainWindow::load(){
    QSettings * setting=new QSettings("set.ini",QSettings::IniFormat);
    setting->beginGroup("Fast");
    ui->sendBox_1->setText(setting->value("edit"+QString::number(1),"").toString());
    ui->sendBox_2->setText(setting->value("edit"+QString::number(2),"").toString());
    ui->sendBox_3->setText(setting->value("edit"+QString::number(3),"").toString());
    ui->sendBox_4->setText(setting->value("edit"+QString::number(4),"").toString());
    ui->sendBox_5->setText(setting->value("edit"+QString::number(5),"").toString());
    ui->sendBox_6->setText(setting->value("edit"+QString::number(6),"").toString());
    ui->sendBox_7->setText(setting->value("edit"+QString::number(7),"").toString());
    setting->endGroup();
}

void MainWindow::on_comboBox_2_activated(const QString &arg1)
{

}

void MainWindow::on_saveButton_clicked()
{
    save_fast();
}
