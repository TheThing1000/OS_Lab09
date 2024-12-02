#ifndef SUPERVISOR_H
#define SUPERVISOR_H

#include <QMainWindow>
#include <QMessageBox>
#include <QDebug>

#include <QString>

#include <sys/wait.h>
#include <unistd.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <semaphore.h>


class Supervisor {
public:
    Supervisor();
    Supervisor(QMainWindow *mainWindow);
    ~Supervisor();

private:
    QString m_boardPath;
    QMainWindow *m_mainWindow;
};

#endif // SUPERVISOR_H
