#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "api.h"
#include "login.h"
#include <iostream>
#include <QMessageBox>
#include <QTextStream>

std::map<std::string,teams::team> tApi::teamsList;
std::map<std::string,teams::team::channel> tApi::channelList;

void openLoginForm() {
    login login;
    login.setModal(true);
    login.exec();
}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    openLoginForm();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::renderTeams() {
    ui->statusbar->showMessage("loading teams...");
    ui->tableWidget->setRowCount(0);
    for(std::pair<std::string, teams::team> element : tApi::teamsList) {
        ui->tableWidget->insertRow(ui->tableWidget->rowCount());
        //set item values
        ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,0,new QTableWidgetItem(QString::fromUtf8(element.second.id.c_str())));
        ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,1,new QTableWidgetItem(QString::fromUtf8(element.second.displayName.c_str())));
        ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,2,new QTableWidgetItem(QString::fromUtf8(element.second.mail.c_str())));
        ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,3,new QTableWidgetItem(QString::fromUtf8(element.second.createdDateTime.c_str())));
    }
    ui->statusbar->showMessage("loaded teams!");
}

void MainWindow::on_loadTeamsBtn_clicked()
{
   tApi::teamsList = tApi::c->getTeams();
   this->renderTeams();
}



void MainWindow::on_tableWidget_itemSelectionChanged()
{
    QItemSelectionModel *select = ui->tableWidget->selectionModel();
    QString deleteBtnStr = QString("archive ") + QString(QString::number(select->selectedRows().count())) + " teams";
    QString unArch = QString("unArchive ") + QString(QString::number(select->selectedRows().count())) + " teams";
    ui->deleteBtn->setText(deleteBtnStr);
    ui->unArchiveBtn->setText(unArch);
}

std::string dueDeletion(QTableWidget *tbl,std::string action = "archived") {
        std::string rowC = "these teams will be " + action + ":\n";
        QItemSelectionModel *sel = tbl->selectionModel();
        QList<QModelIndex> rows = sel->selectedRows();
        for(int i = 0; i < rows.size(); i++) {
            QModelIndex row = rows.at(i);
            teams::team t = tApi::teamsList[tbl->item(row.row(),0)->text().toUtf8().constData()];
            std::string dt = std::string("name: ") + std::string(t.displayName) + std::string("\n");
            rowC+=dt;
        }
        return rowC;
}

void MainWindow::on_deleteBtn_clicked()
{
    QItemSelectionModel *select = ui->tableWidget->selectionModel();
    std::string warnText = std::string("this action will archive ") + std::to_string(select->selectedRows().count()) + " teams";
    QMessageBox msgBox;
    msgBox.setWindowTitle("Teams | confirm action");
    msgBox.setText(warnText.c_str());
    msgBox.setDetailedText(QString::fromStdString(dueDeletion(ui->tableWidget)));
    msgBox.setStandardButtons(QMessageBox::Cancel | QMessageBox::Yes);
    int res = msgBox.exec();
    if(res == QMessageBox::Yes) {
        ui->statusbar->showMessage("archiving teams...",1000*5);
        try {
            tApi::archiveTeams(ui->tableWidget,tApi::teamsList);
            ui->statusbar->showMessage("archiving teams done!",1000*10);
        } catch(std::runtime_error e) {
            QString errmsg = QString("Error: ") + QString(e.what());
            ui->statusbar->showMessage(errmsg);
        }
    }
}

void MainWindow::on_unArchiveBtn_clicked()
{
    QItemSelectionModel *select = ui->tableWidget->selectionModel();
    std::string warnText = std::string("this action will un-archive ") + std::to_string(select->selectedRows().count()) + " teams";
    QMessageBox msgBox;
    msgBox.setWindowTitle("Teams | confirm action");
    msgBox.setText(warnText.c_str());
    msgBox.setDetailedText(QString::fromStdString(dueDeletion(ui->tableWidget,"un-archived")));
    msgBox.setStandardButtons(QMessageBox::Cancel | QMessageBox::Yes);
    int res = msgBox.exec();
    if(res == QMessageBox::Yes) {
        ui->statusbar->showMessage("un-archiving teams...",1000*5);
        try {
            tApi::unArchiveTeams(ui->tableWidget,tApi::teamsList);
            ui->statusbar->showMessage("un-archiving teams done!",1000*10);
        } catch(std::runtime_error e) {
            QString errmsg = QString("Error: ") + QString(e.what());
            ui->statusbar->showMessage(errmsg);
        }

    }
}

void MainWindow::on_filterRun_clicked()
{
    try {
    for(std::pair<std::string, teams::team> element : tApi::teamsList) {
        std::string id = element.first;
        if(!ui->filterName->toPlainText().isEmpty()) {
            QString tName = QString(element.second.displayName.c_str());
            if(!tName.contains(ui->filterName->toPlainText())) {
                tApi::teamsList.erase(id);
            }
        }
        if(!ui->filterTime->date().isNull()) {
            QDate fT = ui->filterTime->date();
            QDate tT = QDate::fromString(QString(element.second.createdDateTime.c_str()),Qt::DateFormat::ISODate);

            if(ui->beforeOrAfter->isChecked()) {
                if(tT > fT) {
                    tApi::teamsList.erase(id);
                }
            } else {
                if(tT < fT) {
                    tApi::teamsList.erase(id);
                }
            }
        }
    }
    this->renderTeams();
    } catch(const std::bad_alloc&) {
        ui->statusbar->showMessage("bad alloc. can not filter");
    }
}

void MainWindow::on_loadChannelsBtn_clicked()
{
    ui->statusbar->showMessage("loading channels...",1000*5);
    tApi::channelList = tApi::loadChannels(ui->tableWidget,ui->channelsTable,tApi::teamsList);
}

void MainWindow::on_exportBtn_clicked()
{
    std::vector<teams::team> tList = tApi::getFromTable<teams::team>(ui->tableWidget,tApi::teamsList);
    QString uri = QFileDialog::getSaveFileName(this,tr("save file"));
    std::vector<teams::team> tVec = tApi::getFromTable<teams::team>(ui->tableWidget,tApi::teamsList);
    QFile file(uri);
    if(file.open(QIODevice::ReadWrite)) {
        QTextStream out(&file);
        for(unsigned int i = 0; i < tVec.size(); i++) {
            teams::team ct = tVec.at(i);
            out << ct.id.c_str() << "," << ct.displayName.c_str() << "," << ct.mailEnabled << "," << ct.mail.c_str() << "," << ct.createdDateTime.c_str() << "\n";
        }
        file.close();
    }
}

