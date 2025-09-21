# XJTU-RMV-Task02

## 运行时的终端图片
<img width="1526" height="986" alt="Screenshot from 2025-09-21 17-13-20" src="https://github.com/user-attachments/assets/4da2bf1e-115d-45ab-b2bb-8a958cd0e1c5" />

## tree.txt是显示项目结构的

## 遇到的困难

### opencv配置

下载完后头文件若使用#include <opencv4/opencv2/core.hpp>会报错，解决无果后在q群求助，后更改路径为#include <opencv4/opencv2/opencv.hpp>，
编译成功return 0，但是有红色波浪线，解决再次无果后找了高组长求助，发现.hpp中的路径都为opencv2/...解决方法为给目录opencv2添加软链接到opencv4
<img width="820" height="500" alt="Screenshot from 2025-09-21 18-36-07" src="https://github.com/user-attachments/assets/a406b737-dad0-4301-83d2-7d371fecb0bd" />
<img width="740" height="388" alt="Screenshot from 2025-09-18 16-53-18" src="https://github.com/user-attachments/assets/7b40a09b-840d-4d4b-809d-5b3009fa51c7" />

## 完成思路

### 一.将任务文件中的教程喂给ai，让其详细化，包括格式、参数意义等

### 二.把任务一喂给ai，后逐行学习并检查

困难：
1.红色覆膜时ai给出了与培训不同的处理。表现为两个inrange和陌生的bitwise_or。了解到红色的色相在0和255，故需要两条代码并合并  
2.在红色区域轮廓、计算面积、以及bounding box时，ai取的是最大轮廓的面积和bounding box，后更改（最开始其实看不懂）
<img width="1120" height="331" alt="Screenshot from 2025-09-20 11-50-19" src="https://github.com/user-attachments/assets/90c2da0f-6508-4fde-884f-30cd18280539" />
3.寻找轮廓时发现少了std::vector\<cv::Vec4i> hierarchy，了解到是用于储存层级信息，后添加到源代码，未发现明显区别（即未深刻理解这行代码）  
4.漫水填充教程未涉及，自学后了解到功能类似油漆桶  
5.画圆的时候thickness值为-1，了解到为填充  
6.绘制红色轮廓时效果不好，最后尝试的方法为全局二值化，但是使用INV，即颜色反转，效果会更好

### 三.任务二只在筛选出灯条时使用了ai

采用椭圆拟合  
学习各个语句和参数（比如三元运算符）  
采用全局二值化，不用INV，不然会导致边框被框起来，无法识别  
最后筛选是调整面积参数得到的，并不稳定，希望以后能得到改进

