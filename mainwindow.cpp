#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtNetwork/QNetworkInterface>
#include <QFileDialog>
#include <QMessageBox>

#include <QMouseEvent>
#include <QTimer>

#include "utils/imageview.h"
#include "dialog_viewphoto.h"
#include "csv/csvfile.h"
#include "dataanimals.h"
//#include<iostream>

#pragma execution_character_set("utf-8")

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    QPalette pal = this->palette();
    pal.setColor(QPalette::Window, Qt::white);
    this->setPalette(pal);

    // *****************************************

    ui->groupBox->setStyleSheet("QGroupBox {"
                                "background-color: white;"
                                "}"
                                "QGroupBox::title {"
                                "color: white;"
                                "background-color:"+DataSystems::Instance().settings___color_header+";"
                                                                                    "padding: 4 20000 4 10;"
                                                                                    "}");
    ui->groupBox_2->setStyleSheet("QGroupBox {"
                                  "background-color: white;"
                                  "}"
                                  "QGroupBox::title {"
                                  "color: white;"
                                  "background-color:"+DataSystems::Instance().settings___color_header+";"
                                                                                      "padding: 4 20000 4 10;"
                                                                                      "}");

    ui->groupBox_3->setStyleSheet("QGroupBox {"
                                  "background-color: white;"
                                  "}"
                                  "QGroupBox::title {"
                                  "color: white;"
                                  "background-color:"+DataSystems::Instance().settings___color_header+";"
                                                                                      "padding: 4 20000 4 10;"
                                                                                      "}");

    ui->groupBox_4->setStyleSheet("QGroupBox {"
                                  "background-color: white;"
                                  "}"
                                  "QGroupBox::title {"
                                  "color: white;"
                                  "background-color:"+DataSystems::Instance().settings___color_header+";"
                                                                                      "padding: 4 20000 4 10;"
                                                                                      "}");


    DataSystems::Instance().clear();

    // ***************************************

    receivedSocketMsg = NULL;

    // *****************************************


    server = NULL ;
    receivedSocket = NULL;
    newFile = NULL;
    //savePath = "D:/_del_";
    //DataSystems::Instance().savePath = QApplication::applicationDirPath()+"/";
    findNoExits = false;
    localPort = 8000;
    localPortMsg = 7777;
    foreach(const QHostAddress& hostAddress,QNetworkInterface::allAddresses())
    //  Здесь мы определяем, действителен ли IP-адресkdlsk;jfg
    {
        OutputLog(hostAddress.toString());
        if ( hostAddress != QHostAddress::LocalHost && hostAddress.toIPv4Address() )
            localIp = hostAddress.toString();

    }

    ui->label_localIP->setText(localIp);
    ui->lineEdit_localPort->setText(QString::number(localPort));

    GetData();
    //Notepad();

    //QTableWidget * table = new QTableWidget(5, 5);
    ui->tableWidget->setMouseTracking(true);
    //connect(ui->tableWidget, SIGNAL(cellEntered(int,int)),SLOT(on_tableWidget_cellEntered(int,int)));
    connect(ui->tableWidget, SIGNAL(cellEntered(int,int)),SLOT(on_tableWidget_cellEntered2(int,int)));

    //connect(ui->tableWidget, SIGNAL(mouseReleaseEvent(QMouseEvent)),SLOT(mouseReleaseEvent(QMouseEvent)));
    connect(ui->tableWidget, SIGNAL(mouseReleaseEvent(QMouseEvent)),SLOT(mouseReleaseEvent(QMouseEvent)));


    // *****************************************


    OutputLog("IP server: " +localIp+"| port: "+QString::number(localPort));

    on_pushButton_startlisten_clicked();

    update_ImageLoad();
    //ui->textEdit->setText(DataSystems::Instance().log);

    //QTimer::singleShot(100, this, SLOT(ViewPhoto()));
    PhotoView(ui->comboBox->currentText());
}

void MainWindow::PhotoView_Dialog()
{
    //OutputLog(QString::number(row));

    QString fileNamePicture = ui->comboBox->currentText();
    //fileNamePicture = "https://demotivatorium.ru/sstorage/3/2014/09/13001529222202/demotivatorium_ru_a_chto_eto_vi_tut_delaete_a_58272.jpg";
    Dialog_ViewPhoto dvf(fileNamePicture, this);

    if(!(dvf.exec()==QDialog::Accepted))
    {
        //QMessageBox::information(this,"Спасибо","Сухостойность");
    }

}

/*
void MainWindow::on_tableWidget_cellEntered2(int row, int column)
{
    OutputLog(QString::number(row) + "|" + QString::number(column));
}

/**/


