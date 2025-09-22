#include <iostream>
#include <opencv4/opencv2/core/mat.hpp>
#include <opencv4/opencv2/highgui.hpp> 
#include <opencv4/opencv2/imgproc.hpp>
#include <opencv4/opencv2/opencv.hpp>

//绝大部分为ai生成

int main()
{
    std::string img_path = "/home/heish/Documents/Projects/opencv_project/resources/test_image.png"; // 请根据实际图片路径修改
    cv::Mat img = cv::imread(img_path);
    if (img.empty()) {
        std::cerr << "无法加载图片: " << img_path << std::endl;
        return -1;
    }

    // 颜色空间转换
    cv::Mat gray, hsv;
    cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);
    cv::cvtColor(img, hsv, cv::COLOR_BGR2HSV);

    // 均值滤波 & 高斯滤波
    cv::Mat mean_blur, gauss_blur;
    cv::blur(img, mean_blur, cv::Size(5, 5));
    cv::GaussianBlur(img, gauss_blur, cv::Size(5, 5), 0);

    // HSV方法提取红色区域 0/179
    cv::Mat mask1, mask2, red_mask;
    cv::inRange(hsv, cv::Scalar(0, 0, 0), cv::Scalar(20, 255, 255), mask1);
    cv::inRange(hsv, cv::Scalar(157, 0, 0), cv::Scalar(180, 255, 255), mask2);
    cv::bitwise_or(mask1, mask2, red_mask); 

    

    //开闭运算
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
    cv::morphologyEx(red_mask, red_mask, cv::MORPH_OPEN, kernel);
    cv::morphologyEx(red_mask, red_mask, cv::MORPH_CLOSE, kernel);
    
    
    // 寻找红色外轮廓
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(red_mask, contours, hierarchy,cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    
    

    //寻找bounding box
    cv::Mat red_img = img.clone();
    double area_sum = 0.0;
    for (size_t i = 0; i < contours.size(); i++) {  //遍历
        // 计算当前轮廓的面积，过滤掉太小的噪点轮廓
        double area = cv::contourArea(contours[i]);
        if (area > 200) { // 面积阈值，根据实际情况调整
            // 绘制轮廓本身 (蓝色)
            cv::drawContours(red_img, contours, static_cast<int>(i), cv::Scalar(255, 0, 0), 2);
            area_sum += area;
            // 计算并绘制该轮廓的 Bounding Box (绿色)
            cv::Rect boundingBox = cv::boundingRect(contours[i]);
            cv::rectangle(red_img, boundingBox, cv::Scalar(0, 255, 0), 2);
    }
}
    std::cout << "检测到的红色区域总面积: " << area_sum << std::endl;

    // 灰度化、二值化、高亮区域膨胀腐蚀
    cv::Mat bin, dilated, eroded;
    cv::threshold(gray, bin, 160, 255, cv::THRESH_BINARY);
    cv::dilate(bin, dilated, kernel);
    cv::erode(dilated, eroded, kernel);
     
    // 漫水填充
    cv::Mat flood = eroded.clone();
    cv::floodFill(flood, cv::Point(0,0), cv::Scalar(128),0, cv::Scalar(10), cv::Scalar(10));

    // 图像绘制（圈、框、文字）
    cv::Mat draw_img = img.clone();
    cv::circle(draw_img, cv::Point(50,50), 30, cv::Scalar(255,255,0), -1);
    cv::rectangle(draw_img, cv::Rect(100,100,60,40), cv::Scalar(0,255,255), 2);
    cv::putText(draw_img, "OpenCV", cv::Point(200,200), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0,0,255), 2);
    
    //绘制轮廓、bounding box
    cv::Mat edges;
    cv::threshold(gray, edges, 150,255, cv::THRESH_BINARY_INV);//二值化
    cv::morphologyEx(edges, edges, cv::MORPH_OPEN, kernel);
    cv::morphologyEx(edges, edges, cv::MORPH_CLOSE, kernel);
    std::vector<std::vector<cv::Point>> contours2;
    std::vector<cv::Vec4i> hierarchy2;
    cv::findContours(edges, contours2,hierarchy2, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    for (size_t i = 0; i < contours2.size(); i++) {
        double area = cv::contourArea(contours2[i]);
        if (area < 10) continue; //过滤小轮廓
        if (area > 200) {
            cv::drawContours(draw_img, contours2, static_cast<int>(i), cv::Scalar(0, 0, 255), 2);
            cv::Rect boundingBox = cv::boundingRect(contours2[i]);
            cv::rectangle(draw_img, boundingBox, cv::Scalar(0, 0, 255), 2);
        }
    }
 
    // 图像旋转35度
    cv::Mat rot_img;
    cv::Point2f center(img.cols/2.0, img.rows/2.0);  //旋转中心
    cv::Mat rot_mat = cv::getRotationMatrix2D(center, 35, 1.0);
    cv::warpAffine(img, rot_img, rot_mat, img.size());

    // 裁剪左上角1/4
    cv::Rect roi(0, 0, img.cols/2, img.rows/2);
    cv::Mat cropped = img(roi);

    // 保存结果
    cv::imwrite("/home/heish/Documents/Projects/opencv_project/output/gray.jpg", gray);
    cv::imwrite("/home/heish/Documents/Projects/opencv_project/output/hsv.jpg", hsv);
    cv::imwrite("/home/heish/Documents/Projects/opencv_project/output/mean_blur.jpg", mean_blur);
    cv::imwrite("/home/heish/Documents/Projects/opencv_project/output/gauss_blur.jpg", gauss_blur);
    cv::imwrite("/home/heish/Documents/Projects/opencv_project/output/red_mask.jpg", red_mask);
    cv::imwrite("/home/heish/Documents/Projects/opencv_project/output/draw.jpg", draw_img);
    cv::imwrite("/home/heish/Documents/Projects/opencv_project/output/rotated.jpg", rot_img);
    cv::imwrite("/home/heish/Documents/Projects/opencv_project/output/cropped.jpg", cropped);
    cv::imwrite("/home/heish/Documents/Projects/opencv_project/output/flood.jpg", flood);
    cv::imwrite("/home/heish/Documents/Projects/opencv_project/output/red_img.jpg", red_img);

    std::cout << "处理完成，结果已保存到 output 文件夹。" << std::endl;
    return 0;
    
}