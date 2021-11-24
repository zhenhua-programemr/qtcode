#include "subwidget.h"
#include "ui_subwidget.h"
#include <QMessageBox>
#include <QDataStream>
#include <iostream>

using namespace std;
subWidget::subWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::subWidget)
{
    ui->setupUi(this);
    serial_is_open = false;
    this->setWindowTitle(QString::fromLocal8Bit("Test子窗口"));

    PortConfigureInit();
    connect(&serial,&QSerialPort::readyRead,this,&subWidget::Serial_read);

    //ui->bandRateBox->setCurrentIndex(0);
}

subWidget::~subWidget()
{
    delete ui;
}

void subWidget::PortConfigureInit()
{
    //填入串口选项
    //ui->comBox->addItem("com1","com1");

    ui->bandRateBox->addItem("9600","9600");

    ui->dataBitBox->addItem("8",8);

    ui->checkoutBox->addItem(QString::fromLocal8Bit("无校验"),0);

    ui->stopBitBox->addItem(QString::fromLocal8Bit("1位"),1);
}
void subWidget::on_open_Button_clicked()
{
    //QMessageBox::warning(NULL,QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("串口打开成功"));
    if (ui->open_Button->text()==QString::fromLocal8Bit("打开串口"))
    {
        //设置端口号
        serial.setPortName(ui->comBox->currentText());
        //设置波特率
        serial.setBaudRate(ui->bandRateBox->currentText().toInt());
        //设置数据位
        switch(ui->dataBitBox->currentText().toInt())
        {
            case 8:serial.setDataBits(QSerialPort::Data8);break;
            case 7:serial.setDataBits(QSerialPort::Data7);break;
            case 6:serial.setDataBits(QSerialPort::Data6);break;
            case 5:serial.setDataBits(QSerialPort::Data5);break;
            default:break;
        }
        //设置停止位
        switch(ui->stopBitBox->currentText().toInt())
        {
            case 1:serial.setStopBits(QSerialPort::OneStop);break;
            case 2:serial.setStopBits(QSerialPort::TwoStop);break;
            default:break;
        }
        //设置校验方式
        switch(ui->checkoutBox->currentIndex())
        {
            case 0:serial.setParity(QSerialPort::NoParity);
            default:break;
        }
        //设置流控制模式
        serial.setFlowControl(QSerialPort::NoFlowControl);
        //打开串口
        if(serial.open(QIODevice::ReadWrite)==false)
        {
            QMessageBox::warning(NULL,QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("串口打开失败"));
            return;
        }
        ui->comBox->setEnabled(false);
        ui->checkoutBox->setEnabled(false);
        ui->bandRateBox->setEnabled(false);
        ui->dataBitBox->setEnabled(false);
        ui->stopBitBox->setEnabled(false);
        ui->search_Button->setEnabled(false);
        //调整串口控制按钮文字提示
        ui->open_Button->setText(QString::fromLocal8Bit("关闭串口"));
        //ui->open_Button->setChecked(false);
        serial_is_open = true;

    }
    else
    {
        serial.close();
        ui->comBox->setEnabled(true);
        ui->checkoutBox->setEnabled(true);
        ui->bandRateBox->setEnabled(true);
        ui->dataBitBox->setEnabled(true);
        ui->stopBitBox->setEnabled(true);
        ui->search_Button->setEnabled(true);
        //调整串口控制按钮文字提示
        ui->open_Button->setText(QString::fromLocal8Bit("打开串口"));
        //serial_is_open = false;
    }

}

void subWidget::on_send_Button_clicked()
{
    if(serial.isOpen()==false)
    {
        QMessageBox::warning(NULL,QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("请打开串口"));
        return;
    }

    //toPlainText() 转换为纯文本格式
    //toUtf8() 转换为UTF-8 编码

    QByteArray senddata = ui->sendEdit->toPlainText().toUtf8();
    if(ui->send16Box->isChecked()==true)
    {
        int cnt = senddata.size();
        char *data = senddata.data();
        for(int i=0;i<cnt;i++)
        {
            if(data[i]>='0'&&(data[i]<='9'))
            {
                continue;
            }
            else if(data[i]>='a'&&(data[i]<='f'))
                continue;
            else if(data[i]>='A'&&(data[i]<='F'))
                continue;
            else if(data[i]==' ')
                continue;
            else
            {
                QMessageBox::warning(NULL,QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("输入非16进制字符"));
                return;
            }
        }
        senddata = senddata.fromHex(senddata);
    }

    if(ui->newlineBox->isChecked()==true)
    {
        int cnt = senddata.size();
        senddata = senddata.insert(cnt,"\r\n");
    }
    serial.write(senddata);
    QMessageBox::warning(NULL,QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("发送成功"));
}

void subWidget::on_search_Button_clicked()
{

    ui->comBox->clear();

    foreach(const QSerialPortInfo &info,QSerialPortInfo::availablePorts())
    //foreach的句法是：foreach(variable,container)
    {
        ui->comBox->addItem(info.portName());
    }
}

void subWidget::Serial_read()
{
    QByteArray buffer = serial.readAll();

    if(ui->show16Box->isChecked()==true)
    {
        buffer = buffer.toHex();
    }
    QString recieve = QString(buffer);

    ui->recieveEdit->setText(recieve);
}

void subWidget::on_clear_r_Button_clicked()
{
    ui->recieveEdit->clear();
}



