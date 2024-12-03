#ifndef SUPERVISOR_H
#define SUPERVISOR_H

#define PORT 8080

#include <QMainWindow>
#include <QMessageBox>
#include <QDebug>

#include <QString>
#include <QFile>
#include <QTextStream>

#include <sys/wait.h>
#include <unistd.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <semaphore.h>

enum STATUS {NO_BOARD, BOARD_CREATED, IDEAS_COLLECTED, VOTING_COMPLETED};

class Supervisor {
public:
    Supervisor();
    Supervisor(QMainWindow *mainWindow);
    ~Supervisor();

    void create_board_file(QString fileName);

    void collect_ideas(int performersCount, int performersTime);

    void start_voting();

    STATUS get_status();

private:
    QString m_filePath;
    QMainWindow *m_mainWindow;
    STATUS m_status;
};

#endif // SUPERVISOR_H
