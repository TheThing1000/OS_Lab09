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
    m_ideasCount = 0;

    signal(SIGUSR1, sigusr_handler);
    disableSubmittingVotes = true;
    disableSubmittingIdeas = false;
    MainWindowPtr = this;

    connect(this, &MainWindow::on_sigusr, this, &MainWindow::display_ideas);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btn_SubbmitIdea_clicked()
{
    if (disableSubmittingIdeas){
        QErrorMessage *err = new QErrorMessage(this);
        err->setWindowTitle("Failed to submit an idea");
        err->showMessage("You can't submit ideas right now.");
        return;
    }
    if(ui->textEdit_IdeaText->toPlainText().isEmpty()){
        QErrorMessage *err = new QErrorMessage(this);
        err->setWindowTitle("Failed to submit an idea");
        err->showMessage("Idea can't be empty.");
        return;
    }

    m_ideasCount++;
    QString idea("PID: ");
    idea += QString::number(getpid());
    idea += "; Idea #";
    idea += QString::number(m_ideasCount);
    idea += ": ";
    idea += ui->textEdit_IdeaText->toPlainText();
    idea.remove('\n');
    idea.remove('\r');
    m_performer.write_idea_to_board(idea);
    ui->textEdit_IdeaText->setText("");
}


void MainWindow::on_btn_SubmitVotes_clicked()
{
    if (disableSubmittingVotes){
        QErrorMessage *err = new QErrorMessage(this);
        err->setWindowTitle("Failed to submit votes");
        err->showMessage("You can't submit votes right now.");
        return;
    }

    m_performer.send_votes(m_performer.collect_votes(m_ideaCheckBoxes));
    disableSubmittingVotes = true;

    QMessageBox::information(this, "Votes submitted!", "Take a rest.");
}

void MainWindow::display_ideas(){
    m_ideaCheckBoxes = m_performer.display_ideas();
}

void sigusr_handler(int signum){
    disableSubmittingIdeas = true;
    disableSubmittingVotes = false;
    qDebug() << "DISPLAY";
    emit MainWindowPtr->on_sigusr();
}
