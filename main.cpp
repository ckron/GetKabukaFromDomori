#include <filesystem>
#include <string>

#include "opencv2/opencv.hpp"

cv::Mat keys[10];

const int MAX_KEYS = 10;

int pickupKabukaInImage(cv::Mat src);
void loadKeys();
std::vector<cv::Point> find(cv::Mat src, cv::Mat tempL, float threshold);
void sortKabuketa(std::vector<int>* poss, std::vector<int>* vals);
cv::Mat adjustImage900(cv::Mat src);

void sortKabuketa(std::vector<int>* poss, std::vector<int>* vals)
{
	for(int j=0; j<poss->size(); j++) {
		for(int i=0; i<poss->size()-1; i++) {
			//std::cout << std::to_string(j) << std::endl;
			//std::cout << std::to_string((*poss)[i]) << ", " << std::to_string((*poss)[i+1]) << std::endl;
			//std::cout << std::to_string((*vals)[i]) << ", " << std::to_string((*vals)[i + 1]) << std::endl;
			if((*poss)[i] < (*poss)[i+1]) {
				int tempP = (*poss)[i];
				int tempV = (*vals)[i];
				(*poss)[i] = (*poss)[i+1];
				(*vals)[i] = (*vals)[i+1];
				(*poss)[i+1] = tempP;
				(*vals)[i+1] = tempV;
			}
		}
	}
}

// kabuka 3 keta only.
int pickupKabukaInImage(cv::Mat src)
{
	std::vector<cv::Point> finds[MAX_KEYS];
	std::vector<int> poss;
	std::vector<int> vals;

	for(int i=0; i<MAX_KEYS; i++) {
		finds[i] = find(src, keys[i], 0.93);
	}

	for(int i=0; i<MAX_KEYS; i++) {
		if(finds[i].size() == 0) { continue; }

		for(int p=0; p<finds[i].size(); p++) {
			poss.push_back(finds[i][p].x);
			vals.push_back(i);
		}
	}

	sortKabuketa(&poss, &vals);

	int kabuka = 0;
	for(int i=0; i<3; i++) {
		kabuka += vals[i] * (pow(10, i));
	}

	return kabuka;
}

void loadKeys()
{
	for(int i=0; i<MAX_KEYS; i++) {
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

cv::Mat adjustImage900(cv::Mat src)
{
	cv::Mat dst;
	cv::resize(src, dst, cv::Size(900, 506));

	return dst;
}

int main(int argc, char* argv[])
{
	cv::Mat src = cv::imread("img/sample/domori.jpg");

	if(src.cols != 900) {
		src = adjustImage900(src);
	}

	loadKeys();

	int kabuka = pickupKabukaInImage(src);
	
	std::cout << kabuka << std::endl;
	cv::imshow("image", src);
	
	cv::waitKey();

	return 0;
}