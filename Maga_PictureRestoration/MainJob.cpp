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
	double Epure = CalcEnergy(StretchedData);
	
	double Emist = 0;
	double Enoise = 0;
	for (int i = 0; i < StretchedData.size(); i++)
	{
		for (int j = 0; j < StretchedData[i].size(); j++)
		{
			Emist = pow2(StretchedData[i][j] - RestoredData[i][j]);
			Enoise = pow2(StretchedData[i][j] - data[i][j]);
		}
	}

	Emist /= Epure;
	Enoise /= Epure;

	EstimateSourceNoise = sqrt(Enoise);
	EstimateSourcePurified = sqrt(Emist);
}

void MainJob::BlurData()
{
	ImageBlurer ib;
	ib.SetData(StretchedData);
	ib.SetGauss(A, x0, y0, Sx, Sy);
	ib.Blur();
	BluredData = ib.GetConvolution();
	H = ib.GetH();

	Restorer rest;
	rest.SetData(BluredData);
	rest.SetH(H);
	rest.Restore();
	RestoredData = rest.GetRestored();
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

	//Estimate();
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
	return StretchedData;
}

std::vector<std::vector<double>> MainJob::GetRestoredData()
{
	return RestoredData;
}

std::vector<std::vector<double>> MainJob::GetBluredData()
{
	return BluredData;
}

std::vector<std::vector<double>> MainJob::GetH()
{
	return H;
}

void MainJob::SetGauss(double pa, double px0, double py0, double psx, double psy)
{
	A = pa;
	x0 = px0;
	y0 = py0;
	Sx = psx;
	Sy = psy;
}


double MainJob::GetMistake()
{
	return EstimateSourcePurified;
}

double MainJob::GetDifferance()
{
	return EstimateSourceNoise;
}