void MainWindow::on_tableWidget_cellEntered2(int row, int column)
{
    OutputLog(QString::number(row) + "|" + QString::number(column));
    if(column !=0 && column!=1)return;
    //QTimer::singleShot(100, this, SLOT(PhotoView_Dialog()));

    return;

    OutputLog(QString::number(row));

    if(ui->tableWidget->selectionModel()->isRowSelected(row, QModelIndex()))
        return;
    ui->tableWidget->clearSelection();
    ui->tableWidget->selectRow(row);

    if(column !=0 && column!=1)return;

    QString fileNamePicture = ui->comboBox->currentText();
    Dialog_ViewPhoto dvf(fileNamePicture, this);

    if(!(dvf.exec()==QDialog::Accepted))
    {
        //QMessageBox::information(this,"Спасибо","Сухостойность");
    }

    //ui->tableWidget->item(row,0)->setBackground(Qt::green);
    //ui->tableWidget->item(row,1)->setBackground(Qt::green);
    //ui->tableWidget->item(row,2)->setBackground(Qt::green);
}

/**/

MainWindow::~MainWindow()
{
    delete server;
    delete serverMsg;
    delete newFile;
    delete ui;
}

void MainWindow::acceptConnection()
{
    ui->receivedStatusLabel->setText(tr("Подключился и готовлюсь к приему файлов!"));

    receivedSocket = server->nextPendingConnection();
    connect(receivedSocket, SIGNAL(readyRead()), this, SLOT(readClient()));
}

void MainWindow::acceptConnectionMsg()
{
    receivedSocketMsg = serverMsg->nextPendingConnection();
    connect(receivedSocketMsg, SIGNAL(readyRead()), this, SLOT(readClientMsg()));
}

void MainWindow::readClientMsg()
{
    QString tab = "dataentry_robot";
    QDataStream in(receivedSocketMsg);
    in>>msgR1>>msgR2;

    OutputLog("msg1: " + msgR1+"| msg2: "+msgR2);

    return;

    QScopedPointer<DataBase> db_func(new DataBase());
    if(db_func->StatusOpen())
    {

    QStringList list= msgR.split("|", QString::SkipEmptyParts);
    for(QString val: list)
    {
        QStringList list_two_value= val.split(";", QString::SkipEmptyParts);
        //qDebug()<<list_two_value[0]<<" = "<<list_two_value[1];
        OutputLog(list_two_value[0] + " | " +list_two_value[1]);
        {
            db_func->insert_add(tab, list_two_value[0], list_two_value[1]);
        }
    }

    QDateTime dt1 = QDateTime::currentDateTime();
    //db_func->insert_add(tab, "paramtime1", dt1.toString("yyyy-MM-dd hh:mm:ss.ms"));
    QString sql_insert = db_func->insert_add(tab, "paramtime1", dt1.toString("yyyy-MM-dd hh:mm:ss.zzzZ"));
    //QString sql_insert = db_func->insert_add(tab, "param1", "");
    db_func->sql_exec(sql_insert, "add Robot");

    }
    else
    {
        OutputLog("Нет подключения к базе данных VIM: ");

    }

    //OutputLog("Message from client: " +msgR);
}

