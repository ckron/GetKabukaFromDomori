#pragma once

#include <opencv2/opencv.hpp>

class KabukaDetector
{
public:
	KabukaDetector() {
		loadKeys();
	};
	~KabukaDetector() {};
	// kabuka 3 keta made.
	int pickupKabuka(cv::Mat img)
	{
		std::vector<cv::Point> finds[MAX_KEYS];
		std::vector<int> poss;
		std::vector<int> vals;

		bool isFind = false;

		for (int i = 0; i < MAX_KEYS; i++) {
			finds[i] = find(img, keys[i], 0.95);
		}

		for (int i = 0; i < MAX_KEYS; i++) {
			if (finds[i].size() == 0) { continue; }

			isFind = true;
			for (int p = 0; p < finds[i].size(); p++) {
				poss.push_back(finds[i][p].x);
				vals.push_back(i);
			}
		}

		if(!isFind) { return 0; }

		sortKabuketa(&poss, &vals);

		int kabuka = 0;
		for (int i = 0; i < vals.size(); i++) {
			kabuka += vals[i] * (pow(10, i));
		}
		return kabuka;
	}

private:
	static const int MAX_KEYS = 10;
	cv::Mat keys[MAX_KEYS];
	void sortKabuketa(std::vector<int>* poss, std::vector<int>* vals)
	{
		for (int j = 0; j < poss->size() - 1; j++) {
			for (int i = 0; i < poss->size() - 1; i++) {
				//std::cout << std::to_string(j) << std::endl;
				//std::cout << std::to_string((*poss)[i]) << ", " << std::to_string((*poss)[i+1]) << std::endl;
				//std::cout << std::to_string((*vals)[i]) << ", " << std::to_string((*vals)[i + 1]) << std::endl;
				int n = i + 1;
				if ((*poss)[i] < (*poss)[n]) {
					int tempP = (*poss)[i];
					int tempV = (*vals)[i];
					(*poss)[i] = (*poss)[n];
					(*vals)[i] = (*vals)[n];
					(*poss)[n] = tempP;
					(*vals)[n] = tempV;
				}
			}
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
					bool isExist = false;
					for (auto ml : maxLocs) {
						// ç°Ç‹Ç≈Ç…å©Ç¬Ç©Ç¡ÇΩèÍèäÇ©ÇÁÇ†ÇÈíˆìxó£ÇÍÇ»Ç¢Ç∆åüèoàµÇ¢Ç…ÇµÇ»Ç¢.
						int d = pow(y - ml.y, 2) + pow(x - ml.x, 2);
						if (d < 50) {
							isExist = true;
						}
					}

					if (!isExist) {
						maxLocs.push_back(cv::Point(x, y));
					}
				}
			}
		}

		return maxLocs;
	}

	void loadKeys()
	{
		for (int i = 0; i < MAX_KEYS; i++) {
			std::string filename = "./img/00_key/" + std::to_string(i) + ".jpg";

			//std::cout << filename << std::endl;

			keys[i] = cv::imread(filename.c_str());
		}
	}
};

