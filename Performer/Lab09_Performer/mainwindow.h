#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "performer.h"

void sigusr_handler(int signum);

static bool disableSubmitting;

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
    void on_btn_SubbmitIdea_clicked();

    void on_btn_SubmitVotes_clicked();

private:
    Ui::MainWindow *ui;
    Performer m_performer;
    QList<QCheckBox*> m_checkBoxes;
    unsigned m_ideaCount;
};
#endif // MAINWINDOW_H
