#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_supervisor = Supervisor(ui->textBrowser_AllIdeas, ui->textBrowser_BestIdeas);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btn_CreateBoard_clicked()
{
    if (m_supervisor.get_status() != NO_BOARD) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Create new Idea Board", "Idea Board already exists.\nDo you wish to replace it?",
                                      QMessageBox::Yes|QMessageBox::No);
        if (reply != QMessageBox::Yes) {
            return;
        }
    }

    QString filePath = QFileDialog::getSaveFileName(this, tr("Create new Idea Board"), QDir::homePath() + "/ideaBoard.txt",
                                                    tr("Text file (*.txt)"));
    if (filePath.isEmpty()){
        QErrorMessage *err = new QErrorMessage(this);
        err->setWindowTitle("Incorrect file name");
        err->showMessage("File name can't be empty");
        return;
    }

    m_supervisor.create_board_file(filePath);
}


void MainWindow::on_btn_AskIdeas_clicked()
{
    if (m_supervisor.get_status() != BOARD_CREATED) {
        QErrorMessage *err = new QErrorMessage(this);
        err->setWindowTitle("Failed to collect ideas");
        // err->showMessage("Create Idea Board first (or ideas were already collected)");
        err->showMessage("An Idea Board has not been created or ideas have already been collected.");
        return;
    }

    QDialog *dialog = new QDialog(this);
    dialog->setFixedSize(300, 200);
    dialog->setWindowTitle("Performers options");

    QLabel *performers_count_lable = new QLabel(dialog);
    performers_count_lable->setText("Performers count:");
    performers_count_lable->setGeometry(10, 10, 280, 20);

    QSpinBox *performers_count_spinBox = new QSpinBox(dialog);
    performers_count_spinBox->setGeometry(10, 40, 180, 20);
    performers_count_spinBox->setValue(3);
    performers_count_spinBox->setRange(PERFORMERS_COUNT_MIN, PERFORMERS_COUNT_MAX);


    QLabel *performers_time_lable = new QLabel(dialog);
    performers_time_lable->setText("Performers time (seconds):");
    performers_time_lable->setGeometry(10, 90, 280, 20);

    QSpinBox *performers_time_spinBox = new QSpinBox(dialog);
    performers_time_spinBox->setGeometry(10, 120, 180, 20);
    performers_time_spinBox->setValue(60);
    performers_time_spinBox->setRange(PERFORMERS_TIME_MIN, PERFORMERS_TIME_MAX);


    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel, dialog);
    buttonBox->setGeometry(10, 160, 280, 40);
    buttonBox->show();
    connect(buttonBox, SIGNAL(accepted()), dialog, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), dialog, SLOT(reject()));


    if (dialog->exec() == QDialog::Accepted) {
        if(!m_supervisor.collect_ideas(performers_count_spinBox->value(),
                                        performers_time_spinBox->value())){
            QErrorMessage *err = new QErrorMessage(this);
            err->setWindowTitle("Failed to collect ideas");
            err->showMessage("No ideas were submitted or other error happend. Restart idea submititing");
        }
    }
}


void MainWindow::on_btn_StartVoting_clicked()
{
    if (m_supervisor.get_status() != IDEAS_COLLECTED) {
        QErrorMessage *err = new QErrorMessage(this);
        err->setWindowTitle("Failed to start the voting");
        err->showMessage("An Idea Board has not been created or no ideas have been collected yet.");
        return;
    }

    QList<unsigned> votes = m_supervisor.get_votes();

    QMessageBox::information(this, "Voting completed", "Voting completed.\nTop 3 ideas formed.");

    m_supervisor.display_best_ideas(votes);
}
