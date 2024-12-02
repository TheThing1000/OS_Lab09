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
    m_status = IDEAS_COLLECTED;
}

void Supervisor::start_voting(){
    m_status = VOTING_COMPLETED;
}
