#include <opencv2/opencv.hpp>
#include <iostream>

int main(){
    // biru 
    // cv::Mat image = cv::imread("../docs/objek.jpg");

    //merah
    // cv::Mat image = cv::imread("../docs/baju.jpg");

    // //hijau
    // cv::Mat image = cv::imread("../docs/mangga.jpg");
    
    cv::VideoCapture cap("http://10.176.240.121:8080/video");
    cv::Mat frame;

    //perulangan
    while (cap.isOpened()){
        cap >> frame;

        if (frame.empty()){
            break;
        }

    cv::Mat hlsImage;
    cv::Mat blurred;
    
    //mengkonversi gambar dari BGR ke HLS
    cv
    double areaMax = 1000;   // area minim::GaussianBlur(frame, blurred, cv::Size(7, 7), 0);
    cv::cvtColor(blurred, hlsImage, cv::COLOR_BGR2HLS);

    //color threshold mask
    cv::Mat biru, merah1, merah2, hijau, thresholdedImage;

    cv::inRange(hlsImage,
        //biru
        cv::Scalar(80, 100, 100),
        cv::Scalar(130, 255, 255),
        biru);

    cv::inRange(hlsImage,
        //merah
        cv::Scalar(10, 30, 30),
        cv::Scalar(179, 255, 255),
        merah1);

    cv::inRange(hlsImage,
        //merah
        cv::Scalar(0, 30, 30),
        cv::Scalar(170, 255, 255),
        merah2);
    cv::Mat merah = merah1 | merah2;

    cv::inRange(hlsImage,
        // //hijau 
        cv::Scalar(35, 30, 30),
        cv::Scalar(85, 255, 255),  
        hijau);

    thresholdedImage = biru | merah | hijau;

    //morphology
    cv::Mat morphology = thresholdedImage.clone();
    
    cv::erode(morphology, morphology, cv::Mat(), cv::Point(-1,-1),2);
    cv::dilate(morphology, morphology, cv::Mat(), cv::Point(-1,-1),8);
    cv::erode(morphology, morphology, cv::Mat(), cv::Point(-1,-1),5);

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(morphology, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    cv::Mat result = frame.clone();

    for(size_t i = 0; i < contours.size(); i++){
        if(cv::contourArea(contours[i]) > 1000){
            cv::Rect boundingBox = cv::boundingRect(contours[i]);
            cv::rectangle(result, boundingBox, cv::Scalar(0, 255, 0),2);
        }
    }

    // cv::imshow("Gambar", image);
    // cv::imshow("hlsImage", hlsImage);
    cv::imshow("ThresholdedIMage", thresholdedImage);
    // cv::imshow("morphologyImage", morphology);
    cv::imshow("BoundingBox", result);

        if (cv::waitKey(25) == 'q'){
            break;
        }

}

    return 0;
}