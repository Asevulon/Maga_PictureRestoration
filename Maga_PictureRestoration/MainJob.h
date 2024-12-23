#pragma once
#include"GaussPreloader.h"
#include"ImageConverter.h"
#include"RangeStretcher.h"
#include"fourea.h"
#include"ImageBlurer.h"
#include"Restorer.h"
class MainJob
{
private:
	CString targetpath;
	bool IsPicture = false;
	bool pathinit = false;

	std::vector<std::vector<double>> data;
	std::vector<std::vector<double>> Puredata;
	std::vector<std::vector<double>> StretchedData;
	std::vector<std::vector<double>> BluredData;
	std::vector<std::vector<double>> RestoredData;
	std::vector<std::vector<double>> H;
	std::pair<int, int> OriginalSourceSize;
	double EstimateSource = 0;
	double EstimateBlured = 0;
	double EstimatePurified = 0;
	double EstimatePurifiedSwap = 0;

	int N = 0;
	double S = 0;
	int Cutter = 0;
protected:
	void LoadPicture();
	void LoadGauss();
	void StretchImage();
	double CalcEnergy(std::vector<std::vector<double>>& source);
	double CalcEnergy(std::vector<std::vector<cmplx>>& source);
	std::vector<std::vector<double>>GetReal(std::vector<std::vector<cmplx>>& source);
	std::vector<std::vector<double>>GetAbs(std::vector<std::vector<cmplx>>& source);
	void RestoreRange();
	void Estimate();
	double Estimate(vector<vector<double>>& target);
	void BlurData();
	void RestoreData();
	vector<vector<double>> CutImage(vector<vector<double>>& source);
public:
	void SetPath(CString& path, bool ispicture);
	void main();
	std::vector<std::vector<double>> GetData();
	std::vector<std::vector<double>> GetPureData();
	std::vector<std::vector<double>> GetStretchedData();
	std::vector<std::vector<double>> GetRestoredData();
	std::vector<std::vector<double>> GetBluredData();
	std::vector<std::vector<double>> GetH();
	void SetGauss(int n, double s);
	void SetCutter(int val);
	double GetEstSource();
	double GetEstBlured();
	double GetEstPurified();
	double GetEstPurifiedSwap();
	void SwapQuadrants();
};

