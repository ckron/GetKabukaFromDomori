#pragma once

#include <vector>

#include <opencv2/opencv.hpp>

class Graph
{
public:
	void setData(std::vector<int>* v, std::vector<char[3]>* d)
	{
		values = v;
		dates = d;
	}

	cv::Mat draw(int width, int height)
	{
		cv::Mat res(cv::Size(width, height), CV_8UC3, cv::Scalar::all(BACKGROUND_COLOR));
		
		if(res.empty()) { 
			std::cout << "•`‰æÝ’è‚ªŠÔˆá‚Á‚Ä‚¢‚Ü‚·(width / height)" << std::endl;
			return cv::Mat();
		}

		drawAxis(res);
		drawPoints(res);
		drawLabel(res);

		return res;
	}
private:
	std::vector<int>* values;
	std::vector<char[3]>* dates;

	// 10px margin.
	static const int GRAPH_MARGIN = 20;

	static const int BACKGROUND_COLOR = 230;
	static const int USE_BLACK_COLOR = 10;
	static const int DRAWAREA_COLOR = 250;

	static const int VALUE_TENJO = 800;

	void drawLabel(cv::Mat img)
	{
		cv::Point yAxisTopLabel(GRAPH_MARGIN - 10, GRAPH_MARGIN);
		cv::Point yAxisDownLabel(GRAPH_MARGIN - 10, img.rows - GRAPH_MARGIN);

		cv::putText(img, std::to_string(VALUE_TENJO), cv::Point(yAxisTopLabel),
								cv::FONT_HERSHEY_SIMPLEX, 0.5,
								cv::Scalar::all(USE_BLACK_COLOR));
		cv::putText(img, std::to_string(0), cv::Point(yAxisDownLabel),
								cv::FONT_HERSHEY_SIMPLEX, 0.5,
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

	void drawPoints(cv::Mat img)
	{
		cv::Point xAxisStart(GRAPH_MARGIN, img.rows - GRAPH_MARGIN);
		cv::Point xAxisEnd(img.cols - GRAPH_MARGIN, xAxisStart.y);
		cv::Point yAxisStart(GRAPH_MARGIN, GRAPH_MARGIN);
		cv::Point yAxisEnd(yAxisStart.x, img.rows - GRAPH_MARGIN);
		
		for(int i=0; i<values->size(); i++) {
			cv::Point p = adjustPos((*values)[i], VALUE_TENJO, 0,
																yAxisStart, yAxisEnd);
			p.x = xAxisStart.x + 100 + 10 * i;

			cv::circle(img, p, 3, cv::Scalar(0, 0, 255));
		}

		for (int i=0; i<values->size()-1; i++) {
			cv::Point p = adjustPos((*values)[i], VALUE_TENJO, 0,
															yAxisStart, yAxisEnd);
			p.x = xAxisStart.x + 100 + 10 * i;

			cv::Point np = adjustPos((*values)[i+1], VALUE_TENJO, 0,
															yAxisStart, yAxisEnd);
			np.x = xAxisStart.x + 100 + 10 * (i+1);

			cv::line(img, p, np, cv::Scalar(0, 0, 255));
		}
	}

	void drawAxis(cv::Mat img)
	{
		cv::Point xAxisStart(GRAPH_MARGIN, img.rows - GRAPH_MARGIN);
		cv::Point xAxisEnd(img.cols - GRAPH_MARGIN, xAxisStart.y);
		cv::Point yAxisStart(GRAPH_MARGIN, GRAPH_MARGIN);
		cv::Point yAxisEnd(yAxisStart.x, img.rows - GRAPH_MARGIN);

		// draw X Axis.
		line(img, xAxisStart, xAxisEnd, cv::Scalar::all(USE_BLACK_COLOR));
		// draw Y Axis.
		line(img, yAxisStart, yAxisEnd, cv::Scalar::all(USE_BLACK_COLOR));
	}
};