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

void Supervisor::create_board_file(QString filePath) {
    m_filePath = filePath;

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

bool Supervisor::collect_ideas(int performersCount, int performersTime){

    m_performersPids.clear();
    m_performersSockets.clear();
    m_ideas.clear();

    m_sem = sem_open(SEM_NAME, O_CREAT, 0777, 1);
    if(m_sem == SEM_FAILED){
        qDebug() << "Error occured: creating semaphore. " << strerror(errno);
        return false;
    }
    sleep(2);

    //Creating performers
    m_performersPids.clear();
    pid_t supervisor_pid = getpid();
    for(int i = 0; i < performersCount; i++){
        if(getpid() == supervisor_pid){

            pid_t pid = fork();
            qDebug() << "forking";
            if(pid == -1){
                qDebug() << "fork fail";
                return false;
            } else if (pid == 0){
                // swapping with performer

                //TODO fix directories
                execl("../../../../Performer/Lab09_Performer/build/Desktop_Qt_6_7_2-Debug/Lab09_Performer", NULL);

                qDebug() << "Failed to execl =(";
                return false;
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
        return false;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, SERVER_PATH);


    // Getting a unique name for the socket
    if (bind(server_fd, (struct sockaddr*) &server_addr, SUN_LEN(&server_addr)) < 0) {
        qDebug() << "bind failed";
        return false;
    }

    //Allow the server to accept incoming client connections. Backlog = performersCount means that the
    //system will queue performersCount incoming connection requests before it start rejecting them
    if (listen(server_fd, performersCount) < 0) {
        qDebug() << "listen failed";
        return false;
    }

    for(int i = 0; i < performersCount; i++){
        //appending all the file handles
        //accept will block indefinitely waiting for the incoming connection to arrive
        m_performersSockets.append(accept(server_fd, (struct sockaddr*)&server_addr,
                                          &addrlen));
        // check for errors
        if (m_performersSockets.last() < 0) {
            qDebug() << "accept file handle error";
            return false;
        }
        // sending filePath to all the clients
        write(m_performersSockets.last(), m_filePath.toStdString().c_str(), m_filePath.length());
    }

    //change sleep time
    qDebug() << "going to sleep";
    sleep(performersTime);
    qDebug() << "Stopped sleeping";

    //Stopping all performers
    for (int i = 0; i < performersCount; i++){
        kill(m_performersPids[i], SIGUSR1);
        kill(m_performersPids[i], SIGSTOP);
    }

    sem_close(m_sem);
    sem_unlink(SEM_NAME);
    m_sem = nullptr;

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

    if(m_ideas.size() == 0) {
        for (int i = 0; i < m_performersPids.size(); i++){
            kill(m_performersPids[i], SIGCONT);
            kill(m_performersPids[i], SIGTERM);
            m_browserAll->setText("Nothing to show. Ask for ideas.");
            m_browserBest->setText("Nothing to show. Ask for ideas.");
        }
        return false;
    }


    m_status = IDEAS_COLLECTED;
    return true;
}

QList<unsigned> Supervisor::get_votes(){

    for (int i = 0; i < m_performersPids.size(); i++){
        kill(m_performersPids[i], SIGCONT);
    }
  
    QList<unsigned> votes(m_ideas.size(), 0);

    for(int clientInd = 0; clientInd < m_performersSockets.size(); clientInd++){
        char clientVotes[1024] = {0};
        int res = read(m_performersSockets[clientInd], clientVotes, 1024 - 1); // subtract 1 for the null terminator
        qDebug() << " read: " << res;
        clientVotes[1023] = '\0';
        for(int ideaInd = 0; ideaInd < m_ideas.size(); ideaInd++){
            if(clientVotes[ideaInd] == '1') votes[ideaInd]++;
        }
        close(m_performersSockets[clientInd]);
    }
    m_performersSockets.clear();

    for (int i = 0; i < m_performersPids.size(); i++){
        kill(m_performersPids[i], SIGTERM);
    }

    m_status = VOTING_COMPLETED;
    return votes;
}

void Supervisor::display_best_ideas(QList<unsigned> votes){
    m_browserBest->setText("Top 3 ideas:\n");
    qDebug() << votes;
    QList<std::pair<unsigned, QString>> votesPerIdea;
    for (int i = 0; i < m_ideas.size(); ++i) {
        votesPerIdea.append(std::pair<unsigned, QString>(votes[i], m_ideas[i]));
    }
    //sorry for this atrocity of a function call
    std::sort(votesPerIdea.begin(), votesPerIdea.end(),
        [](std::pair<unsigned, QString> x, std::pair<unsigned, QString> y){
        return x.first > y.first;
    });

    QFile board(m_filePath);
    board.open(QIODevice::Append);
    QTextStream out(&board);
    out << "\n\nTop 3 ideas:\n";

    int i = 0;
    while(i < votesPerIdea.size()){
        if(i == 3) break;
        QString str_bestIdea = votesPerIdea[i].second + " (" + QString::number(votesPerIdea[i].first);
        if (votesPerIdea[i].first % 10 == 1){
            str_bestIdea += " vote)";
        }
        else {
            str_bestIdea += " votes)";
        }
        m_browserBest->append(str_bestIdea);
        out << str_bestIdea + "\n";
        i++;
    }
    board.close();
}
