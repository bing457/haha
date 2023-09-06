#include <iomanip>
#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>

int main()
{
    std::string inputFolder = "Images/";   // 输入图片文件夹路径
    std::string outputFolder = "results/"; // 输出图片文件夹路径

    // 打开输入图片文件夹
    cv::String imgPattern = inputFolder + "*.jpg";
    std::vector<cv::String> imageFiles;
    cv::glob(imgPattern, imageFiles);

    // 遍历处理每张图片
    int imageIndex = 1;
    for (const auto &imagePath : imageFiles)
    {
        cv::Mat inputImage = cv::imread(imagePath);
        if (inputImage.empty())
        {
            std::cerr << "Failed to read image: " << imagePath << std::endl;
            continue;
        }

        // 图像处理步骤：转换为灰度图 -> 边缘检测 -> 寻找轮廓
        cv::Mat grayImage, edgeImage;
        cv::cvtColor(inputImage, grayImage, cv::COLOR_BGR2GRAY);
        cv::Canny(grayImage, edgeImage, 50, 150);

        std::vector<std::vector<cv::Point>> contours;
        cv::findContours(edgeImage, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

        // 标记轮廓和方向
        cv::Scalar lineColor(0, 255, 0); // 绿色
        std::string direction;
        if (contours.size() == 0)
        {
            direction = "直行";
        }
        else if (contours.size() == 1)
        {
            direction = "左拐";
        }
        else
        {
            direction = "右拐";
        }

        cv::Point textOrg(10, 30);
        int fontFace = cv::FONT_HERSHEY_SIMPLEX;
        double fontScale = 1;
        int thickness = 2;

        cv::putText(inputImage, direction, textOrg, fontFace, fontScale, lineColor, thickness);

        cv::drawContours(inputImage, contours, -1, lineColor, 2);

        // 保存标记后的图片
        std::ostringstream imagePathStream;
        imagePathStream << outputFolder << "image" << std::setw(3) << std::setfill('0') << imageIndex << ".jpg";
        std::string outputImagePath = imagePathStream.str();
        cv::imwrite(outputImagePath, inputImage);

        std::cout << "Processed image: " << outputImagePath << " - Direction: " << direction << std::endl;

        imageIndex++;
    }

    return 0;
}
