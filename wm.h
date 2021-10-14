#ifndef WM_H
#define WM_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QMessageBox>
#include <QDateTime>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#define HeaderCSV QString("Name,Password,Date\n")
QT_BEGIN_NAMESPACE
namespace Ui { class WM; }
QT_END_NAMESPACE

class WM : public QMainWindow
{
    Q_OBJECT

public:
    WM(QWidget *parent = nullptr);
    ~WM();
    int randInt(int low, int high);
    QString randString(int len);

    QString GetCurrentDateTime(const QString &format);
    void WritePasswords();
    void ReadPasswords();
private slots:
    void on_btn_Gen_clicked();
    void on_lineNameOfPass_editingFinished();

private:
    Ui::WM *ui;

    // QWidget interface
protected:
    void closeEvent(QCloseEvent *event);
};
#endif // WM_H
