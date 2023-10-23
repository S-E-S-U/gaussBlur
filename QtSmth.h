#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_qtsmth.h"
#include <opencv2/opencv.hpp>
#include <QImage>
#include <QPixmap>
#include <QLabel>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QSpinBox>
#include <QPushButton>
#include <QLineEdit>
#include <QProgressDialog>


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

private:
    Ui::QtSmthClass ui;
   
    void displayImage(const cv::Mat& image, QLabel* label);
    void applyFilter();
    void showAbout();

    std::vector<double> calculateGaussianKernel(int& sigma, int& kernelSize, int& kernelHalf);
    cv::Mat applyGaussianBlur(int& sigma);
    
    QLabel* originalImageLabel; 
    QLabel* blurredImageLabel;
    QLineEdit* sigmaInput;
    QPushButton* filterButton;
    QScrollArea* scrollArea;
    QAction* aboutAction;

    cv::Mat blurredImage;
    cv::Mat originalImage;
};
