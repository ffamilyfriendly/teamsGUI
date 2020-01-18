#include "main.h"
#include <QSettings>
#include <QTableWidget>

namespace tApi {
    extern teams::client *c;
    void archiveTeams(QTableWidget *tbl, std::map<std::string,teams::team> teamsList);
    void unArchiveTeams(QTableWidget *tbl, std::map<std::string,teams::team> teamsList);
}
