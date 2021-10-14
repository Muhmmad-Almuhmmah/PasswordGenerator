#include "wm.h"
#include "ui_wm.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    WM w;
    w.show();
    return a.exec();
}

WM::WM(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::WM)
{
    ui->setupUi(this);
    ReadPasswords();
}

WM::~WM()
{
    delete ui;
}

int WM::randInt(int low, int high)
{
    // Random number between low and high
    return qrand() % ((high + 1) - low) + low;
}

QString WM::randString(int len)
{
    QString str;
    str.resize(len);
    for (int s = 0; s < len ; ++s){
        if(s % 2==0)
            str[s] = QChar('A' + char(qrand() % ('Z' - 'A')));
        else
            str[s]=QChar(randInt(1,100));
    }

    return str;
}

QString WM::GetCurrentDateTime(const QString &format)
{
    QLocale testLocale = QLocale(QLocale::English, QLocale::UnitedStates);
    QString dateTimeText = testLocale.toString(QDateTime::currentDateTime(), format);
    return dateTimeText;
}

void WM::WritePasswords()
{
    if(!ui->tw_Passwords->topLevelItemCount()) return;
    QFile file("UserPasswords.csv");
    file.open(QIODevice::WriteOnly);
    QTextStream stream(&file);
    stream<<HeaderCSV;
    for (int index=0;index<ui->tw_Passwords->topLevelItemCount();index++) {
        auto item=ui->tw_Passwords->topLevelItem(index);
        stream<<QString("%1,%2,%3\n").arg(item->text(0)).arg(item->text(1)).arg(item->text(2));
    }
    file.flush();
    file.close();
}

void WM::ReadPasswords()
{
    QFile file("UserPasswords.csv");
    file.open(QIODevice::ReadOnly);
    QString text=file.readAll();
    if(!text.startsWith(HeaderCSV)) return;
    QStringList listPass=text.split("\n"),colums;
    for (int index=1;index<listPass.count();index++) {
        colums=QString(listPass.at(index)).split(",");
        if(colums.count()!=3) continue;
        ui->tw_Passwords->addTopLevelItem(new QTreeWidgetItem(colums));
    }
    file.close();
}

void WM::on_btn_Gen_clicked()
{
    if(ui->ch_Save->isChecked() and ui->lineNameOfPass->text().isEmpty()){
        QMessageBox::warning(this,"Error","Please Input Name First!");
        ui->lineNameOfPass->setFocus();
        return;
    }

    QString item=randString(ui->spinBox->value());
    if(!ui->cmbType->currentIndex())
        item=item.toLatin1().toBase64(QByteArray::OmitTrailingEquals);
    else
        item=item.toLatin1().toHex().mid(0,ui->spinBox->value());
    ui->linePassword->setText(item);
    if(!ui->lineNameOfPass->text().isEmpty())
        ui->tw_Passwords->addTopLevelItem(new QTreeWidgetItem(
                                              QStringList()<<ui->lineNameOfPass->text()
                                              <<ui->linePassword->text()
                                              <<GetCurrentDateTime("yyyy/MM/dd hh:mm:ss")));
}

void WM::on_lineNameOfPass_editingFinished()
{
    if(!ui->lineNameOfPass->text().isEmpty())
        ui->ch_Save->setChecked(true);
}

void WM::closeEvent(QCloseEvent *event)
{
    WritePasswords();
    event->accept();
}