void MainWindow::readClient()
{
    ui->receivedStatusLabel->setText(tr("Получение файлов..."));

    if(byteReceived == 0)  //Только начали получать данные, эти данные представляют собой информацию о файле
    {
        ui->receivedProgressBar->setValue(0);
        QDataStream in(receivedSocket);
        in>>totalSize>>byteReceived>>fileName;
        fileName = DataSystems::Instance().savePath+"/" + QString::fromStdString(logger::CreateLogName2())+ "_" + fileName;

        OutputLog(QString("Information: send")+"| totalSize: "+QString::number(totalSize)+"| byteReceived: "+QString::number(byteReceived));

        QFileInfo file(fileName);
        if(file.exists()){
            QMessageBox::critical(this, tr("Пожалуйста, обрати внимание:") ,tr("В текущем пути уже есть файл с таким именем, пожалуйста, выберите новый путь, чтобы сохранить файл!"), QMessageBox::Ok | QMessageBox::Default , QMessageBox::Cancel | QMessageBox::Escape , 0 );
            findNoExits = true;
            //            this->~MainWindow();
            //            exit(0);
            //            return;
        }else{
            newFile = new QFile(fileName);
            newFile->open(QFile::WriteOnly);
            //            fileExits = true;
        }

    }

    else  //Официально прочитать содержимое файла
    {
        //Если файл не создан, будет ли здесь заблокирован основной поток?
        //Определите, является ли оно NULL
        if(newFile == NULL){
            //_sleep(1);
//            QThread::sleep
            return;//Подождите, пока файл будет создан, прежде чем звонить снова.
        }
        inBlock = receivedSocket->readAll();

        byteReceived += inBlock.size();
        newFile->write(inBlock);
        newFile->flush();

        OutputLog(QString("Information: send")+"| inblock: "+QString::number(inBlock.size())+"| byteReceived: "+QString::number(byteReceived));

    }

    //ui->progressLabel->show();
    ui->receivedProgressBar->setMaximum(totalSize);
    ui->receivedProgressBar->setValue(byteReceived);

    if(byteReceived == totalSize)
    {
        ui->receivedStatusLabel->setText(tr("%1 --- Прием завершен").arg(fileName));

        DataSystems::Instance().log = "";
        float s = newFile->size()/1024.0;
        OutputLog(QString("Файл принят: ")+" | fileName: " + fileName+ " ||| " + QString("Размер файла: ")+"  " + QString::number(s)+" Kb"+" = " + QString::number(s/1000.0)+" Mb");
        ui->lineEdit_savepath->setText(fileName);


        QDateTime dt1 = QDateTime::currentDateTime();


        update_ImageLoad();

        PhotoView(fileName);

        inBlock.clear();
        byteReceived = 0;
        totalSize = 0;
        newFile->close();
        newFile = NULL;
        findNoExits = false;

    }
}

void MainWindow::on_pushButton_startlisten_clicked()
{
    totalSize = 0;
    byteReceived = 0;

    server = new QTcpServer(this);
    //server->listen(QHostAddress("172.19.198.43"), 10000);
    server->listen(QHostAddress::Any, localPort);

    connect(server, SIGNAL(newConnection()), this, SLOT(acceptConnection()));

    ui->receivedProgressBar->setValue(0);
    ui->receivedStatusLabel->setText(tr("Начать мониторинг..."));
    ui->pushButton_startlisten->setEnabled(false);


    //************************************

    serverMsg = new QTcpServer(this);
    //server->listen(QHostAddress("172.19.198.43"), 10000);
    serverMsg->listen(QHostAddress::Any, localPortMsg);

    connect(serverMsg, SIGNAL(newConnection()), this, SLOT(acceptConnectionMsg()));

    //************************************

    OutputLog("Начать мониторинг...");
}

void MainWindow::OutputLog(QString msgLog)
{
        logger::WriteLog(msgLog);
        logger::WriteMsg(msgLog.toStdString());
        ui->textEdit->setText(DataSystems::Instance().log);
}

void MainWindow::PhotoView(QString fileNamePicture)
{
        QFile *file = new QFile(fileNamePicture);
        file->open(QFile::ReadOnly);
        if(!file->exists())
        {
           OutputLog("No exist file");
           return;
        }
        QByteArray ba = (file)->readAll();
        OutputLog(fileNamePicture);
        OutputLog(QString("save db size ") + QString::number(ba.size()));
        QDateTime dt1 = QDateTime::currentDateTime();
        //saveFileToDB(ba,ba,"vz",fileNamePicture,"photo",dt1.toString("yyyy-MM-dd hh:mm:ss.ms"));
        //saveFileToDB();
        saveFileToDB(ba, //DataSystems::Instance().robot_paramfile1,
                     ba, //DataSystems::Instance().robot_paramfile2,
                     DataSystems::Instance().robot_robot_no,
                     DataSystems::Instance().robot_coord_teat_lf,
                     DataSystems::Instance().robot_coord_teat_lr,
                     DataSystems::Instance().robot_coord_teat_rf,
                     DataSystems::Instance().robot_coord_teat_rr,
                     DataSystems::Instance().robot_param1,
                     DataSystems::Instance().robot_param2,
                     DataSystems::Instance().robot_param3,
                     DataSystems::Instance().robot_param4,
                     DataSystems::Instance().robot_paramtime1,
                     DataSystems::Instance().robot_paramtime2,
                     DataSystems::Instance().robot_paramtime3,
                     DataSystems::Instance().robot_paramtime4
                     );

        file->close();

    if(fileNamePicture.toLower().contains("http")||
        fileNamePicture.toLower().contains("jpg")||
        fileNamePicture.toLower().contains("gif")||
        fileNamePicture.toLower().contains("png"))
    {

        ImageView *imageView = new ImageView(this);
        imageView->setDisplayStyle(imageView->STYLE::contain);//
        if(!fileNamePicture.contains("http"))imageView->setLoadingPixmap(QPixmap(fileNamePicture));
        else imageView->setUrl(fileNamePicture);



        if(ui->verticalLayout_2->count()<1)
        {
            //form_dataentry_dataentry___transfer *tf = new form_dataentry_dataentry___transfer(this);
            OutputLog(QString("Error"));
            ui->verticalLayout_2->addWidget(imageView);
            return;
        }

        QLayoutItem *child = ui->verticalLayout_2->takeAt(0);
        //while ((child = ui->verticalLayout_2->takeAt(0)) != 0)
        if(!child) {return;}
        QWidget* w = child->widget();
        delete child;
        if(!w) {return;}
        ui->verticalLayout_2->removeWidget(w);
        delete w;

        //form_dataentry_dataentry___transfer *tf = new form_dataentry_dataentry___transfer(this);
        ui->verticalLayout_2->addWidget(imageView);

    }
}

