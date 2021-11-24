#ifndef SUBWIDGET_H
#define SUBWIDGET_H

#include <QWidget>

#include <QtSerialPort/QSerialPort>//提供访问串口的功能
#include <QtSerialPort/QSerialPortInfo>//提供访问串口的功能

QT_BEGIN_NAMESPACE
namespace Ui { class subWidget; }
QT_END_NAMESPACE

class subWidget : public QWidget
{
    Q_OBJECT

public:
    subWidget(QWidget *parent = nullptr);
    ~subWidget();

private:
    Ui::subWidget *ui;
    bool serial_is_open;

public:
    QSerialPort serial;
private slots:
    void on_open_Button_clicked();
    void on_send_Button_clicked();
    void on_search_Button_clicked();
    void Serial_read();
    void on_clear_r_Button_clicked();
    void PortConfigureInit();
    //void on_pushButton_clicked();
};
#endif // SUBWIDGET_H
