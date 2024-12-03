#include "performer.h"

Performer::Performer() {}

Performer::Performer(QMainWindow *mainWindow) {
    m_mainWindow = mainWindow;
}

Performer::~Performer() {}

void Performer::establish_connection(){
    struct sockaddr_in serv_addr;
    if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
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

    if (connect(clientSocket, (struct sockaddr* )&serv_addr, sizeof(serv_addr)) < 0) {
        qDebug() << "Connection Failed";
        return;
    }
}

void Performer::send_idea(QString idea){
    send(clientSocket, idea.toStdString().c_str(), idea.length() + 1, 0);
}