void MainWindow::on_pushButton_clicked()
{
    //savePath = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "D:/_del_/", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    //DataSystems::Instance().savePath = QApplication::applicationDirPath()+"/";
    ui->lineEdit_savepath->setText(DataSystems::Instance().savePath);
    if(findNoExits&&byteReceived!=0)
    {   //Чтобы изменить имя файла!
        fileName = DataSystems::Instance().savePath +"/" + fileName.right(fileName.size() - fileName.lastIndexOf('/')-1);
        QFileInfo file(fileName);
        if(file.exists()){
            QMessageBox::critical(this, tr("Пожалуйста, обрати внимание:") ,tr("В текущем пути уже есть файл с таким именем. Перезапустите компьютер и выберите новый путь, чтобы сохранить файл!"), QMessageBox::Ok | QMessageBox::Default , QMessageBox::Cancel | QMessageBox::Escape , 0 );
            return;
        }
        newFile = new QFile(fileName);
        newFile->open(QFile::WriteOnly);
        readClient();

    }
}

void MainWindow::on_lineEdit_localPort_returnPressed()
{
    localPort = ui->lineEdit_localPort->text().toInt();
}


void MainWindow::tabwidget_insert_add_column(QTableWidget *tab, const QString &nameColumnX,  int numRow, int numColumn)
{
    QString const DateTimeFormat = "yyyy-MM-ddTHH:mm:ss.zzzZ";
    if(nameColumnX.contains("pic")||nameColumnX.contains("param"))
    {
        QString time = QString::fromStdString(logger::time()); //a.toString();

        QTableWidgetItem *item = new QTableWidgetItem(time);
        item->setTextAlignment(Qt::AlignHCenter);
        tab->setItem( numRow, numColumn,  item);

        OutputLog(QString::number(numRow) + "|" + QString::number(numColumn) + "|" + time);

    }
}

