
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QDesktopServices>
#include <QUrl>
#include<QFileDialog>
#include "Terrian.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QMenuBar *menuBar=new QMenuBar(this);
    setMenuBar(menuBar);
    QMenu * fileMenu =menuBar->addMenu(tr("&File"));
    QAction *openAction =new QAction(tr("Open"),this);
    fileMenu->addAction(openAction);
    connect(openAction,&QAction::triggered,this,&MainWindow::openFileExplorerAndSelectFile);
    QLabel *labelFilePath=new QLabel(this);
    ui->statusbar->addWidget(labelFilePath);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openFileExplorerAndSelectFile()
{
    QString filePath =QFileDialog::getOpenFileName(this,tr("Select File"),QDir::homePath());

    if(!filePath.isEmpty())
    {
        selectedFilePath=filePath;
        //Terrian::FilePath = selectedFilePath;

        //ui->labelFilePath->setText(Terrian::FilePath);
    }
}
