#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void renderTeams();
    ~MainWindow();

private slots:
    void on_loadTeamsBtn_clicked();

    void on_tableWidget_itemSelectionChanged();

    void on_deleteBtn_clicked();

    void on_unArchiveBtn_clicked();

    void on_filterRun_clicked();

    void on_loadChannelsBtn_clicked();

    void on_exportBtn_clicked();

    void on_createChannelsBtn_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
