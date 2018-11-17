#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QKeyEvent>
#include <QScrollBar>
#include <QSettings>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

/**
***********************************************
*@brief 以下声明槽函数
***********************************************
*/
private slots:
    void on_pushButton_open_clicked();
    void on_pushButton_send_clicked();
    void on_pushButton_clear_clicked();
    void SecondarySerialRecvMsgEvent();

    void on_go_forward_clicked();

    void on_turn_left_clicked();

    void on_stop_clicked();

    void on_turn_right_clicked();

    void on_go_back_clicked();

    void on_up_clicked();

    void on_down_clicked();

    void keyPressEvent(QKeyEvent *event);
    void on_sendButton_1_clicked();
    void on_sendButton_2_clicked();
    void on_sendButton_3_clicked();
    void on_sendButton_4_clicked();
    void on_sendButton_5_clicked();
    void on_sendButton_6_clicked();
    void on_sendButton_7_clicked();

    void on_comboBox_2_activated(const QString &arg1);

    void on_pushButton_clicked();

    void fresh_coms();

    void save_fast();
    void load();
    void on_saveButton_clicked();

private:
    Ui::MainWindow *ui;
        QSerialPort *SecondarySerial;             //记得声明你的串口名字！！！！


};
