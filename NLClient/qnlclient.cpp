#include "qnlclient.h"
#include <QtMath>
#include <math.h>

QNLClient::QNLClient(QWidget *parent) : QWidget(parent)
{
    m_X = 0;
    m_Y = 0;

    m_Width = 100;
    m_Height = 200;

    m_LState = COMMAND_OFF;
    m_LColor = Qt::white;

    m_TcpSocket = new QTcpSocket();

    m_isTCPConnection = false;

    connect(m_TcpSocket, SIGNAL(connected()), this, SLOT(onConnected()));
    connect(m_TcpSocket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
    connect(m_TcpSocket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
}

void QNLClient::Init( int x, int y, int w, int h, QColor lantern_color, QColor conn_color)
{    
    m_X = x;
    m_Y = y;

    m_Width = w;
    m_Height = h;

    m_LColor = lantern_color;

    m_ConnColor = conn_color;
}

void QNLClient::ConnectToServer()
{
    ConnectToServer(m_IpAdress,m_Port);
}

void QNLClient::ConnectToServer(QString ip_adress,int port)
{
    QMessageBox msg;

    m_IpAdress = ip_adress;
    m_Port = port;

    m_TcpSocket->connectToHost(m_IpAdress,m_Port);

    int istate = m_TcpSocket->state();

    QString state;
    state.setNum(istate);

    if(!m_TcpSocket->waitForConnected(10000))
    {
        if (m_TcpSocket->state() != QAbstractSocket::ConnectedState )
        {
            msg.setText("Соединение не установлено!");
            msg.exec();
        }
    }
    else

    if (m_TcpSocket->state() == QAbstractSocket::ConnectedState)
    {
        //msg.setText("Соединение с сервером установлено!");
       // msg.exec();
    }
    else
    {
        msg.setText("Соединение не установлено!");
        msg.exec();
    }

}

void QNLClient::paintEvent(QPaintEvent *event)
{
    QRect bodyRect;
    QPoint bodyCenter;
    int LRad;
    QPoint connectionIndicatorCenter;
    int connectionIndicatorRad;

    QPainter painter(this); //новый объект "рисовальщика"
    //создать и установить перо - красная сплошная линия
    painter.setPen (QPen(Qt::black,Qt::SolidLine));

    bodyRect.setRect(m_X,m_Y,m_Width,m_Height);

    painter.setBrush(QBrush(Qt::lightGray,Qt::SolidPattern));
    painter.drawRect(bodyRect);

    LRad = qMin(m_Width/2,m_Height/2);
    LRad = LRad-0.1*LRad;

    bodyCenter.setX(m_X + m_Width/2);
    bodyCenter.setY(m_Y + m_Height/2);

    //Устанавливаем заливку для светового элемента фонаря
    if(m_LState == COMMAND_ON)
    {
       painter.setBrush(QBrush(m_LColor,Qt::SolidPattern));

    }
    else
    {
        painter.setBrush(QBrush(Qt::gray,Qt::SolidPattern));
    }

    painter.drawEllipse(bodyCenter,LRad,LRad);

    int diag = qSqrt((m_Width/2)*(m_Width/2) + (m_Height/2)*(m_Height/2));
    connectionIndicatorRad = (diag - LRad)/4;
    connectionIndicatorCenter.setX(m_X + connectionIndicatorRad*2);
    connectionIndicatorCenter.setY(m_Y + connectionIndicatorRad*2);

    //Устанавливаем заливку для элемента, отображающего состояние соединения с сервером
    if(m_isTCPConnection)
    {
       painter.setBrush(QBrush(m_ConnColor,Qt::SolidPattern));

    }
    else
    {
        painter.setBrush(QBrush(Qt::gray,Qt::SolidPattern));
    }

    painter.drawEllipse(connectionIndicatorCenter,connectionIndicatorRad,connectionIndicatorRad);
}


void QNLClient::onConnected()
{
    m_isTCPConnection = true;

    update();
}

void QNLClient::onDisconnected()
{
    m_isTCPConnection = false;

    update();
}

void QNLClient::onReadyRead()
{
    int i;
    QMessageBox msg;
    QDataStream in(m_TcpSocket);
    quint16 type_command;
    quint16 length = 0;
    quint32 *value;
    QColor color;

    if(m_TcpSocket->bytesAvailable() < 2*sizeof(quint16))
        return;

    in >> type_command;
    in >> length;

    if (m_TcpSocket->bytesAvailable() < length)
        return;

    if(type_command == COMMAND_COLOR && length/4 >= 1)
    {
        value = new quint32(length/4);

        for(i=0;i<=length/4-1;i++)
        {
            in >> value[i];
        }

        if(length)
        {
            color.setRgb(value[0]);
            m_LColor = color.rgb();
        }
    }

    if(type_command == COMMAND_ON || type_command == COMMAND_OFF)
        m_LState = type_command;

    update();
}

void QNLClient::DisconnectToServer()
{
    m_TcpSocket->disconnectFromHost();
}
