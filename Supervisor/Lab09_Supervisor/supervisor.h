#ifndef SUPERVISOR_H
#define SUPERVISOR_H

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
    ~Supervisor();

private:
    QString boardPath;
};

#endif // SUPERVISOR_H
