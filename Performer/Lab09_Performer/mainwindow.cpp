#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_performer = Performer(this);
    m_performer.establish_connection();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btn_SubbmitIdea_clicked()
{
    m_performer.send_idea(ui->textEdit_IdeaText->toPlainText());
}


void MainWindow::on_btn_SubmitVotes_clicked()
{

}
