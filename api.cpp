#include "api.h"



void tApi::archiveTeams(QTableWidget *tbl, std::map<std::string,teams::team> teamsList) {
    std::string rowC = "these teams will be deleted:\n";
    QItemSelectionModel *sel = tbl->selectionModel();
    QList<QModelIndex> rows = sel->selectedRows();
    for(int i = 0; i < rows.size(); i++) {
        QModelIndex row = rows.at(i);
        teams::team t = teamsList[tbl->item(row.row(),0)->text().toUtf8().constData()];
        t.archive();
    }
}

void tApi::unArchiveTeams(QTableWidget *tbl, std::map<std::string,teams::team> teamsList) {
    std::string rowC = "these teams will be deleted:\n";
    QItemSelectionModel *sel = tbl->selectionModel();
    QList<QModelIndex> rows = sel->selectedRows();
    for(int i = 0; i < rows.size(); i++) {
        QModelIndex row = rows.at(i);
        teams::team t = teamsList[tbl->item(row.row(),0)->text().toUtf8().constData()];
        t.unArchive();
    }
}


