#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define PERFORMERS_COUNT_MIN 1
#define PERFORMERS_COUNT_MAX 16
#define PERFORMERS_TIME_MIN 1 //mins
#define PERFORMERS_TIME_MAX 5 //mins

#include <QMainWindow>
#include "supervisor.h"

#include <QSpinBox>
#include <QFileDialog>
#include <QErrorMessage>

#include <QFile>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btn_CreateBoard_clicked();

    void on_btn_AskIdeas_clicked();

    void on_btn_StartVoting_clicked();

private:
    Ui::MainWindow *ui;
    Supervisor m_supervisor;
};
#endif // MAINWINDOW_H
