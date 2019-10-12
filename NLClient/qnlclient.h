#ifndef QNLCLIENT_H
#define QNLCLIENT_H

#include <QObject>
#include <QWidget>
#include <QPainter>
#include<QMessageBox>
#include<QTcpSocket>

#define COMMAND_ON 0x12
#define COMMAND_OFF 0x13
#define COMMAND_COLOR 0x20

class QNLClient : public QWidget
{
    Q_OBJECT
public:
    explicit QNLClient(QWidget *parent = 0);

    QString m_IpAdress;
    int m_Port;

    int m_X;
    int m_Y;
    int m_Width;
    int m_Height;

    quint16 m_LState;
    QColor m_LColor;

    QColor m_ConnColor;

    bool m_isTCPConnection;

    QTcpSocket *m_TcpSocket;

    void Init(int x, int y, int w, int h, QColor lantern_color, QColor conn_color);
    void ConnectToServer(QString ip_adress,int port);
    void ConnectToServer();
    void DisconnectToServer();
signals:

public slots:
private slots:
    void onConnected();
    void onDisconnected();
    void onReadyRead();
protected:
    void paintEvent(QPaintEvent *event);
};

#endif // QNLCLIENT_H
