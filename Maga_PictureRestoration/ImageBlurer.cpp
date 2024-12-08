#include "pch.h"
#include "ImageBlurer.h"
#include<algorithm>
using namespace std;
#define pow2(x) ((x) * (x))

void ImageBlurer::GenerateH(std::vector<double>& target)
{
	//S = Sx
	int Hsize = 6 * Sx;
	if ((Hsize % 2) == 0)Hsize++;
	int mid = Hsize / 2;
	target.resize(Hsize);
	for (int i = 0; i < Hsize; i++)
	{
		target[i] = exp(-pow2(i - mid) / 2. / Sx / Sx) / sqrt(Pi2 * Sx * Sx);
	}
}

void ImageBlurer::Convolute(std::vector<std::vector<double>>& target, std::vector<std::vector<double>>& source, std::vector<double>& h)
{
	double val = 0;
	int xsize = target[0].size();
	int ysize = target.size();
	int hsize = h.size();
	int mid = hsize / 2;
	double temp = 0;
	int id = 0;
	for (int i = 0; i < ysize; i++)
	{
		for (int j = 0; j < xsize; j++)
		{
			temp = 0;
			for (int k = 0; k < hsize; k++)
			{
				id = j - mid + k;
				if (id < 0)id = 0;
				else if (id >= xsize)id = xsize - 1;
				temp += source[i][id] * h[k];
			}
			target[i][j] = temp;
		}
	}
	vector<vector<double>>res = target;

	for (int j = 0; j < xsize; j++)
	{
		for (int i = 0; i < ysize; i++)
		{
			temp = 0;
			for (int k = 0; k < hsize; k++)
			{
				id = i - mid + k;
				if (id < 0)id = 0;
				else if (id >= ysize)id = ysize - 1;
				temp += res[id][j] * h[k];
			}
			target[i][j] = temp;
		}
	}
}

void ImageBlurer::SetData(std::vector<std::vector<double>>& source)
{
	data = source;
}

std::vector<std::vector<double>> ImageBlurer::GetData()
{
	return data;
}

std::vector<std::vector<double>> ImageBlurer::GetConvolution()
{
	return Convolution;
}

vector<vector<double>> ImageBlurer::GetH()
{
	std::vector<std::vector<double>>res = data;
	if (res.empty())return res;

	double xmid = res[0].size() / 2.;
	double ymid = res.size() / 2.;

	for (int i = 0; i < res.size(); i++)
	{
		for (int j = 0; j < res[i].size(); j++)
		{
			res[i][j] = exp(-(pow2(j - xmid) + pow2(i - ymid)) / 2. / Sx / Sx) / Pi2 / Sx / Sx;
		}
	}
	return res;
}

void ImageBlurer::Blur()
{
	H.clear();
	GenerateH(H);
	Convolution = data;
	Convolute(Convolution, data, H);
}

void ImageBlurer::SetGauss(double pa, double px0, double py0, double psx, double psy)
{
	A = pa;
	x0 = px0;
	y0 = py0;
	Sx = psx;
	Sy = psy;
}
