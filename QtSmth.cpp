#include "qtsmth.h"

QtSmth::QtSmth(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    connect(ui.filterButton, &QPushButton::clicked, this, &QtSmth::applyFilter);

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


QImage QtSmth::applyGaussianBlur(int& sigma) {

    int width = originalImage.width();
    int height = originalImage.height();
    
    int kernelSize = 2 * sigma + 1;
    int kernelHalf = (kernelSize - 1) / 2;

    std::vector<double> gaussianKernel = calculateGaussianKernel(sigma, kernelSize, kernelHalf);

    QImage blurredQImage(width, height, QImage::Format_RGB888);

    //Horisontal
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            double sumR = 0.0, sumG = 0.0, sumB = 0.0;
            for (int i = 0; i < kernelSize; i++) {
                int xPos = x + (i - kernelHalf);

                if (xPos < 0 || xPos >= width)
                    xPos = x;

                QRgb pixel = originalImage.pixel(xPos, y);
                sumR += qRed(pixel) * gaussianKernel[i];
                sumG += qGreen(pixel) * gaussianKernel[i];
                sumB += qBlue(pixel) * gaussianKernel[i];
            }
            blurredQImage.setPixel(x, y, qRgb(sumR, sumG,sumB));
        }
    }

    //Vertical
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            double sumR = 0.0, sumG = 0.0, sumB = 0.0;
            for (int i = 0; i < kernelSize; i++) {
                int yPos = y + (i - kernelHalf);

                if (yPos < 0 || yPos >= height)
                    yPos = y;

                QRgb pixel = blurredQImage.pixel(x, yPos);
                sumR += qRed(pixel) * gaussianKernel[i];
                sumG += qGreen(pixel) * gaussianKernel[i];
                sumB += qBlue(pixel) * gaussianKernel[i];
            }
            blurredQImage.setPixel(x, y, qRgb(sumR, sumG, sumB));
        }
    }

    return blurredQImage;
}

void QtSmth::applyFilter() {
    int sigma = ui.sigmaInput->text().toInt();

    if (sigma <= 0 || sigma > 20 || sigma == NULL) {
        QMessageBox::warning(this, "Warning", "Invalid sigma value");
        return;
    }
    
    blurredImage = applyGaussianBlur(sigma);
    displayImage(blurredImage, ui.blurredImageLabel);

}


void QtSmth::displayImage(const QImage image, QLabel* label) {

     QPixmap pixmap = QPixmap::fromImage(image);
     label->setPixmap(pixmap);
 }


void QtSmth::on_actionOpen_triggered() {

    QString filePath = QFileDialog::getOpenFileName(this, "Open Image", "", "Images (*.png *.jpg *.bmp)");

    if (!filePath.isEmpty()) {
        originalImage.load(filePath);
        displayImage(originalImage, ui.originalImageLabel);
    }
}


void QtSmth::on_actionSave_triggered() {

    if (blurredImage.isNull()) {
        QMessageBox::warning(this, "Warning", "There is no image to save");
        return;
    }

    QString filePath = QFileDialog::getSaveFileName(this, "Save Image", "", "Images (*.png *.jpg *.bmp)");
    if (!filePath.isEmpty()) {
        blurredImage.save(filePath);
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