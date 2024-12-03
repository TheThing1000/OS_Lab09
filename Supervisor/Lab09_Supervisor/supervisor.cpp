#include "supervisor.h"

Supervisor::Supervisor() {
    m_browserAll = nullptr;
    m_browserBest = nullptr;
    m_status = NO_BOARD;
}

Supervisor::Supervisor(QTextBrowser *browserAll,
                       QTextBrowser *browserBest) {
    m_browserAll = browserAll;
    m_browserBest = browserBest;
    m_status = NO_BOARD;
}

Supervisor::~Supervisor() {}

void Supervisor::create_board_file(QString fileName) {
    m_filePath = fileName;

    QFile board(m_filePath);
    board.open(QIODevice::WriteOnly);
    board.close();

    m_browserAll->setText("Nothing to show. Ask for ideas.");
    m_browserBest->setText("Nothing to show. Ask for ideas.");
    m_status = BOARD_CREATED;
}

STATUS Supervisor::get_status() {
    return m_status;
}

void Supervisor::collect_ideas(int performersCount, int performersTime){

    //Creating performers
    m_performersPids.clear();
    pid_t supervisor_pid = getpid();
    for(int i = 0; i < performersCount; i++){
        if(getpid() == supervisor_pid){

            pid_t pid = fork();
            qDebug() << "forking";
            if(pid == -1){
                qDebug() << "fork fail";
                return;
            } else if (pid == 0){
                // swapping with performer

                //TODO fix directories
                execl("../../../../Performer/Lab09_Performer/build/Desktop_Qt_6_7_2-Debug/Lab09_Performer", NULL);

                qDebug() << "Failed to execl =(";
                return;
            } else {
                m_performersPids.append(pid);
            }
        }
    }

    int server_fd = -1;
    struct sockaddr_un server_addr;
    socklen_t addrlen = sizeof(server_addr);


    if (access(SERVER_PATH, F_OK) != -1) {
        unlink(SERVER_PATH); // delete the old socket
    }

    // Creating socket file descriptor
    if ((server_fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
        qDebug() << "socket failed";
        return;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, SERVER_PATH);


    // Getting a unique name for the socket
    if (bind(server_fd, (struct sockaddr*) &server_addr, SUN_LEN(&server_addr)) < 0) {
        qDebug() << "bind failed";
        return;
    }

    //Allow the server to accept incoming client connections. Backlog = performersCount means that the
    //system will queue performersCount incoming connection requests before it start rejecting them
    if (listen(server_fd, performersCount) < 0) {
        qDebug() << "listen failed";
        return;
    }

    for(int i = 0; i < performersCount; i++){
        //appending all the file handles
        //accept will block indefinitely waiting for the incoming connection to arrive
        m_performersSockets.append(accept(server_fd, (struct sockaddr*)&server_addr,
                                          &addrlen));
        // check for errors
        if (m_performersSockets.last() < 0) {
            qDebug() << "accept file handle error";
            return;
        }
        // sending filePath to all the clients
        // send(m_performersSockets.last(), m_filePath.toStdString().c_str(), m_filePath.length(), 0);
        write(m_performersSockets.last(), m_filePath.toStdString().c_str(), m_filePath.length());
    }

    //change sleep time
    qDebug() << "going to sleep";
    sleep(15);
    qDebug() << "Stopped sleeping";

    //Stopping all performers
    for (int i = 0; i < performersCount; i++){
        kill(m_performersPids[i], SIGUSR1);
        kill(m_performersPids[i], SIGSTOP);
    }

    //TODO stop voting

    QFile board(m_filePath);
    board.open(QIODevice::ReadWrite);
    QTextStream in(&board);

    m_browserAll->setText("All ideas:\n");
    m_browserBest->setText("Nothing to show. Vote for ideas.");

    //Read ideas from file into ideas_vector
    while (!in.atEnd()) {
        QString idea = in.readLine();
        m_ideas.append(idea);
        m_browserAll->append(idea);
    }

    board.close();
    m_status = IDEAS_COLLECTED;
}

QList<unsigned> Supervisor::start_voting(){

    for (int i = 0; i < m_performersPids.size(); i++){
        kill(m_performersPids[i], SIGCONT);
    }
  
    QList<unsigned> votes(m_ideas.size(), 0);

    for(int clientInd = 0; clientInd < m_performersSockets.size(); clientInd++){
        char clientVotes[1024] = {0};
        read(m_performersSockets[clientInd], clientVotes, 1024 - 1); // subtract 1 for the null terminator
        clientVotes[1023] = '\0';
        for(int ideaInd = 0; ideaInd < m_ideas.size(); ideaInd++){
            if(clientVotes[ideaInd] == '1') votes[ideaInd]++;
        }
        close(m_performersSockets[clientInd]);
    }
    m_performersSockets.clear();

    m_status = VOTING_COMPLETED;
    return votes;
}

void Supervisor::display_best(QList<unsigned> votes){
    m_browserBest->setText("Top 3 ideas:\n");

}
