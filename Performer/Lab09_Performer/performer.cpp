#include "performer.h"

Performer::Performer() {}

Performer::Performer(QMainWindow *mainWindow) {
    m_mainWindow = mainWindow;
}

Performer::~Performer() {}

void Performer::establish_connection(){
    struct sockaddr_in serv_addr;
    if ((m_serverSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        qDebug() << "Socket creation error";
        return;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary
    // form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)
        <= 0) {
        qDebug() << "Invalid address/ Address not supported";
        return;
    }

    if (connect(m_serverSocket, (struct sockaddr* )&serv_addr, sizeof(serv_addr)) < 0) {
        qDebug() << "Connection Failed";
        return;
    }

    char buffer[1024] = { 0 };
    read(m_serverSocket, buffer, 1024 - 1); // subtract 1 for the null terminator

    buffer[1023] = '\0';
    m_filePath = buffer;
    qDebug() << m_filePath;
}

void Performer::send_idea(QString idea){
    QFile board(m_filePath);
    board.open(QIODevice::Append);
    QTextStream out(&board);
    out << idea + "\n";
}