void MainWindow::tabwidget_insert_add_column(QSqlQuery *query, QTableWidget *tab, const QString &nameColumnX,  int numRow, int numColumn)
{
    //QString const DateTimeFormat = "yyyy-MM-ddTHH:mm:ss[Z|[+|-]HH:mm]";
    QString const DateTimeFormat = "yyyy-MM-ddTHH:mm:ss.zzzZ";
    //QString const DateTimeFormat = "yyyy-MM-ddTHH:mm:ss.zzzZ";

    if(nameColumnX.contains("timey2"))
    {
        //uint t_ = query->value("timey2").toDouble();
        double t_ = query->value("timey2").toDouble();
        QTime a(0,0,0);
        a = a.addSecs(int(t_));
        QString time = a.toString();

        QTableWidgetItem *item = new QTableWidgetItem(time);
        item->setTextAlignment(Qt::AlignHCenter);
        tab->setItem( numRow, numColumn,  item);

    }
    else if(nameColumnX.contains("timey1"))
    {

        QString t_ = query->value("timey1").toString();
        QDateTime a;
        //a.fromString(t_,"yyyy-MM-ddTHH:mm:ss.zzzZ");
        //a.fromString(t_,"yyyy-MM-dd HH:mm:ss.zzzZ");
        //a.fromString(t_,("yyyy-MM-dd hh:mm:ss.ms"));
        //a.fromString(t_,DateTimeFormat);
        //a.fromString(t_, Qt::ISODate);
        QDateTime time2 = QDateTime::fromString(t_, Qt::ISODateWithMs);
        //fromStringValue(t_,a);
        //logger::WriteMsg("date: "+a.toString().toStdString());
        //a.fromString(t_, Qt::ISODateWithMs);
        QString time = time2.toString("yyyy-MM-dd hh:mm:ss.zzzZ");

        //fromStringValue(t_,a);

        QTableWidgetItem *item = new QTableWidgetItem(time);
        //QTableWidgetItem *item = new QTableWidgetItem(t_);
        item->setTextAlignment(Qt::AlignHCenter);
        tab->setItem( numRow, numColumn,  item);

    }
    else if(nameColumnX.contains("paramtime1"))
    {

        QString t_ = query->value("paramtime1").toString();
        QDateTime a;
        //QDateTime time2 = QDateTime::fromString(t_, Qt::ISODateWithMs).currentDateTimeUtc().toLocalTime();
        QDateTime time2 = QDateTime::fromString(t_, Qt::ISODateWithMs);
        double xi = time2.toTime_t();
        QString time = time2.toString("yyyy-MM-dd hh:mm:ss.zzzZ");
        QTableWidgetItem *item = new QTableWidgetItem(time);
        //QTableWidgetItem *item = new QTableWidgetItem(t_);
        item->setTextAlignment(Qt::AlignHCenter);
        tab->setItem( numRow, numColumn,  item);
    }
    else if(nameColumnX.toLower().contains("paramfile2"))
    {
        //return;
        QByteArray ba = QByteArray::fromBase64(query->value("paramfile2").toByteArray());

        if(ba == nullptr)
        {
            QTableWidgetItem *item = new QTableWidgetItem(QString("[binary data]"));
            item->setTextAlignment(Qt::AlignHCenter);
            tab->setItem( numRow, numColumn,  item);
            return;
        }
        return;
        QPixmap p;
        if(p.loadFromData(ba,"JPG"))
        {
            // do something with pixmap
        }

        //Form_in_work *fiw = new Form_in_work(ba);
        //tab->setCellWidget(numRow, numColumn,  fiw);
    }
    else
    {
        if(nameColumnX.toLower().contains("file"))
        {
            return;
        }

        QTableWidgetItem *item = new QTableWidgetItem(query->value(nameColumnX).toString());
        item->setTextAlignment(Qt::AlignLeft);
        tab->setItem( numRow, numColumn,  item);
    }
    //ui->comboBox->addItem(query->value(nameColumn).toString());
    //xi = query->value(nameColumnX).toDouble();
    //yi = query->value(nameColumnY).toDouble();
}

void MainWindow::saveFileToDB()
{
    DataSystems::Instance().robot_robot_no = QString("Robo")+QString::number((rand()%10000));
    DataSystems::Instance().robot_coord_teat_lf = QString::number(0*((DataSystems::Instance().y<80)?(10+(rand()%5)/7.0):(
                         (DataSystems::Instance().y<120)?(20+(rand()%6)/7.0):(
                                                (DataSystems::Instance().y<200)?(15+(rand()%4)/7.0):(12+(rand()%4)/7.0)))));
    DataSystems::Instance().robot_coord_teat_lr = QString::number(0*((DataSystems::Instance().y<80)?(70+(rand()%8)/7.0):(
                         (DataSystems::Instance().y<120)?(60+(rand()%3)/7.0):(
                                                (DataSystems::Instance().y<200)?(65+(rand()%3)/7.0):(68+(rand()%5)/7.0)))));
    DataSystems::Instance().robot_coord_teat_rf = QString::number((DataSystems::Instance().y<80)?(70+sin(DataSystems::Instance().y)):(
                                                 (DataSystems::Instance().y<120)?(60+sin(DataSystems::Instance().y)):(65+2*sin(DataSystems::Instance().y))));
    DataSystems::Instance().robot_coord_teat_rr = QString::number(40+(rand()%5)/7.0);

    DataSystems::Instance().robot_param1 = QString("param")+QString::number((rand()%10000));
    DataSystems::Instance().robot_param2 = QString("param")+QString::number((rand()%1000));
    DataSystems::Instance().robot_param3 = QString("param")+QString::number((rand()%1000));
    DataSystems::Instance().robot_param4 = QString("param")+QString::number((rand()%10000));

    QDateTime t = QDateTime::currentDateTime();
    //double xi = time2.toTime_t();
    QString time = t.toString("yyyy-MM-dd hh:mm:ss.zzzZ");
    DataSystems::Instance().robot_paramtime1 = time;

    QScopedPointer<DataBase> db_func(new DataBase());
    if(db_func->StatusOpen())
    {
        QString tab = "dataentry_robot";

        /*

        db_func->insert_add(tab, "robot_No", DataSystems::Instance().robot_robot_no);
        db_func->insert_add(tab, "coord_teat_lf", DataSystems::Instance().robot_coord_teat_lf);
        db_func->insert_add(tab, "coord_teat_lr", DataSystems::Instance().robot_coord_teat_lr);
        db_func->insert_add(tab, "coord_teat_rf", DataSystems::Instance().robot_coord_teat_rf);
        db_func->insert_add(tab, "coord_teat_rr", DataSystems::Instance().robot_coord_teat_rr);
        db_func->insert_add(tab, "param1", DataSystems::Instance().robot_param1);
        db_func->insert_add(tab, "param2", DataSystems::Instance().robot_param2);
        db_func->insert_add(tab, "param3", DataSystems::Instance().robot_param3);
        db_func->insert_add(tab, "param4", DataSystems::Instance().robot_param4);
        db_func->insert_add(tab, "paramtime1", DataSystems::Instance().robot_paramtime1);
        db_func->insert_add(tab, "paramtime2", DataSystems::Instance().robot_paramtime2);
        db_func->insert_add(tab, "paramtime3", DataSystems::Instance().robot_paramtime3);
        db_func->insert_add(tab, "paramtime4", DataSystems::Instance().robot_paramtime4);
/**/
        //db_func->insert_add_question(tab, "paramfile1");//DataSystems::Instance().robot_paramfile1);
        QString sql_insert = db_func->insert_add_question(tab, "paramfile1");//DataSystems::Instance().robot_paramfile2);


        logger::WriteMsg(sql_insert.toStdString());

        //"----------" + "')";
        db_func->sql_exec(sql_insert, DataSystems::Instance().robot_paramfile1, DataSystems::Instance().robot_paramfile2, "add fixed feeding");
    }
}

