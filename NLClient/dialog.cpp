#include "dialog.h"
#include "ui_dialog.h"
#include "qnlclient.h"
#include <QGridLayout>
#include <QFormLayout>
#include <QBoxLayout>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    m_isConnection = false;

    m_NLClient.Init(0,0,100,100,Qt::cyan,Qt::green);

    ui->gridLayout->addWidget(&m_NLClient);
    ui->gridLayout->invalidate();
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_Dialog_finished(int result)
{
    m_NLClient.DisconnectToServer();
}

void Dialog::on_ButtonClose_clicked()
{
    close();
}

void Dialog::on_ButtonConnect_clicked()
{
    m_NLClient.ConnectToServer();
}
