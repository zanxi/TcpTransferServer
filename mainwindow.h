#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QFile>
#include <QString>

#include <QTableWidget>

#include "Debug/logger.h"
#include "dataanimals.h"



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void PhotoView(QString fileNamePicture);
    void OutputLog(QString msgLog);


private slots:
    void on_tableWidget_cellEntered2(int row, int column);
    //void on_tableWidget_cellEntered(int row, int column);
    void mouseReleaseEvent (QMouseEvent * event );
    void acceptConnection();
    void acceptConnectionMsg();
    void readClient();
    void readClientMsg();
    void on_pushButton_startlisten_clicked();

    void on_pushButton_clicked();
    void PhotoView_Dialog();
    void on_lineEdit_localPort_returnPressed();

    void add();
    void Delete();

    void on_comboBox_activated(int index);

private:
    Ui::MainWindow *ui;

    QTcpServer *server;    
    QTcpSocket *receivedSocket;
    QFile *newFile;

    QByteArray inBlock;
    QString fileName;
    qint64 totalSize;  //Общий размер отправляемого файла (содержимое файла и информация об имени файла)
    qint64 byteReceived;  //размер отправлен

    QString localIp;
    //QString savePath;
    bool findNoExits;
    int localPort;

    QTcpServer *serverMsg;
    QTcpSocket *receivedSocketMsg;
    int localPortMsg;
    QString msgR, msgR1, msgR2;


    QAction *add_action;
    QAction *Delete_action;

    void Notepad();
    void test();
    void GetData();
    void tabwidget_insert_add_column(QSqlQuery *query, QTableWidget *tab, const QString &nameColumnX,  int numRow, int numColumn);
    void tabwidget_insert_add_column(QTableWidget *tab, const QString &nameColumnX,  int numRow, int numColumn);

    void update_ImageLoad();

    //void saveFileToDB(QByteArray var1_ba, QByteArray var2_ba, QString param1, QString paramtime1);
    void saveFileToDB(QByteArray var1_ba, QByteArray var2_ba,  QString robot_no, QString coord_teat_lf, QString coord_teat_lr, QString coord_teat_rf, QString coord_teat_rr,  QString param1, QString param2, QString param3, QString param4, QString paramtime1, QString paramtime2, QString paramtime3, QString paramtime4);
    void saveFileToDB(QByteArray var1_ba, QByteArray var2_ba, QString param1, QString param2, QString param3, QString paramtime1);
    void saveFileToDB();
    //   void saveFileToDB(QString vart1, QByteArray var1_ba);


};
#endif // MAINWINDOW_H