void MainWindow::saveFileToDB(QByteArray var1_ba, QByteArray var2_ba, QString param1, QString param2, QString param3, QString paramtime1)
{
    QScopedPointer<DataBase> db_func(new DataBase());
    if(db_func->StatusOpen())
    {
        //QDateTime dt1 = QDateTime::currentDateTime();
        //db_func->insert_add("dataentry_robot", "paramtime1", dt1.toString("yyyy-MM-dd hh:mm:ss.ms"));
        //QString  sql_insert = db_func->insert_add(tab, "paramtime1", dt1.toString("yyyy-MM-dd hh:mm:ss.zzzZ"));
        //QString sql_insert = db_func->insert_add(tab, "param1", "");
        db_func->sql_prepare_exec(
            "insert into dataentry_robot(paramfile1, paramfile2, param1,param2,param3, paramtime1) values(?,?,?,?,?,?)",
            var1_ba,
            var2_ba,
            param1,
            param2,
            param3,
            paramtime1, "");

    }
    else
    {
        OutputLog("Нет подключения к базе данных VIM: ");
    }

}
/**/

void MainWindow::saveFileToDB(QByteArray var1_ba, QByteArray var2_ba, QString robot_no, QString coord_teat_lf, QString coord_teat_lr, QString coord_teat_rf, QString coord_teat_rr,  QString param1, QString param2, QString param3, QString param4, QString paramtime1, QString paramtime2, QString paramtime3, QString paramtime4)
{
    DataSystems::Instance().robot_robot_no = QString("Robo")+QString::number((rand()%10000));
    DataSystems::Instance().robot_coord_teat_lf = QString::number(0*((DataSystems::Instance().y<80)?(10+(rand()%5)/7.0):(
                                                                           (DataSystems::Instance().y<120)?(20+(rand()%6)/7.0):(
                                                                               (DataSystems::Instance().y<200)?(15+(rand()%4)/7.0):(12+(rand()%4)/7.0)))));
    DataSystems::Instance().robot_coord_teat_lr = QString::number(0*((DataSystems::Instance().y<80)?(70+(rand()%8)/7.0):(
                                                                           (DataSystems::Instance().y<120)?(60+(rand()%3)/7.0):(
                                                                               (DataSystems::Instance().y<200)?(65+(rand()%3)/7.0):(68+(rand()%5)/7.0)))));
    DataSystems::Instance().robot_coord_teat_rf = QString::number((DataSystems::Instance().y<80)?(70+sin(DataSystems::Instance().y)):(
                                                                      (DataSystems::Instance().y<120)?(60+sin(DataSystems::Instance().y)):(65+2*sin(DataSystems::Instance().y))));
    DataSystems::Instance().robot_coord_teat_rr = QString::number(40+(rand()%5)/7.0);

    DataSystems::Instance().robot_param1 = QString("param")+QString::number((rand()%10000));
    DataSystems::Instance().robot_param2 = QString("param")+QString::number((rand()%1000));
    DataSystems::Instance().robot_param3 = QString("param")+QString::number((rand()%1000));
    DataSystems::Instance().robot_param4 = QString("param")+QString::number((rand()%10000));

    QDateTime t = QDateTime::currentDateTime();
    //double xi = time2.toTime_t();
    QString time = t.toString("yyyy-MM-dd hh:mm:ss.zzzZ");
    DataSystems::Instance().robot_paramtime1 = time;

    QScopedPointer<DataBase> db_func(new DataBase());
    if(db_func->StatusOpen())
    {
        //QDateTime dt1 = QDateTime::currentDateTime();
        //db_func->insert_add("dataentry_robot", "paramtime1", dt1.toString("yyyy-MM-dd hh:mm:ss.ms"));
        //QString  sql_insert = db_func->insert_add(tab, "paramtime1", dt1.toString("yyyy-MM-dd hh:mm:ss.zzzZ"));
        //QString sql_insert = db_func->insert_add(tab, "param1", "");
        db_func->sql_prepare_exec(
            "insert into dataentry_robot(paramfile1, paramfile2, robot_No, coord_teat_lf,coord_teat_lr,coord_teat_rf,coord_teat_rr, param1,param2,param3,param4, paramtime1, paramtime2, paramtime3, paramtime4) values(?, ?,? ,?,?,?,?, ?,?,?,?, ?,?,?,?)",
            var1_ba,
            var2_ba,
            robot_no,
            coord_teat_lf,
            coord_teat_lr,
            coord_teat_rf,
            coord_teat_rr,
            param1,
            param2,
            param3,
            param4,
            paramtime1,
            paramtime2,
            paramtime3,
            paramtime4,
            "");

    }
    else
    {
        OutputLog("Нет подключения к базе данных VIM: ");
    }

}

