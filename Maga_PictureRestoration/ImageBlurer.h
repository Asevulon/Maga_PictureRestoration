#pragma once
#include<vector>
using namespace std;
const double Pi2 = 8 * atan(1);
class ImageBlurer
{
private:
	std::vector<std::vector<double>> data;
	std::vector<std::vector<double>> Convolution;
	vector<double> H;

	double A = 0;
	double x0 = 0;
	double y0 = 0;
	double Sx = 0;
	double Sy = 0;


protected:
	void GenerateH(std::vector<double>& target);
	void Convolute(std::vector<std::vector<double>>& target, std::vector<std::vector<double>>& source, std::vector<double>& H);
public:
	void SetData(std::vector<std::vector<double>>& source);
	std::vector<std::vector<double>>GetData();
	std::vector<std::vector<double>>GetConvolution();
	std::vector<std::vector<double>>GetH();
	void Blur();
	void SetGauss(double pa, double px0, double py0, double psx, double psy);
};

