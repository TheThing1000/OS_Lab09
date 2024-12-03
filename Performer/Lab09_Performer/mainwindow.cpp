#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_performer = Performer(ui->scrollAreaWidgetContents);
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
    idea.remove('\n');
    idea.remove('\r');
    m_performer.write_idea_to_board(idea);
    ui->textEdit_IdeaText->setText("");
}


void MainWindow::on_btn_SubmitVotes_clicked()
{
    m_checkBoxes = m_performer.display_ideas();

    //m_performer.send_votes(m_performer.collect_votes(m_checkBoxes));
}
