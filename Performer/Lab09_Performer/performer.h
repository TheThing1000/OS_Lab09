#ifndef PERFORMER_H
#define PERFORMER_H

#define PORT 8080

#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include <QMainWindow>
#include <QStringList>
#include <QFile>
#include <QTextStream>

class Performer
{
public:
    Performer();
    Performer(QMainWindow *mainWindow);
    ~Performer();

    void establish_connection();
    void send_idea(QString idea);
    QStringList recieve_ideas();
    void send_votes(QString idea);

private:
    int m_serverSocket;
    QMainWindow *m_mainWindow;
    QString m_filePath;
};

#endif // PERFORMER_H
