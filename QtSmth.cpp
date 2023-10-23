#include "qtsmth.h"
#include <qmessagebox.h>
#include<qfiledialog.h>
#include <opencv2/opencv.hpp>
#include<qlabel.h>
#include<QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QProgressDialog>


QtSmth::QtSmth(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    originalImageLabel = new QLabel(this);
    ui.imageTab->setLayout(new QVBoxLayout);
    ui.imageTab->layout()->addWidget(originalImageLabel);

    blurredImageLabel = new QLabel(this);
    sigmaInput = new QLineEdit(this);
    filterButton = new QPushButton("Apply Filter", this);

    QVBoxLayout* filterTabLayout = new QVBoxLayout(ui.filterTab);

    filterTabLayout->addWidget(sigmaInput);
    filterTabLayout->addWidget(filterButton);
    filterTabLayout->addWidget(blurredImageLabel);

    connect(filterButton, &QPushButton::clicked, this, &QtSmth::applyFilter);

    aboutAction = new QAction("About", this);
    ui.menuBar->addAction(aboutAction);
    connect(aboutAction, &QAction::triggered, this, &QtSmth::showAbout);
    
}

QtSmth::~QtSmth() {}

const double PI = acos(-1.0);

std::vector<double> QtSmth::calculateGaussianKernel(int& sigma, int& kernelSize, int& kernelHalf) {

    std::vector<double> kernel(kernelSize);
    double sum = 0.0;

    for (int x = -kernelHalf; x <= kernelHalf; x++) {
        double gX = std::exp(-x * x / (2.0 * sigma * sigma)) / (std::sqrt(2 * PI) * sigma);
        kernel[x + kernelHalf] = gX;
        sum += gX;
    }

    for (int i = 0; i < kernelSize; i++) {
        kernel[i] /= sum;
    }

    return kernel;
}

cv::Mat QtSmth::applyGaussianBlur(int& sigma) {

    int width = originalImage.cols;
    int height = originalImage.rows;

    int kernelSize = 2 * sigma + 1;
    int kernelHalf = (kernelSize - 1) / 2;

    std::vector<double> gaussianKernel = calculateGaussianKernel(sigma, kernelSize, kernelHalf);

    cv::Mat blurredImage(height, width, CV_8UC3);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            double sumR = 0.0, sumG = 0.0, sumB = 0.0;
            for (int i = 0; i < kernelSize; i++) {
                int xPos = x + (i - kernelHalf);
               
                if (xPos < 0 || xPos >= originalImage.cols) 
                    xPos = x;

                sumR += originalImage.at<cv::Vec3b>(y, xPos)[2] * gaussianKernel[i];
                sumG += originalImage.at<cv::Vec3b>(y, xPos)[1] * gaussianKernel[i];
                sumB += originalImage.at<cv::Vec3b>(y, xPos)[0] * gaussianKernel[i];
                
            }
            blurredImage.at<cv::Vec3b>(y, x) = cv::Vec3b(sumB, sumG, sumR);
        }
    }
    return blurredImage;
}

void QtSmth::applyFilter() {
    int sigma = sigmaInput->text().toInt();

    if (sigma <= 0 || sigma > 20) {
        QMessageBox::warning(this, "Warning", "Invalid sigma value");
        return;
    }
    
    blurredImage = applyGaussianBlur(sigma);
    displayImage(blurredImage, blurredImageLabel);

}

 void QtSmth::displayImage(const cv::Mat& image, QLabel* label) {

     QImage qtImage(image.data, image.cols, image.rows, image.step, QImage::Format_BGR888);
     QPixmap pixmap = QPixmap::fromImage(qtImage);
     label->setPixmap(pixmap);
     label->setScaledContents(false);
     label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
 }

void QtSmth::on_actionOpen_triggered() {

    QString filePath = QFileDialog::getOpenFileName(this, "Open Image", "", "Images (*.png *.jpg *.bmp)");
    if (!filePath.isEmpty()) {

        originalImage = cv::imread(filePath.toStdString());

        displayImage(originalImage, originalImageLabel);
        
    }

}

void QtSmth::on_actionSave_triggered() {

    if (blurredImage.empty()) {
        QMessageBox::warning(this, "Warning", "There is no image to save");
        return;
    }

    QString filePath = QFileDialog::getSaveFileName(this, "Save Image", "", "Images (*.png *.jpg *.bmp)");
    if (!filePath.isEmpty()) {
        cv::imwrite(filePath.toStdString(), blurredImage);
    }
};

void QtSmth::on_actionExit_triggered() {
    qApp->quit();
};


void QtSmth::showAbout() {

    QString aboutText = "Title: " + QtSmth::windowTitle() + "\n";
    aboutText += "Version: 1.2.5.\n";
    aboutText += "Developer: Amina\n";

    QMessageBox::about(this, "About", aboutText);
};



