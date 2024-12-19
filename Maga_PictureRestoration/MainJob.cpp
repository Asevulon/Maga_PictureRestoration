#include "pch.h"
#include "MainJob.h"
using namespace std;

#define pow2(x) (x) * (x)

void MainJob::LoadPicture()
{
	ImageConverter conv;
	conv.SetImagePath(targetpath);
	conv.Convert();
	data = conv.GetData();
}

void MainJob::LoadGauss()
{
	GaussPreloader gauss;
	gauss.SetPath(targetpath);
	gauss.Load();
	data = gauss.GetData();
}

void MainJob::StretchImage()
{
	RangeStretcher rstr;
	rstr.ConvertRange(data);
	data = rstr.GetStretched();
	OriginalSourceSize = rstr.GetOriginalSize();
}

double MainJob::CalcEnergy(std::vector<std::vector<cmplx>>& source)
{
	double res = 0;
	for (int i = 0; i < source.size(); i++)
	{
		for (int j = 0; j < source[i].size(); j++)
		{
			res += source[i][j].im * source[i][j].im + source[i][j].re * source[i][j].re;
		}
	}
	return res;
}
double MainJob::CalcEnergy(std::vector<std::vector<double>>& source)
{
	double res = 0;
	for (int i = 0; i < source.size(); i++)
	{
		for (int j = 0; j < source[i].size(); j++)
		{
			res += pow2(source[i][j]);
		}
	}
	return res;
}

std::vector<std::vector<double>> MainJob::GetReal(std::vector<std::vector<cmplx>>& source)
{
	std::vector<std::vector<double>> res;
	res.resize(source.size());
	for (int i = 0; i < res.size(); i++)
	{
		res[i].resize(source[i].size());
		for (int j = 0; j < res[i].size(); j++)
		{
			res[i][j] = source[i][j].re;
		}
	}
	return res;
}

std::vector<std::vector<double>> MainJob::GetAbs(std::vector<std::vector<cmplx>>& source)
{
	std::vector<std::vector<double>> res;
	res.resize(source.size());
	for (int i = 0; i < res.size(); i++)
	{
		res[i].resize(source[i].size());
		for (int j = 0; j < res[i].size(); j++)
		{
			res[i][j] = sqrt(pow2(source[i][j].re) + pow2(source[i][j].im));
		}
	}
	return res;
}

void MainJob::RestoreRange()
{
	RangeStretcher rstr;
	rstr.SetTargetSize(OriginalSourceSize);
	rstr.ConvertRange(RestoredData);
	RestoredData = rstr.GetStretched();
}

void MainJob::Estimate()
{
	EstimateSource = Estimate(StretchedData);
	EstimateBlured = Estimate(BluredData);
	EstimatePurified = Estimate(RestoredData);
	SwapQuadrants();
	EstimatePurifiedSwap = Estimate(RestoredData);
	SwapQuadrants();
}

double MainJob::Estimate(vector<vector<double>>& target)
{
	double res = 0;
	if (target.empty())return res;
	double max = 0;
	double temp = 0;
	for (int i = 1 + Cutter; i < target.size() - 1 - Cutter; i++)
	{
		for (int j =  1; j < target[i].size() - 1; j++)
		{
			max = (max < target[i][j]) ? target[i][j] : max;
			temp = fabs(target[i][j] - target[i - 1][j]);
			temp += fabs(target[i][j] - target[i][j - 1]);
			res += temp * temp;
		}
	}
	res /= target.size() - 1;
	res /= target[0].size() - 1;
	res /= max / 2.;

	return res;
}

void MainJob::BlurData()
{
	ImageBlurer ib;
	ib.SetData(StretchedData);
	ib.SetGauss(N, S);
	ib.Blur();
	BluredData = ib.GetConvolution();
	H = ib.GetH();
}

void MainJob::RestoreData()
{
	Restorer rest;
	rest.SetData(BluredData);
	rest.SetH(H);
	rest.Restore();
	RestoredData = rest.GetRestored();
}

vector<vector<double>> MainJob::CutImage(vector<vector<double>>& source)
{
	if (Cutter == 0)return source;
	vector<vector<double>>res(source.size() - Cutter * 2);
	for (int i = 0; i < res.size(); i++)
	{
		res[i].resize(source[i].size() - Cutter * 2);
		for (int j = 0; j < res[i].size(); j++)
		{
			res[i][j] = source[i + Cutter][j + Cutter];
		}
	}
	return res;
}

void MainJob::SetPath(CString& path, bool ispicture)
{
	if (path.IsEmpty())return;
	targetpath = path;
	IsPicture = ispicture;
	pathinit = false;
}

void MainJob::main()
{
	if (IsPicture)
	{
		LoadPicture();
	}
	else
	{
		LoadGauss();
	}
	Puredata = data;
	StretchImage();
	StretchedData = data;
	BlurData();
	RestoreData();
	

	Estimate();
	//RestoreRange();
}

std::vector<std::vector<double>> MainJob::GetData()
{
	return data;
}

std::vector<std::vector<double>> MainJob::GetPureData()
{
	return Puredata;
}

std::vector<std::vector<double>> MainJob::GetStretchedData()
{
	return CutImage(StretchedData);
}

std::vector<std::vector<double>> MainJob::GetRestoredData()
{
	return CutImage(RestoredData);
}

std::vector<std::vector<double>> MainJob::GetBluredData()
{
	return CutImage(BluredData);
}

std::vector<std::vector<double>> MainJob::GetH()
{
	return H;
}

void MainJob::SetGauss(int n, double s)
{
	N = n;
	S = s;
}

void MainJob::SetCutter(int val)
{
	Cutter = val;
}

double MainJob::GetEstSource()
{
	return EstimateSource;
}

double MainJob::GetEstBlured()
{
	return EstimateBlured;
}

double MainJob::GetEstPurified()
{
	return EstimatePurified;
}

double MainJob::GetEstPurifiedSwap()
{
	return EstimatePurifiedSwap;
}

void MainJob::SwapQuadrants()
{
	if (RestoredData.empty())return;

	int xsize = RestoredData[0].size();
	int ysize = RestoredData.size();
	int xh = xsize / 2;
	int yh = ysize / 2;
	double temp = 0;

	for (int i = 0; i < yh; i++)
	{
		for (int j = 0; j < xh; j++)
		{
			temp = RestoredData[i][j];
			RestoredData[i][j] = RestoredData[i + yh][j + xh];
			RestoredData[i + yh][j + xh] = temp;

			temp = RestoredData[i][j + xh];
			RestoredData[i][j + xh] = RestoredData[i + yh][j];
			RestoredData[i + yh][j] = temp;
		}
	}
	
}
