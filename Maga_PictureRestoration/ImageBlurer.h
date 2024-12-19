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
	vector<vector<double>> H2D;

	double S = 0;
	int N = 0;

protected:
	void GenerateH(std::vector<double>& target);
	void Convolute(std::vector<std::vector<double>>& target, std::vector<std::vector<double>>& source, std::vector<double>& H);
	void GenerateH(vector<vector<double>>& target);
	void Convolute(std::vector<std::vector<double>>& target, std::vector<std::vector<double>>& source, std::vector<std::vector<double>>& H);
public:
	void SetData(std::vector<std::vector<double>>& source);
	std::vector<std::vector<double>>GetData();
	std::vector<std::vector<double>>GetConvolution();
	std::vector<std::vector<double>>GetH();
	void Blur();
	void SetGauss(int n, double s);
};

