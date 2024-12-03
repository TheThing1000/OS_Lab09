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
    pid_t supervisor_pid = getpid();
    for(int i = 0; i < performersCount; i++){
        if(getpid() == supervisor_pid){

            pid_t pid = fork();
            qDebug() << "!";
            if(pid == -1){
                qDebug() << "fork";
                return;
            } else if (pid == 0){
                //execl();
                std::string cmd_line = "/home/thething/TheThing/OS Lab09/Lab09/OS_Lab09/Supervisor/test_client";
                char* cmd = new char[cmd_line.length() + 1];
                strcpy(cmd, cmd_line.c_str());
                execl("/usr/bin/xterm", "xterm", "-T", "New Process", "-e", cmd, NULL);
                delete[] cmd;
                qDebug() << "execl";
                return;
            }
        }
    }

    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    socklen_t addrlen = sizeof(address);

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        qDebug() << "socket failed";
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET,
                   SO_REUSEADDR | SO_REUSEPORT, &opt,
                   sizeof(opt))) {
        qDebug() << "setsockopt";
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr*)&address,
             sizeof(address))
        < 0) {
        qDebug() << "bind failed";
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0) {
        qDebug() << "listen";
        exit(EXIT_FAILURE);
    }
    if ((new_socket
         = accept(server_fd, (struct sockaddr*)&address,
                  &addrlen))
        < 0) {
        qDebug() << "accept";
        exit(EXIT_FAILURE);
    }

    char buffer[1024] = { 0 };
    char* hello = "Hello from server";
    read(new_socket, buffer,
                   1024 - 1); // subtract 1 for the null
        // terminator at the end
    printf("%s\n", buffer);
    send(new_socket, hello, strlen(hello), 0);
    printf("Hello message sent\n");

    // closing the connected socket
    close(new_socket);
    // closing the listening socket
    close(server_fd);


    board.close();
    m_status = IDEAS_COLLECTED;
}

void Supervisor::start_voting(){
    m_status = VOTING_COMPLETED;
}
