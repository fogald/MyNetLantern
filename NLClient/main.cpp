#include "dialog.h"
#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Dialog w;
    w.show();

    int i;
    int arc_count = argc;
    QMessageBox msg;
    QString *s;
    QString ip_adress;
    int port;

    QStringList cmdline_args = QCoreApplication::arguments();

    s = new QString[arc_count];
    for(i=0;i<=arc_count-1;i++)
    {
        s[i] = cmdline_args.at(i);
    }

    if(argc>1)
    {
        ip_adress = s[1];
        port = s[2].toInt();

        w.m_NLClient.ConnectToServer(ip_adress,port);
    }
    else
    {
        msg.setText("Не заданы IP-адерес и номер порта!");
        msg.exec();
    }

    ip_adress = "127.0.0.1";
    port = 9999;

    w.m_NLClient.ConnectToServer(ip_adress,port);

    return a.exec();
}
