#include "pch.h"
#include "ImageBlurer.h"
#include<algorithm>
using namespace std;
#define pow2(x) ((x) * (x))

void ImageBlurer::GenerateH(std::vector<double>& target)
{
	//S = Sx
	int Hsize = N;
	if ((Hsize % 2) == 0)Hsize++;
	int mid = Hsize / 2;
	target.resize(Hsize);
	for (int i = 0; i < Hsize; i++)
	{
		target[i] = exp(-pow2(i - mid) / 2. / S / S) / sqrt(Pi2 * S * S);
	}

	double summ = 0;
	for (int i = 0; i < Hsize; i++)
	{
		summ += target[i];
	}

	for (int i = 0; i < Hsize; i++)
	{
		target[i] /= summ;
	}

}

void ImageBlurer::GenerateH(vector<vector<double>>& target)
{
	//S = Sx
	int Hsize = N;
	if ((Hsize % 2) == 0)Hsize++;
	int mid = Hsize / 2;
	target.resize(Hsize);
	
	double summ = 0;
	for (int i = 0; i < Hsize; i++)
	{
		target[i].resize(Hsize);
		for (int j = 0; j < Hsize; j++)
		{
			target[i][j] = exp(-(pow2(i - mid) + pow2(j - mid)) / 2. / S / S) / sqrt(Pi2 * S * S);
			summ += target[i][j];
		}
	}

	for (int i = 0; i < Hsize; i++)
	{
		for (int j = 0; j < Hsize; j++)
		{
			target[i][j] /= summ;
		}
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

void ImageBlurer::Convolute(std::vector<std::vector<double>>& target, std::vector<std::vector<double>>& source, std::vector<std::vector<double>>& h)
{
	double val = 0;
	int xsize = target[0].size();
	int ysize = target.size();
	int hsize = h.size();
	int mid = hsize / 2;
	double temp = 0;
	int idx = 0;
	int idy = 0;
	for (int i = 0; i < ysize; i++)
	{
		for (int j = 0; j < xsize; j++)
		{
			temp = 0;
			for (int k = 0; k < hsize; k++)
			{
				idy = i - mid + k;
				if (idy < 0)idy = 0;
				else if (idy >= ysize)idy = ysize - 1;
				for (int m = 0; m < hsize; m++)
				{
					idx = j - mid + m;
					if (idx < 0)idx = 0;
					else if (idx >= xsize)idx = xsize - 1;
					temp += source[idy][idx] * h[k][m];
				}
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

	double summ = 0;
	for (int i = 0; i < res.size(); i++)
	{
		for (int j = 0; j < res[i].size(); j++)
		{
			res[i][j] = exp(-(pow2(j - xmid) + pow2(i - ymid)) / 2. / S / S) / Pi2 / S / S;
			summ += res[i][j];
		}
	}

	for (int i = 0; i < res.size(); i++)
	{
		for (int j = 0; j < res[i].size(); j++)
		{
			res[i][j] /= summ;
		}
	}

	return res;
}

void ImageBlurer::Blur()
{
	H.clear();
	GenerateH(H2D);
	Convolution = data;
	Convolute(Convolution, data, H2D);
}

void ImageBlurer::SetGauss(int n, double s)
{
	N = n;
	S = s;
}
