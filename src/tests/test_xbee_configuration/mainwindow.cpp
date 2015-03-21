#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "xbeeparamitem.h"
#include "xbeeparamstablemodel.h"
#include "atcommand.h"

#include <QMessageBox>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QtArmax-1.0/datamodels/listmodel.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    xbee(NULL)
{
    xbee = new XBee(this);
    //paramsModel = new XBeeParamsTableModel(new XBeeParamItem, this);
    paramsModel = new XBeeParamsTableModel(xbee, this);

    //paramsModel->appendRow(new XBeeParamItem("PanID", QVariant(), paramsModel));

    ui->setupUi(this);
    QHeaderView * header = new QHeaderView(Qt::Horizontal, ui->tableView);
    header->setModel(paramsModel);
    ui->tableView->setHorizontalHeader(header);
    ui->tableView->setModel(paramsModel);
    connect(ui->openSerialButton, SIGNAL(clicked()), this, SLOT(openSelectedSerialPort()));

    foreach(QSerialPortInfo port, QSerialPortInfo::availablePorts())
    {
        ui->serialPortSelection->addItem(port.portName(), port.systemLocation());
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openSelectedSerialPort()
{
    if(!xbee->setSerialPort(ui->serialPortSelection->currentData().toString())) {
        QMessageBox::information(
            this,
            "Error",
            QString("Failed to open serial port \"%1\"").arg(ui->serialPortSelection->currentData().toString()));
    }
    ATCommand atapi;
    atapi.setCommand(ATCommand::Command_AP);
    atapi.setParameter("1");
    //xbee->send(&atapi);
    xbee->loadAddressingProperties();

    atapi.setCommand(ATCommand::Command_ID);
    xbee->send(&atapi);
}
