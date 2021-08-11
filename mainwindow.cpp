#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "initdb.h"
#include <QMessageBox>
#include <QSqlError>
#include <QSqlQueryModel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QSqlError err = initDb();
    if (err.type() != QSqlError::NoError) {
        QMessageBox::critical(this, tr("Error"), err.text());
        return;
    }

    QSqlQueryModel* model = new QSqlQueryModel(this);
    model->setQuery("select * from vudocuments");
    if (model->lastError().isValid()) {
        QMessageBox::critical(this, tr("Database error"), model->lastError().text());
        return;
    }
    ui->tableView->setModel(model);
}

MainWindow::~MainWindow()
{
    delete ui;
}