/*
void MainWindow::saveFileToDB(QString var1, QByteArray var1_ba)
{
    QScopedPointer<DataBase> db_func(new DataBase());
    if(db_func->StatusOpen())
    {
        QDateTime dt1 = QDateTime::currentDateTime();
        //db_func->insert_add("dataentry_robot", "paramtime1", dt1.toString("yyyy-MM-dd hh:mm:ss.ms"));
        //QString sql_insert = db_func->insert_add(tab, "paramtime1", dt1.toString("yyyy-MM-dd hh:mm:ss.zzzZ"));
        //QString sql_insert = db_func->insert_add(tab, "param1", "");
        db_func->sql_prepare_exec("insert into dataentry_robot(paramtime1, paramfile1) values(?,?)", var1, var1_ba, "");

    }
    else
    {
        OutputLog("Нет подключения к базе данных VIM: ");

    }

}
/**/

void MainWindow::update_ImageLoad()
{
    QString folder = DataSystems::Instance().savePath + "/";
    //folder = "d:/img2/";

    ui->comboBox->clear();
    ui->comboBox->addItem("https://masterpiecer-images.s3.yandex.net/e67320c3a29e11eea1b38a24f5e205f7:upscaled");
    ui->comboBox->addItem("https://naked-science.ru/wp-content/uploads/2022/11/1-1-1.jpg");
    ui->comboBox->addItem("https://masterpiecer-images.s3.yandex.net/ffc0d777a29c11ee988efac5ed247044:upscaled");
    ui->comboBox->addItem("https://spb.belochka-umelochka.ru/wa-data/public/shop/products/82/92/49282/images/135224/135224.750x0.jpg");
    ui->comboBox->addItem("https://img.freepik.com/premium-photo/dazzling-starry-sky-wallpaper-universe-starry-sky-backgroundgenerative-ai_710973-6709.jpg");
    ui->comboBox->addItem("https://masterpiecer-images.s3.yandex.net/6b1b14002fc711ee82886e855efad8a9:upscaled");
    ui->comboBox->addItem("https://cdn.vseinstrumenti.ru/images/goods/tovary-dlya-ofisa-i-doma/tovary-dlya-doma/9334718/1200x800/126777146.jpg");
    ui->comboBox->addItem("https://storage.yandexcloud.net/storage.yasno.media/nat-geo/images/2019/8/30/7c1fd9b106be46d6a3e283cfc654e836.max-2500x1500.jpg");
    ui->comboBox->addItem("https://img.lovepik.com/background/20211022/medium/lovepik-starry-sky-background-image_401734583.jpg");

    std::vector<std::string> vec = csvfile::ReadFiles(folder.toStdString().c_str());
    for(int k=0; k<vec.size();k++)
    {
        if(QString::fromStdString(vec[k]).toLower().contains("jpg")||
            QString::fromStdString(vec[k]).toLower().contains("gif")||
            QString::fromStdString(vec[k]).toLower().contains("png"))
        {
            //logger::WriteLog(folder + QString::fromStdString(vec[k]));
            ui->comboBox->addItem(folder + QString::fromStdString(vec[k]));
        }
    }
}

