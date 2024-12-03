#include "supervisor.h"

Supervisor::Supervisor() {
    m_mainWindow = nullptr;
    m_status = NO_BOARD;
}

Supervisor::Supervisor(QMainWindow *mainWindow) {
    m_mainWindow = mainWindow;
    m_status = NO_BOARD;
}

Supervisor::~Supervisor() {}

void Supervisor::create_board_file(QString fileName) {
    m_filePath = fileName;

    QFile board(m_filePath);
    board.open(QIODevice::WriteOnly);
    board.close();

    m_status = BOARD_CREATED;
}

STATUS Supervisor::get_status() {
    return m_status;
}

void Supervisor::collect_ideas(int performersCount, int performersTime){
    QFile board(m_filePath);
    board.open(QIODevice::WriteOnly);
    QTextStream out(&board);
    out << "\tAll Ideas:\n";

    //Creating performers
    m_performersPids.clear();
    pid_t supervisor_pid = getpid();
    for(int i = 0; i < performersCount; i++){
        if(getpid() == supervisor_pid){

            pid_t pid = fork();
            qDebug() << "!";
            if(pid == -1){
                qDebug() << "fork";
                return;
            } else if (pid == 0){
                execl("/home/thething/TheThing/OS Lab09/Lab09/OS_Lab09/Performer/Lab09_Performer/build/Desktop_Qt_6_7_3-Release/Lab09_Performer", NULL);
                //std::string cmd_line = "/home/thething/TheThing/OS Lab09/Lab09/OS_Lab09/Supervisor/test_client";
                // char* cmd = new char[cmd_line.length() + 1];
                // strcpy(cmd, cmd_line.c_str());
                // execl("/usr/bin/xterm", "xterm", "-T", "New Process", "-e", cmd, NULL);
                // delete[] cmd;
                qDebug() << "execl";
                return;
            } else {
                m_performersPids.append(pid);
            }
        }
    }

    int server_fd;
    struct sockaddr_in address;
    int opt = 1;
    socklen_t addrlen = sizeof(address);

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        qDebug() << "socket failed";
        return;;
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET,
                   SO_REUSEADDR | SO_REUSEPORT, &opt,
                   sizeof(opt))) {
        qDebug() << "setsockopt";
        return;;
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr*)&address,
             sizeof(address))
        < 0) {
        qDebug() << "bind failed";
        return;;
    }
    if (listen(server_fd, performersCount) < 0) {
        qDebug() << "listen";
        return;;
    }

    for(int i = 0; i < performersCount; i++){
        m_performersSockets.append(accept(server_fd, (struct sockaddr*)&address,
                                          &addrlen));
        if (m_performersSockets.last() < 0) {
            qDebug() << "accept";
            return;;
        }
    }

    qDebug() << "!!!!";

    int ideas_collected_count = 0;
    while (ideas_collected_count < performersCount){
        qDebug() << "!";
        for(int i = 0; i < m_performersSockets.size(); i++){
            qDebug() << "!!";
            char buffer[1024] = { 0 };
            read(m_performersSockets[i], buffer,
                 1024 - 1); // subtract 1 for the null
            // terminator at the end
            buffer[1023] = '\0';
            m_ideas.append(buffer);
            qDebug() << m_ideas.last();
            out << m_ideas.last();
            ideas_collected_count++;
        }
    }


    board.close();
    m_status = IDEAS_COLLECTED;
}

void Supervisor::start_voting(){
    m_status = VOTING_COMPLETED;
}
