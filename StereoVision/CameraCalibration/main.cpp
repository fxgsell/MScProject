#include <iostream>
#include "opencv2/opencv.hpp"


int main(int, char**)
{
  int frameCount = 0;
  cv::VideoCapture capLeft(0);
  cv::VideoCapture capRight(1);

  capRight.set(CV_CAP_PROP_FRAME_WIDTH, 640);
  capRight.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
  capLeft.set(CV_CAP_PROP_FRAME_WIDTH, 640);
  capLeft.set(CV_CAP_PROP_FRAME_HEIGHT, 480);

  if(!capLeft.isOpened() || !capRight.isOpened()) {
    std::cerr << "ERROR: Could not open cameras." << std::endl;
    return -1;
  }
  
  cv::namedWindow("Left",1);
  cv::namedWindow("Right",1);
  
  for(;;) {
    bool isValid = true;
    
    cv::Mat frameLeft;
    cv::Mat frameRight;

    capLeft >> frameLeft;
    capRight >> frameRight;

    try {
      cv::imshow("Left", frameLeft);
      cv::imshow("Right", frameRight);

      if (frameCount % 30 == 0) {
        std::ostringstream fileNameL;
        fileNameL << "images/Left" << frameCount << ".ppm";

        std::ostringstream fileNameR;
        fileNameL << "images/Right" << frameCount << ".ppm";

        cv::imwrite(fileNameL.str(), frameLeft);
        cv::imwrite(fileNameR.str(), frameRight);
        //cv::imwrite(fileNameL.str(), frameLeft, std::vector<int>(CV_IMWRITE_PXM_BINARY, 1));
        //cv::imwrite(fileNameR.str(), frameRight, std::vector<int>(CV_IMWRITE_PXM_BINARY, 1));
      }
      ++frameCount;
    } catch( cv::Exception& e ) {
      std::cout << "An exception occurred. Ignoring frame. " << e.err << std::endl;
    }
    cv::waitKey(30);
  }
  return EXIT_SUCCESS;
}
