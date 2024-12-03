#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QErrorMessage>
#include <QMessageBox>

#include "performer.h"

void sigusr_handler(int signum);

static bool disableSubmittingVotes;
static bool disableSubmittingIdeas;


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

    void display_ideas();

private slots:
    void on_btn_SubbmitIdea_clicked();

    void on_btn_SubmitVotes_clicked();

private:
    Ui::MainWindow *ui;
    Performer m_performer;
    QList<QCheckBox*> m_checkBoxes;
    unsigned m_ideaCount;
};

static MainWindow* MainWindowPtr;

#endif // MAINWINDOW_H
