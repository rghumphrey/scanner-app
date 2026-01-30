#include <iostream>

#include <opencv2/opencv.hpp>

#include <ZXing/ReadBarcode.h>
#include <ZXing/TextUtfEncoding.h>
#include <ZXing/ReaderOptions.h>

#include "BarcodeScanner.h"
#include "OpenFoodFactsClient.h"


/* preprocess image to make scanning process smoother */
cv::Mat BarcodeScanner::preprocess(const cv::Mat& frame) const
{
    cv::Mat gray, sharp, blurred, thresh;

    cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

    // Sharpen
    cv::GaussianBlur(gray, blurred, {0, 0}, 3);
    cv::addWeighted(gray, 1.5, blurred, -0.5, 0, sharp);

    // Adaptive threshold
    cv::adaptiveThreshold(
        sharp, thresh, 255,
        cv::ADAPTIVE_THRESH_GAUSSIAN_C,
        cv::THRESH_BINARY, 31, 5
    );

    return thresh;
}

/* try to get the barcode # from frame */
std::string BarcodeScanner::decodeFrame(const cv::Mat& img) const
{
    ZXing::ImageView view(
        img.data,
        img.cols,
        img.rows,
        ZXing::ImageFormat::Lum
    );

    ZXing::ReaderOptions options;
    options.setTryHarder(true);
    options.setFormats(ZXing::BarcodeFormat::EAN13 |
                       ZXing::BarcodeFormat::UPCA |
                       ZXing::BarcodeFormat::UPCE);

    auto results = ZXing::ReadBarcodes(view, options);

    if (!results.empty()) {
        return results[0].text(); // ✅ JUST THE BARCODE
    }

    return "";
}

/* scan video stream for barcode, return once barcode with item is found */
std::string BarcodeScanner::scan()
{
    cv::VideoCapture cap(streamUrl, cv::CAP_FFMPEG);
    
    OpenFoodFactsClient client;

    if (!cap.isOpened()) {
        std::cerr << "Failed to open video stream\n";
        return "";
    }

    cv::Mat frame;

    while (true) {
        cap >> frame;
        if (frame.empty())
            continue;

        cv::Mat processed = preprocess(frame);
        std::string barcode = decodeFrame(processed);

        cv::imshow("Scanner", frame);

        if (!barcode.empty()) {
            std::cout << "Detected barcode: " << barcode << "\n";
            if(client.lookup(barcode))
                return barcode;
        }

        if (cv::waitKey(1) == 27) // ESC
            break;
    }

    return "";
}
