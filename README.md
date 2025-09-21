# XJTU-RMV-Task02

## tree.txt是显示项目结构的

## 运行时的终端图片
<img width="1526" height="986" alt="Screenshot from 2025-09-21 17-13-20" src="https://github.com/user-attachments/assets/4da2bf1e-115d-45ab-b2bb-8a958cd0e1c5" />


## 遇到的困难

### opencv配置

下载完后头文件若使用#include <opencv4/opencv2/core.hpp>会报错，解决无果后在q群求助，后更改路径为#include <opencv4/opencv2/opencv.hpp>，
编译成功return 0，但是有红色波浪线，解决再次无果后找了高组长求助，发现.hpp中的路径都为opencv2/...解决方法为给目录opencv2添加软链接到opencv4
<img width="820" height="500" alt="Screenshot from 2025-09-21 18-36-07" src="https://github.com/user-attachments/assets/a406b737-dad0-4301-83d2-7d371fecb0bd" />
<img width="740" height="388" alt="Screenshot from 2025-09-18 16-53-18" src="https://github.com/user-attachments/assets/7b40a09b-840d-4d4b-809d-5b3009fa51c7" />

