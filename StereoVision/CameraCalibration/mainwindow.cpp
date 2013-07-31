#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtCore>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    capLeft(0),
    capRight(1)
{
    ui->setupUi(this);

    capRight.set(CV_CAP_PROP_FRAME_WIDTH, 640);
    capRight.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
    capLeft.set(CV_CAP_PROP_FRAME_WIDTH, 640);
    capLeft.set(CV_CAP_PROP_FRAME_HEIGHT, 480);

    if (capRight.isOpened() == false) {
        ui->output->appendPlainText("error: cannot open webcam");
        return;
    }


    tmrTimer = new QTimer(this);
    connect(tmrTimer, SIGNAL(timeout()), this, SLOT(processFrame()));
    tmrTimer->start(20);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::processFrame() {
    capLeft >> frameLeft;
    capRight >> frameRight;

    if (frameLeft.empty() && frameRight.empty()) return;

    QImage qimgLeft((uchar*)frameLeft.data, frameLeft.cols, frameLeft.rows, frameLeft.step,
                    QImage::Format_RGB888);
    QImage qimgRight((uchar*)frameRight.data, frameRight.cols, frameRight.rows, frameRight.step,
                     QImage::Format_RGB888);

    ui->left->setPixmap(QPixmap::fromImage(qimgLeft));
    ui->right->setPixmap(QPixmap::fromImage(qimgRight));
}

void MainWindow::on_btnPauseResume_clicked()
{
        std::cerr << "resume" << std::endl;
    if (tmrTimer->isActive()) {
        tmrTimer->stop();
        ui->btnPauseResume->setText("Resume");
    } else {
        tmrTimer->start(20);
        ui->btnPauseResume->setText("Pause");
    }
}

void MainWindow::on_btnCapture_clicked()
{
        std::cerr << "cap" << std::endl;
    /*
    std::ostringstream fileNameL;
    fileNameL << "images/Left" << frameCount << ".jpg";

    std::ostringstream fileNameR;
    fileNameL << "images/Right" << frameCount << ".jpg";

    cv::imwrite(fileNameL.str(), frameLeft);
    cv::imwrite(fileNameR.str(), frameRight);
    */
}
