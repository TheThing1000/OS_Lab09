#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_performer = Performer(this);
    qDebug() << "Establishing connection...";
    m_performer.establish_connection();
    m_ideaCount = 0;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btn_SubbmitIdea_clicked()
{
    m_ideaCount++;
    QString idea("PID: ");
    idea += QString::number(getpid());
    idea += "; Idea #";
    idea += QString::number(m_ideaCount);
    idea += ": ";
    idea += ui->textEdit_IdeaText->toPlainText();
    m_performer.send_idea(idea);
    ui->textEdit_IdeaText->setText("");
}


void MainWindow::on_btn_SubmitVotes_clicked()
{

}
