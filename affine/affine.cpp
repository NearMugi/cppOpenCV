#include <opencv2/opencv.hpp>
#include <iostream>
#include <chrono>

using namespace std;

template <typename ... Args>
std::string format(const std::string& fmt, Args ... args )
{
    size_t len = std::snprintf( nullptr, 0, fmt.c_str(), args ... );
    std::vector<char> buf(len + 1);
    std::snprintf(&buf[0], len + 1, fmt.c_str(), args ... );
    return std::string(&buf[0], &buf[0] + len);
}

// 座標・スケール・角度を変更するMatを生成
cv::Mat affineMat(const cv::Mat &img, const int tx, const int ty, const float scale, const float angle){
	// 回転の中心座業
	cv::Point2d ctr(img.cols/2, img.rows/2);
	// 回転行列の作成
	cv::Mat mat = cv::getRotationMatrix2D(ctr, angle, scale);
	// 回転後の平行移動量
	mat.at<double>(0,2) +=tx;
	mat.at<double>(1,2) +=ty;		
	return mat;	
}

// スケール・角度を変更するMatを生成
cv::Mat affineMat(const cv::Mat &img, const float scale, const float angle){
	return affineMat(img, 0, 0, scale, angle);
}

// 座標を変更するMatを生成
cv::Mat affineMat(const cv::Mat &img, const int tx, const int ty){
	return affineMat(img, tx, ty, 1.0, 0.0);	
}

int main(void)
{   
	// 時間計測
	chrono::system_clock::time_point loopTime, processStart, processEnd;
	double processTime, waitTime;

	// fps and mpf(ms per frame)
	double fps=10;
	double mpf=1000 / fps;
	std::cout << format("FPS:%f(%fms)",fps, mpf) << std::endl;

	std::string windowName = "Affine";
	cv::namedWindow(windowName, cv::WINDOW_NORMAL);
	// FullScreen
	cv::setWindowProperty(windowName, cv::WND_PROP_FULLSCREEN, cv::WINDOW_FULLSCREEN);
    
	cv::Mat img(cv::Size(100, 100), CV_8UC3, cv::Scalar(255, 0, 0));
	int tx = 0;
	int ty = 0;
	float angle = 0.0;
	loopTime = chrono::system_clock::now();
	while (1) {
		processStart = chrono::system_clock::now();
		
		// 1Loopの時間を取得
		double tmpLoopTime = static_cast<double>(chrono::duration_cast<chrono::microseconds>(processStart - loopTime).count() / 1000.0);
		loopTime = chrono::system_clock::now();
		
		cv::Mat dstImg(cv::Size(720, 720), CV_8UC3, cv::Scalar(0, 255, 255)); 
		cv::putText(dstImg, format("Target FPS:%.2f(%.2fms)",fps, mpf), cv::Point(10,30), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 0, 0), 2);
		cv::putText(dstImg, format(" Loop Time(ms):%.2f",tmpLoopTime), cv::Point(10,60), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 0, 0), 2);
		
		cv::Mat mat = affineMat(img, tx, ty, 1.0, angle);
		cv::warpAffine(img, dstImg, mat, dstImg.size(), cv::INTER_LINEAR, cv::BORDER_TRANSPARENT);
		
		cv::imshow(windowName, dstImg);

		tx++;
		ty++;
		angle++;

 		//ESCを押すと終了		
		if (cv::waitKey(1) == 27) break;
		
		
		// 処理時間を考慮して待つ
		processEnd = chrono::system_clock::now();
		processTime = static_cast<double>(chrono::duration_cast<chrono::microseconds>(processEnd - processStart).count() / 1000.0);
		waitTime = mpf - processTime;
		if(waitTime >= 1){
			cv::waitKey(waitTime);
		}
	}

	cv::destroyWindow(windowName);
    return 0;
}