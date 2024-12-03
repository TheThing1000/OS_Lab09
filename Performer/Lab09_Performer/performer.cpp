#include "performer.h"

Performer::Performer() {}

Performer::Performer(QWidget *scrollAreaWidget) {
    m_scrollAreaWidget = scrollAreaWidget;
}

Performer::~Performer() {}

void Performer::establish_connection(){


    m_serverSocket = socket(AF_UNIX, SOCK_STREAM, 0);
    if (m_serverSocket == -1){
        qDebug() << "socket failed";
        return;
    }

    struct sockaddr_un server_addr;
    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, SERVER_PATH);

    if (connect(m_serverSocket, (struct sockaddr* )&server_addr, sizeof(server_addr)) < 0) {
        qDebug() << "connect failed";
        return;
    }

    char filePath[1024] = {0};
    read(m_serverSocket, filePath, 1024 - 1); // subtract 1 for the null terminator
    filePath[1023] = '\0';

    m_filePath = filePath;
    qDebug() << m_filePath;
}

void Performer::send_idea(QString idea){
    QFile board(m_filePath);
    board.open(QIODevice::Append);
    QTextStream out(&board);
    out << idea + "\n";
}

void Performer::display_ideas(){
    QFile board(m_filePath);
    board.open(QIODevice::ReadWrite);
    QTextStream in(&board);

    QList<QCheckBox*> checkBoxes;
    int idea_count =0;
    while (!in.atEnd()) {
        QStringList idea_tokens = in.readLine().split(" ");
        QString idea = idea_tokens[0] + " " + idea_tokens[1] + " " + idea_tokens[2] + " " + idea_tokens[3].removeLast();
        m_scrollAreaWidget->resize(m_scrollAreaWidget->width(), m_scrollAreaWidget->height() + 50);

        checkBoxes.append(new QCheckBox(m_scrollAreaWidget));
        checkBoxes.last()->setCheckState(Qt::Unchecked);
        checkBoxes.last()->setGeometry(10, 10 + 50 * idea_count, 200, 30);
        checkBoxes.last()->setText(idea);
        checkBoxes.last()->show();

        idea_count++;
    }

    board.close();
}
