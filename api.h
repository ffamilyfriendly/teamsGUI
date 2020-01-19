#include "main.h"
#include <QSettings>
#include <QTableWidget>
#include <vector>
#include <QFileDialog>

namespace tApi {
    extern teams::client *c;
    void archiveTeams(QTableWidget *tbl, std::map<std::string,teams::team> teamsList);
    void unArchiveTeams(QTableWidget *tbl, std::map<std::string,teams::team> teamsList);
    std::map<std::string,teams::team::channel> loadChannels(QTableWidget *tbl,QTableWidget *cTbl, std::map<std::string,teams::team> teamsList);

    extern std::map<std::string,teams::team> teamsList;
    extern std::map<std::string,teams::team::channel> channelList;

    template<typename T>
    std::vector<T> getFromTable(QTableWidget *tbl, std::map<std::string,T> list) {
        QItemSelectionModel *sel = tbl->selectionModel();
        QList<QModelIndex> rows = sel->selectedRows();
        std::vector<T> return_data;
        for(int i = 0; i < rows.size(); i++) {
            QModelIndex row = rows.at(i);
            T t = list[tbl->item(row.row(),0)->text().toUtf8().constData()];
            return_data.push_back(t);
        }
        return return_data;
    }
}
