#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_supervisor = Supervisor(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btn_CreateBoard_clicked()
{
    if(false){ //if already created ask to create new

    }

    QString fileName = QFileDialog::getSaveFileName(this, tr("Create new Idea Board"), QDir::homePath() + "/ideaBoard.txt",
                                                    tr("Text file (*.txt)"));
    if (fileName.isEmpty()){
        QErrorMessage *err = new QErrorMessage(this);
        err->setWindowTitle("Incorrect file name");
        err->showMessage("Empty file name");
        return;
    }

    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Create new Idea Board", "File already exists.\nReplace it?",
                                      QMessageBox::Yes|QMessageBox::No);
        if (reply != QMessageBox::Yes) {
            file.close();
            return;
        }
    }

    file.close();

    //TODO

}


void MainWindow::on_btn_AskIdeas_clicked()
{
    if(false){ //if already asked show error

    }

    QDialog *dialog = new QDialog(this);
    dialog->setFixedSize(300, 200);
    dialog->setWindowTitle("Performers options");

    QLabel *performers_count_lable = new QLabel(dialog);
    performers_count_lable->setText("Performers count:");
    performers_count_lable->setGeometry(10, 10, 280, 20);

    QSpinBox *performers_count_spinBox = new QSpinBox(dialog);
    performers_count_spinBox->setGeometry(10, 40, 180, 20);
    performers_count_spinBox->setRange(PERFORMERS_COUNT_MIN, PERFORMERS_COUNT_MAX);


    QLabel *performers_time_lable = new QLabel(dialog);
    performers_time_lable->setText("Performers time:");
    performers_time_lable->setGeometry(10, 90, 280, 20);

    QSpinBox *performers_time_spinBox = new QSpinBox(dialog);
    performers_time_spinBox->setGeometry(10, 120, 180, 20);
    performers_time_spinBox->setRange(PERFORMERS_TIME_MIN, PERFORMERS_TIME_MAX);


    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel, dialog);
    buttonBox->setGeometry(10, 160, 280, 40);
    buttonBox->show();
    connect(buttonBox, SIGNAL(accepted()), dialog, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), dialog, SLOT(reject()));


    if (dialog->exec() == QDialog::Accepted) {

    }
}


void MainWindow::on_btn_StartVoting_clicked()
{

}

