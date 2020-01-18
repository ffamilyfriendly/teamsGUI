#include "login.h"
#include "ui_login.h"
#include "api.h"
#include <QMessageBox>
#include <iostream>

teams::client *tApi::c = new teams::client();
QSettings settings("gaming","club");

login::login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::login)
{
    ui->setupUi(this);
    if(settings.contains("id") && settings.contains("token")) {
        ui->cId->setPlainText(settings.value("id").toString());
        ui->cToken->setPlainText(settings.value("token").toString());
    }
}

login::~login()
{
    delete ui;
    if(!tApi::c->isInitialized()) {
        QMessageBox::critical(this,tr("Error | initializing"),tr("could not initialize client"));
    }
}


void login::on_pushButton_clicked()
{
    QString id = ui->cId->toPlainText();
    QString token = ui->cToken->toPlainText();
    if(id.isEmpty() || token.isEmpty()) {
        QMessageBox::warning(this,tr("Warning | Fields"),tr("required fields are empty."));
        return;
    }

    try {
        tApi::c->login(id.toUtf8().constData(),token.toUtf8().constData());
        if(ui->rememberMe->isChecked()) {
            settings.setValue("id",id);
            settings.setValue("token",token);
        }
        this->close();
    } catch(std::runtime_error e) {
        QMessageBox::critical(this,tr("Error | Login"),tr(e.what()));
        return;
    }
}

