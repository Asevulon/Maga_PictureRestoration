#pragma once
#include<vector>
#include"fourea.h"
using namespace std;
typedef  pair<int, int>point;
class Restorer
{
private:
	vector<vector<double>>data;
	vector<vector<double>>H;
	vector<vector<double>>restored;
	vector<point>fixedid;
	vector<cmplx>fixedval;
protected:
	void Div(vector<vector<cmplx>>& target, vector<vector<cmplx>>& div, vector<point>& id, vector<cmplx>&val);
	double Differance(vector<vector<cmplx>>& left, vector<vector<cmplx>>& right);
	void CorrectReal(vector<vector<cmplx>>& target);
	void SetRestoredValue(vector<vector<cmplx>>& target, vector<point>& id, vector < cmplx>& val);
	void Transform(vector<vector<cmplx>>& source, vector<vector<double>>&target);
public:
	void SetData(vector<vector<double>>& source);
	void SetH(vector<vector<double>>& source);
	vector<vector<double>>GetRestored();
	void Restore();
	void Test();
};

