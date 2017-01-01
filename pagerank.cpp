#include <cmath>
#include <string>
#include <vector>
#include <numeric>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <assert.h>
#include <map>
using namespace std;

#define DEBUG

const string FILEDIR= "D:/计科/大二/数据结构/大作业—搜索引擎/Spider-master/nju";
const string LISTFILE = FILEDIR + "/pagelist.txt";
const string RESULTFILE = FILEDIR + "/pagerank.txt";

const double eps = 1e-6;
const double beta = 0.8;



int NR_PAGE=0;
map<string, int> mp;
vector<vector<double>> M;

//************************************
// Method:    init_listfile
// FullName:  init_listfile
// Access:    public 
// Returns:   void
// Qualifier:导入网页列表，创建map
//************************************
void init_listfile()
{
	ifstream fin(LISTFILE);
	assert(fin);
	string temp;
	while (fin >> temp)
		mp[temp] = NR_PAGE++;
#ifdef DEBUG
	cout << NR_PAGE << endl;
#endif
	fin.close();

// init Matrix vector
	M.resize(NR_PAGE);
	for_each(M.begin(), M.end(), [&](std::vector<double>& vec) { vec.resize(NR_PAGE); });
}

string int2str(int temp)
{
	stringstream ss;
	string num_str;
	ss << temp;
	ss >> num_str;
	return num_str;
}



vector<double> vec_operate(const vector<double>& v1, const vector<double>& v2, double fun(double,double))
{
	vector<double> resultant; // stores the results
	for (int i = 0; i < v1.size(); ++i)
	{
		resultant.push_back(fun(v1[i], v2[i])); // stores the results of todo() called on the i'th element on each vector
	}

	return resultant;
}


//************************************
// Method:    sqrt_of_pinfangsum
// FullName:  sqrt_of_pinfangsum
// Access:    public 
// Returns:   double
// Qualifier: 平方和的sqrt
// Parameter: const vector<double> & v1
//************************************
double sqrt_of_pinfangsum(const vector<double>& v1)
{

	double running_total = 0.0;
	for_each(v1.begin(), v1.end(), [&](double val) {running_total += (val * val); });
	return sqrt(running_total); // returns the L2 Norm of the vector v1, by sqrt(sum of squares)
}



//************************************
// Method:    init_childrenlink
// FullName:  init_childrenlink
// Access:    public 
// Returns:   void
// Qualifier: 导入链接关系
//************************************
void init_link()
{

	for (int father_index = 0; father_index < NR_PAGE; father_index ++)
	{
		string linkfile = FILEDIR + "/childrenlink/" + int2str(father_index) + ".txt";
		ifstream fin(linkfile);
#ifdef DEBUG	
		assert(fin);
#endif
		string child;
		int nr_children=0;
		while (fin>>child)
		{
			map<string, int>::iterator it = mp.find(child);
			if (it != mp.end()) {
				M[it->second][father_index] = 1.0;
				nr_children++;
			}
		}
		for (int i = 0; i < NR_PAGE; i++)
			M[i][father_index] /= nr_children;
		fin.close();
	}

}


//************************************
// Method:    pr_caculate
// FullName:  pr_caculate
// Access:    public 
// Returns:   std::vector<double>
// Qualifier:计算PR值
// Parameter: double eps 
// Parameter: double init  default=1/NR_page
// Parameter: double beta
//************************************
vector<double> pr_caculate(double init, double beta)
{
	double diff = 1 << 10; // make absurdly high for now for the while loop

	vector<double> re_PR(NR_PAGE); // page-rank vector


	fill(re_PR.begin(), re_PR.end(), init);


	while (diff > eps)
	{
		vector<double> temp_vec(NR_PAGE);
		for (int i = 0; i < NR_PAGE; i++)
		{
			double line_re = 0;
			for (int j = 0; j < NR_PAGE; j++)
			{
				line_re += M[i][j] * re_PR[j];
			}
			temp_vec[i] = beta*line_re+(1 - beta) / NR_PAGE;
		}
		diff = sqrt_of_pinfangsum(vec_operate(temp_vec, re_PR, [](double i, double j) {return i - j; }));
		re_PR = temp_vec;
	}
	return re_PR;

}


int main()
{
	init_listfile();
	cout << "初始化文件列表成功..."<<endl;
	init_link();
	cout << "初始化链接关系成功..." << endl;
	auto re=pr_caculate(1.0 / NR_PAGE, beta);

	ofstream fout(RESULTFILE);
#ifdef  DEBUG
	assert(fout);
#endif
	for_each(re.begin(), re.end(), [&fout](double rank) {fout << rank << endl; });
	fout.close();
	cout << "计算成功..." << endl;
	return 0;
}
