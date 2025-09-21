#include <iostream>
#include <opencv4/opencv2/core/mat.hpp>
#include <opencv4/opencv2/core/types.hpp>
#include <opencv4/opencv2/highgui.hpp> 
#include <opencv4/opencv2/imgproc.hpp>
#include <opencv4/opencv2/opencv.hpp>



int main()
{
    //导入图片
    std::string img_path = "/home/heish/Documents/Projects/opencv_project/resources/test_image_2.png";
    cv::Mat img = cv::imread(img_path);


    if (img.empty()) {
        std::cerr << "无法加载图片: " << img_path << std::endl;
        return -1;
    }

    //灰度化
    cv::Mat gray, hsv;
    cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);

    //二值化
    cv::Mat bin;
    cv::threshold(gray, bin, 200, 255, cv::THRESH_BINARY);//用INV会导致轮廓检测失败
    
   
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(bin, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    
    for (const auto& contour : contours) {
        // --- 筛选标准 1: 轮廓点数 ---
        // fitEllipse 函数要求轮廓点数至少为5
        if (contour.size() < 15) { // 我们设定一个更高的阈值，过滤掉太简单的轮廓
            continue;
        }

        // --- 拟合椭圆 ---
        // fitEllipse 返回一个 RotatedRect 对象
        cv::RotatedRect fitted_ellipse = cv::fitEllipse(contour);

        // --- 筛选标准 2: 面积比 ---
        double contour_area = cv::contourArea(contour);
        
        if (contour_area < 700) { // 过滤掉面积过小的轮廓
            continue;
        }
        
        // RotatedRect的size成员包含椭圆外接矩形的长和宽，即椭圆的长轴和短轴
        double ellipse_area = CV_PI * (fitted_ellipse.size.width / 2.0) * (fitted_ellipse.size.height / 2.0);
        
        double area_ratio = contour_area / ellipse_area;

        // 如果面积比偏离1.0太多（比如超过20%），则认为不是一个好的椭圆
        if (std::abs(1.0 - area_ratio) > 0.2) {
            continue;
        }

        // --- 筛选标准 3: 长宽比 ---
        // 确保长轴是较长的一边
        double aspect_ratio = (fitted_ellipse.size.width > fitted_ellipse.size.height) ?
                              (fitted_ellipse.size.width / fitted_ellipse.size.height) :
                              (fitted_ellipse.size.height / fitted_ellipse.size.width);

        // 我们只想要比较“扁”的椭圆，排除掉近似圆形的物体
        if (aspect_ratio < 1.2 || aspect_ratio > 5.0) {
            continue;
        }

        // --- 绘制外接矩形 ---
        cv::Rect boundingBox = cv::boundingRect(contour);
        cv::rectangle(img, boundingBox, cv::Scalar(0, 0, 255), 2);
    }
    cv::imwrite("/home/heish/Documents/Projects/opencv_project/output/output_task2.jpg", img);
    return 0;

}