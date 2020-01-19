#include "api.h"

void tApi::archiveTeams(QTableWidget *tbl, std::map<std::string,teams::team> teamsList) {
    std::vector<teams::team> tl = getFromTable<teams::team>(tbl,teamsList);
    for(unsigned int i = 0; i < tl.size(); i++) {
        teams::team dueArchive = tl.at(i);
        dueArchive.archive();
    }
}

void tApi::unArchiveTeams(QTableWidget *tbl, std::map<std::string,teams::team> teamsList) {
    std::vector<teams::team> tl = getFromTable<teams::team>(tbl,teamsList);

    for(unsigned int i = 0; i < tl.size(); i++) {
        teams::team dueArchive = tl.at(i);
        dueArchive.unArchive();
    }
}

void populateChannelTable(std::map<std::string,teams::team::channel> c,QTableWidget *tbl) {
    for(std::pair<std::string, teams::team::channel> element : c) {
        tbl->insertRow(tbl->rowCount());
        //set item values
        tbl->setItem(tbl->rowCount()-1,0,new QTableWidgetItem(QString::fromUtf8(element.second.id.c_str())));
        tbl->setItem(tbl->rowCount()-1,1,new QTableWidgetItem(QString::fromUtf8(element.second.displayName.c_str())));
        tbl->setItem(tbl->rowCount()-1,2,new QTableWidgetItem(QString::fromUtf8(element.second.email.c_str())));
    }
}

std::map<std::string,teams::team::channel> tApi::loadChannels(QTableWidget *tbl,QTableWidget *cTbl, std::map<std::string,teams::team> teamsList) {
    cTbl->setRowCount(0);
    std::map<std::string,teams::team::channel> lst;

    std::vector<teams::team> tl = getFromTable<teams::team>(tbl,teamsList);
    for(unsigned int i = 0; i < tl.size(); i++) {
        teams::team t = tl.at(i);
        std::map<std::string,teams::team::channel> channels = t.getChannels();
        populateChannelTable(channels,cTbl);
        lst.insert(channels.begin(),channels.end());
    }
    return lst;
}


