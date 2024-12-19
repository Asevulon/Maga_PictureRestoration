#include "pch.h"
#include "Restorer.h"

void Restorer::Div(vector<vector<cmplx>>& target, vector<vector<cmplx>>& div, vector<point>& id, vector<cmplx>& val)
{
	for (int i = 0; i < target.size(); i++)
	{
		for (int j = 0; j < target[i].size(); j++)
		{
			/*if (div[i][j].Abs() < 1e-20)
			{
				target[i][j] = cmplx(0, 0);
			}
			else
			{
				target[i][j] = target[i][j] / div[i][j];
				id.push_back(point(i, j));
				val.push_back(target[i][j]);
			}*/
			cmplx t = target[i][j] / div[i][j];
			if ((isnan(t.re)) || (isnan(t.im)))
			{
				target[i][j] = cmplx(0, 0);
			}
			else
			{
				target[i][j] = target[i][j] / div[i][j];
				id.push_back(point(i, j));
				val.push_back(target[i][j]);
			}
		}
	}
}

double Restorer::Differance(vector<vector<cmplx>>& left, vector<vector<cmplx>>& right)
{
	double res = 0;
	for (int i = 0; i < left.size(); i++)
	{
		for (int j = 0; j < left[i].size(); j++)
		{
			res += (left[i][j].re - right[i][j].re) * (left[i][j].re - right[i][j].re);
		}
	}
	return sqrt(res);
}

void Restorer::CorrectReal(vector<vector<cmplx>>& target)
{
	for (int i = 0; i < target.size(); i++)
	{
		for (int j = 0; j < target[i].size(); j++)
		{
			target[i][j].im = 0;
			if (target[i][j].re < 0)target[i][j].re = 0;
			else if (target[i][j].re > 255) target[i][j].re = 255;
		}
	}
}

void Restorer::SetRestoredValue(vector<vector<cmplx>>& target, vector<point>& id, vector<cmplx>& val)
{
	for (int i = 0; i < id.size(); i++)
	{
		target[id[i].first][id[i].second] = val[i];
	}
}

void Restorer::Transform(vector<vector<cmplx>>& source, vector<vector<double>>& target)
{
	if (source.empty())return;
	target.resize(source.size());
	for (int i = 0; i < target.size(); i++)
	{
		target[i].resize(source[i].size());
		for (int j = 0; j < target[i].size(); j++)
		{
			target[i][j] = source[i][j].re;
		}
	}
}

void Restorer::SetData(vector<vector<double>>& source)
{
	data = source;
}

void Restorer::SetH(vector<vector<double>>& source)
{
	H = source;
}

vector<vector<double>> Restorer::GetRestored()
{
	return restored;
}

void Restorer::Restore()
{
	foureier f;

	f.DoFourea(data);
	vector<vector<cmplx>>fdata = f.GetData();
	
	f.DoFourea(H);
	vector<vector<cmplx>>fH = f.GetData();

	fixedid.clear();
	fixedval.clear();
	Div(fdata, fH, fixedid,fixedval);

	vector<vector<cmplx>>prev = fdata;

	f.DoInversedFourea(fdata);
	fdata = f.GetData();

	CorrectReal(fdata);
	int count = 0;
	int cap = 2;
	do
	{
		prev = fdata;

		f.DoFourea(fdata);
		fdata = f.GetData();

		SetRestoredValue(fdata, fixedid, fixedval);

		f.DoInversedFourea(fdata);
		fdata = f.GetData();

		CorrectReal(fdata);
		count++;
	//} while (count < cap);
	} while (Differance(fdata, prev) > 1e-2);

	Transform(fdata, restored);

	for (int i = 0; i < restored.size(); i++)
	{
		for (int j = 0; j < restored[i].size(); j++)
		{
			if (restored[i][j] > 255)restored[i][j] = 0;
			else if (restored[i][j] < 0)restored[i][j] = 0;
		}
	}
}

void Restorer::Test()
{

}
