#include "performer.h"

Performer::Performer() {}

Performer::Performer(QWidget *scrollAreaWidget) {
    m_scrollAreaWidget = scrollAreaWidget;
}

Performer::~Performer() {}

void Performer::establish_connection(){

    m_sem = sem_open(SEM_NAME, O_EXCL);
    if(m_sem == SEM_FAILED){
        qDebug() << "sem open client: " << strerror(errno);
        exit(1);
    }

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

void Performer::write_idea_to_board(QString idea){
    sem_wait(m_sem);
    QFile board(m_filePath);
    board.open(QIODevice::Append);
    QTextStream out(&board);
    out << idea + "\n";
    board.close();
    qDebug() << m_filePath;
    sem_post(m_sem);
}

QList<QCheckBox*> Performer::display_ideas(){
    sem_wait(m_sem);
    QFile board(m_filePath);
    board.open(QIODevice::ReadOnly);
    QTextStream in(&board);

    QList<QCheckBox*> ideaCheckBoxes;
    int idea_count =0;
    while (!in.atEnd()) {
        QStringList idea_tokens = in.readLine().split(" ");
        QString idea = idea_tokens[0] + " " + idea_tokens[1] + " " + idea_tokens[2] + " " + idea_tokens[3].removeLast();
        m_scrollAreaWidget->resize(m_scrollAreaWidget->width(), m_scrollAreaWidget->height() + 50);
        ideaCheckBoxes.append(new QCheckBox(m_scrollAreaWidget));
        ideaCheckBoxes.last()->setCheckState(Qt::Unchecked);
        ideaCheckBoxes.last()->setGeometry(10, 10 + 50 * idea_count, 200, 30);
        ideaCheckBoxes.last()->setText(idea);
        ideaCheckBoxes.last()->show();

        idea_count++;
    }

    board.close();
    sem_post(m_sem);
    sem_close(m_sem);
    m_sem = nullptr;
    return ideaCheckBoxes;
}

QList<bool> Performer::collect_votes(QList<QCheckBox*> ideaCheckBoxes){
    QList<bool> votes(ideaCheckBoxes.size(), false);
    for(int i = 0; i < ideaCheckBoxes.size(); i++){
        votes[i] = (ideaCheckBoxes[i]->checkState() == Qt::Checked) ? true : false;
    }
    return votes;
}

void Performer::send_votes(QList<bool> votes){
    QString votesStr("");
    for(int i = 0; i < votes.size(); i++)
        votesStr += votes[i] ? '1' : '0';

    write(m_serverSocket, votesStr.toStdString().c_str(), votesStr.length());
}


