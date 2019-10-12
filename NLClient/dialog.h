#ifndef DIALOG_H
#define DIALOG_H

#include "qnlclient.h"
#include <QDialog>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

    QNLClient m_NLClient;
    bool m_isConnection;

    void on_Dialog_finished(int result);

private slots:
    void on_ButtonClose_clicked();

    void on_ButtonConnect_clicked();

private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
