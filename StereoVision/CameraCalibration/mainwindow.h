#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv2/opencv.hpp>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow      *ui;
    cv::VideoCapture    capLeft;
    cv::VideoCapture    capRight;
    cv::Mat             frameLeft;
    cv::Mat             frameRight;
    QImage              qimgRight;
    QImage              qimgLeft;
    int                 frameCount;
    QTimer*             tmrTimer;

public slots:
    void    processFrame();

private slots:
    void on_btnPauseResume_clicked();
    void on_btnCapture_clicked();

};

#endif // MAINWINDOW_H
