#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_qtsmth.h"
#include <QPixmap>
#include <QLabel>
#include <QVBoxLayout>
#include <Qmessagebox>
#include <QProgressDialog>
#include <QImage>
#include<QFileDialog>


class QtSmth : public QMainWindow
{
    Q_OBJECT

public:
    QtSmth(QWidget *parent = nullptr);
    ~QtSmth();

private slots:
    void on_actionOpen_triggered();
    void on_actionSave_triggered();
    void on_actionExit_triggered();   
    void applyFilter();

private:
    Ui::QtSmthClass ui;
   
    void displayImage(const QImage image, QLabel* label);
    void showAbout();
    std::vector<double> calculateGaussianKernel(int& sigma, int& kernelSize, int& kernelHalf);
    QImage applyGaussianBlur(int& sigma);

    QAction* aboutAction;
    QImage blurredImage;
    QImage originalImage;
};
