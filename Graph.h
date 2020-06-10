#pragma once

#include <vector>

#include <opencv2/opencv.hpp>

class Label
{
public:
	Label() {};
	Label(cv::Point minP, cv::Point maxP)
		:minPos(minP), maxPos(maxP)
	{
	};
	~Label() {};
	void draw(cv::Mat img, int minVal, int maxVal)
	{
		drawLabel(img, minVal, minPos);
		drawLabel(img, maxVal, maxPos);
	}
	void draw(cv::Mat img, std::vector<std::vector<int>>* data)
	{

	}
private:
	cv::Point minPos;
	cv::Point maxPos;

	static const int USE_BLACK_COLOR = 10;

	void drawLabel(cv::Mat img, int val, cv::Point pos)
	{
		cv::putText(img, std::to_string(val), pos,
								cv::FONT_HERSHEY_SIMPLEX, 0.5,
								cv::Scalar::all(USE_BLACK_COLOR));
	}

};

class Chart
{
public:
	Chart():rect(cv::Rect()), minVal(0), maxVal(0) {};
	Chart(cv::Rect r, int minV, int maxV)
		:rect(r), minVal(minV), maxVal(maxV) {};
	~Chart() {};
	void draw(cv::Mat img, std::vector<int>* v,
							std::vector<std::vector<int>>* d)
	{
		cv::rectangle(img, rect, cv::Scalar::all(250), -1);

		drawAxis(img);
		drawPoints(img, v, d);
	}
private:
	cv::Rect rect;
	int maxVal;
	int minVal;

	static const int USE_BLACK_COLOR = 10;

	void drawAxis(cv::Mat img)
	{
		// draw X Axis.
		line(img, cv::Point(rect.x, rect.y + rect.height),
					cv::Point(rect.x + rect.width, rect.y + rect.height),
					cv::Scalar::all(USE_BLACK_COLOR));
		// draw Y Axis.
		line(img, cv::Point(rect.x, rect.y),
					cv::Point(rect.x, rect.y + rect.height),
					cv::Scalar::all(USE_BLACK_COLOR));
	}

	cv::Point adjustPos(int val, int max, int min,
											cv::Point maxPos, cv::Point minPos)
	{
		double d = max - min;
		double m = (val - min) / d;
		cv::Point vec = maxPos - minPos;

		return minPos + (vec * m);
	}

	void drawPoints(cv::Mat img, std::vector<int>* v,
										std::vector<std::vector<int>>* d)
	{
		cv::Point xAxisStart(rect.x, rect.y + rect.height);
		cv::Point xAxisEnd(rect.x + rect.width, rect.y + rect.height);
		cv::Point yAxisStart(rect.x, rect.y);
		cv::Point yAxisEnd(rect.x, rect.y + rect.height);

		for (int i = 0; i < v->size(); i++) {
			cv::Point p = adjustPos((*v)[i], maxVal, 0,
															yAxisStart, yAxisEnd);
			cv::Point d = adjustPos(i, v->size(), 0,
															xAxisEnd, xAxisStart);
			p.x = d.x;

			cv::putText(img, std::to_string((*v)[i]), p,
									cv::FONT_HERSHEY_SIMPLEX, 0.4,
									cv::Scalar(0, 0, 255));
			cv::circle(img, p, 3, cv::Scalar(0, 0, 255));
		}

		for (int i = 0; i < v->size() - 1; i++) {
			cv::Point p = adjustPos((*v)[i], maxVal, 0,
															yAxisStart, yAxisEnd);
			cv::Point d = adjustPos(i, v->size(), 0,
															xAxisEnd, xAxisStart);
			p.x = d.x;

			cv::Point np = adjustPos((*v)[i + 1], maxVal, 0,
															 yAxisStart, yAxisEnd);
			cv::Point nd = adjustPos(i + 1, v->size(), 0,
															 xAxisEnd, xAxisStart);
			np.x = nd.x;

			cv::line(img, p, np, cv::Scalar(0, 0, 255));
		}
	}

};

class Graph
{
public:
	Graph() {};
	~Graph() {};

	cv::Mat draw(int width, int height,
								std::vector<int>* v, std::vector<std::vector<int>>* d)
	{
		cv::Mat res(cv::Size(width, height), CV_8UC3,
									cv::Scalar::all(BACKGROUND_COLOR));
		
		if(res.empty()) { 
			std::cout << "•`‰æÝ’è‚ªŠÔˆá‚Á‚Ä‚¢‚Ü‚·(width / height)" << std::endl;
			return cv::Mat();
		}

		chart = Chart(cv::Rect(GRAPH_MARGIN, GRAPH_MARGIN,
														width - GRAPH_MARGIN * 2,
														height - GRAPH_MARGIN * 2), 0, VALUE_TENJO);
		label = Label(cv::Point(GRAPH_MARGIN, height - GRAPH_MARGIN),
									cv::Point(GRAPH_MARGIN, GRAPH_MARGIN));

		chart.draw(res, v, d);
		label.draw(res, 0, VALUE_TENJO);

		return res;
	}
private:
	// 10px margin.
	static const int GRAPH_MARGIN = 20;

	static const int BACKGROUND_COLOR = 230;
	static const int USE_BLACK_COLOR = 10;
	static const int DRAWAREA_COLOR = 250;

	static const int VALUE_TENJO = 800;

	Chart chart;
	Label label;
};