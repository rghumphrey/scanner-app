#ifndef BARCODESCANNER_H_
#define BARCODESCANNER_H_

#include <string>
#include <opencv2/opencv.hpp>


class BarcodeScanner {
public:
    explicit BarcodeScanner(const std::string& url) : streamUrl(url) {};
    std::string scan();
private:
    std::string streamUrl;

    std::string decodeFrame(const cv::Mat& frame) const;
    cv::Mat preprocess(const cv::Mat& frame) const;
};


#endif