void MainWindow::GetData()
{
    //ui->tableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("Параметры робота"));
    for(int i=0;i<ui->tableWidget->columnCount();i++)ui->tableWidget->setColumnWidth(i, 250);

    int i=0;
    while(i<3)
    {
        ui->tableWidget->insertRow( ui->tableWidget->rowCount() );
        //QString time = QString::fromStdString(logger::time()); //a.toString();
        //QTableWidgetItem *item = new QTableWidgetItem(time);
        //item->setTextAlignment(Qt::AlignHCenter);
        //ui->tableWidget->setItem( i, 2,  new QTableWidgetItem(time));

        tabwidget_insert_add_column(ui->tableWidget,"pic1", i, 0);
        tabwidget_insert_add_column(ui->tableWidget,"pic2", i, 1);
        tabwidget_insert_add_column(ui->tableWidget,"param1", i, 2);
        tabwidget_insert_add_column(ui->tableWidget,"param2", i, 3);        
        i++;
    }
    ui->tableWidget->update();
}


// *************************************************************************************************

void MainWindow::Notepad()
{
    //ui->tableWidget = new QTableWidget();
    test();

    add_action = new QAction(tr("Add cell"), this);
    add_action->setIcon(QIcon("add.jpg"));
    Delete_action = new QAction(tr("Delete cell"), this);
    Delete_action->setIcon(QIcon("delete.jpg"));

    connect(Delete_action, SIGNAL(triggered()), this, SLOT(Delete()));
    connect(add_action, SIGNAL(triggered()), this, SLOT(add()));

    //tableItem->setFlags(tableItem->flags() ^ Qt::ItemIsEditable);

    //centralWidget()->setAttribute(Qt::WA_TransparentForMouseEvents);
    //centralWidget()->setAttribute(Qt::WA_MouseTracking,true);

    setMouseTracking(true);

}
void MainWindow::test()
{

    //QTableWidgetItem* tableItem = new QTableWidgetItem();

    ui->tableWidget->setRowCount(1);
    ui->tableWidget->setColumnCount(3);
    ui->tableWidget->setItem(0,0,new QTableWidgetItem());
    ui->tableWidget->setItem(0,1,new QTableWidgetItem());
    ui->tableWidget->setItem(0,2,new QTableWidgetItem());

    ui->tableWidget->setMouseTracking(true);
    ui->tableWidget->viewport()->setMouseTracking(true);
    ui->tableWidget->installEventFilter(this);
    ui->tableWidget->viewport()->installEventFilter(this);

    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    //table->setSelectionMode( QAbstractItemView::ExtendedSelection );
    //table->setAttribute(Qt::WA_TransparentForMouseEvents);



    ui->tableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("combobox"));
    ui->tableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem("spinbox"));
    ui->tableWidget->setHorizontalHeaderItem(2, new QTableWidgetItem("lineEdit"));
    //ui->tableWidget->setFlags(ui->tableWidget->flags() ^ Qt::ItemIsEditable);
    ui->tableWidget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    //setCentralWidget(ui->tableWidget);

}

void MainWindow::mouseReleaseEvent (QMouseEvent * event )
{
    QMessageBox* msgBox;
    if(event->button() == Qt::RightButton)
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*> (event);
        QMenu *menu = new QMenu(this);
        menu->addAction(add_action);
        menu->addAction(Delete_action);
        menu->exec(mouseEvent->globalPos());
        //msgBox->setInformativeText("u pressed right button");
    }
}

void MainWindow::add()
{

    ui->tableWidget->insertRow( 1);

    //setCentralWidget(ui->tableWidget);
    //centralWidget()->setAttribute(Qt::WA_TransparentForMouseEvents);
    //setMouseTracking(true);
}

void MainWindow::Delete()
{

    ui->tableWidget->removeRow(1);

    //setCentralWidget(ui->tableWidget);
    //centralWidget()->setAttribute(Qt::WA_TransparentForMouseEvents);
    //setMouseTracking(true);
}


void MainWindow::on_comboBox_activated(int index)
{
    QString fileNamePicture = ui->comboBox->currentText();

    //send(new QFile(fileNamePicture));
    //send2(fileNamePicture);

    //QString fileNamePicture = ui->comboBox->currentText();

    //PhotoView_Dialog();

    PhotoView(fileNamePicture);

}

