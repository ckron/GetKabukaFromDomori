#include <string>

#include "opencv2/opencv.hpp"

cv::Mat keys[10];

int pickupKabukaInImage(cv::Mat src)
{

	return 0;
}

void loadKeys()
{
	for(int i=0; i<10; i++) {
		std::string filename = "./img/00_key/" + std::to_string(i) + ".jpg";

		//std::cout << filename << std::endl;

		keys[i] = cv::imread(filename.c_str());
	}
}

std::vector<cv::Point> find(cv::Mat src, cv::Mat tempL, float threshold)
{
	cv::Mat res;
	cv::matchTemplate(src, tempL, res, cv::TM_CCOEFF_NORMED);

	//std::cout << res.rows << std::endl;
	//std::cout << res.cols << std::endl;
	//std::cout << res.channels() << std::endl;
	//std::cout << res.depth() << std::endl;

	std::vector<cv::Point> maxLocs;

	for (int y = 0; y < res.rows; y++) {
		for (int x = 0; x < res.cols; x++) {
			uchar* uctemp = &res.data[y * res.cols * 4 + x * 4];
			float* ftemp = (float*)(uctemp);
			if (*ftemp > threshold) {
				maxLocs.push_back(cv::Point(x, y));
			}
		}
	}

	return maxLocs;
}

int main(int argc, char* argv[])
{
	cv::Mat mat = cv::imread("img/sample/domori.jpg");
	loadKeys();
	
	std::vector<cv::Point> locs = find(mat, keys[0], 0.95);

	std::cout << locs.size() << std::endl;
	for(auto ml : locs) {
		cv::rectangle(mat, cv::Rect(ml.x, ml.y, keys[0].cols, keys[0].rows),
										cv::Scalar(0, 0, 255));
	}

	cv::imshow("image", mat);
	
	cv::waitKey();

	return 0;
}