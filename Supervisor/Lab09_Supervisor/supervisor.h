#ifndef SUPERVISOR_H
#define SUPERVISOR_H

#define PORT 8080

#define SERVER_PATH "/tmp/server"

#include <QMainWindow>
#include <QMessageBox>
#include <QDebug>

#include <QString>
#include <QFile>
#include <QTextStream>
#include <QList>
#include <QStringList>
#include <QTextBrowser>

#include <sys/wait.h>
#include <unistd.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/un.h>

enum STATUS {NO_BOARD, BOARD_CREATED, IDEAS_COLLECTED, VOTING_COMPLETED};

class Supervisor {
public:
    Supervisor();
    Supervisor(QTextBrowser *browserAll,
               QTextBrowser *browserBest);
    ~Supervisor();

    void create_board_file(QString fileName);

    void collect_ideas(int performersCount, int performersTime);

    QList<unsigned> start_voting();

    STATUS get_status();

private:
    QString m_filePath;
    QTextBrowser *m_browserAll;
    QTextBrowser *m_browserBest;
    STATUS m_status;
    QList<int> m_performersSockets;
    QList<pid_t> m_performersPids;
    QStringList m_ideas;
};

#endif // SUPERVISOR_H
