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

/*限制读入网页个数，-1为不限制*/
unsigned NR_Page_Limit = 1500;

const double eps = 1e-2;
const double beta = 0.8;

#define fabs(x) ((x)>0?(x):-(x))

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

	//限制读入个数
	while (NR_Page_Limit&&fin >> temp)
	{
		mp[temp] = NR_PAGE++;
		NR_Page_Limit--;
	}
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
void init_link(double beta)
{

	for (int father_index = 0; father_index < NR_PAGE; father_index ++)
	{
		cout << "正在读入第" << father_index << "个文件" << endl;
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
				M[it->second][father_index] += 10000*beta;
				nr_children++;
			}
		}
		//如果是孤立结点，就把他修正为10000.0*beta / NR_PAGE
		if (nr_children == 0)
			for (int i = 0; i < NR_PAGE; i++)
				M[i][father_index] = 10000.0*beta / NR_PAGE;
		else 
		{
			for (int i = 0; i < NR_PAGE; i++)
				M[i][father_index] /= nr_children;
		}
		fin.close();
	}

	//beta 系数
	for (int i = 0; i < NR_PAGE; i++)
		for (int j = 0; j < NR_PAGE; j++)
			M[i][j] += 10000.0*(1 - beta) / NR_PAGE;

	//A-E
	for (int i = 0; i < NR_PAGE; i++)
		M[i][i] -= 10000;
}



//************************************
// Method:    gauss_cpivot
// FullName:  gauss_cpivot
// Access:    public 
// Returns:   int
// Qualifier:高斯消元法列主元gauss消去求解a[][]x[] = b[]返回是否有唯一解, 若有解在b[]中
// Parameter: vector<vector<double>> a
// Parameter: vector<double> & b
//************************************
int gauss_cpivot(vector<vector<double>> a, vector<double>&b) {
	int n = NR_PAGE;
	int i, j, k, row;
	double maxp, t;
	for (k = 0; k < n; k++) {
		cout << ".";
		for (maxp = 0, i = k; i<n; i++)
			if (fabs(a[i][k])>fabs(maxp))
				maxp = a[row = i][k];
		if (fabs(maxp) < eps)
			return 0;
		if (row != k) {
			for (j = k; j < n; j++)
				t = a[k][j], a[k][j] = a[row][j], a[row][j] = t;
			t = b[k], b[k] = b[row], b[row] = t;
		}
		for (j = k + 1; j < n; j++) {
			a[k][j] /= maxp;
			for (i = k + 1; i < n; i++)
				a[i][j] -= a[i][k] * a[k][j];
		}
		b[k] /= maxp;
		for (i = k + 1; i < n; i++)
			b[i] -= b[k] * a[i][k];
	}
	for (i = n - 1; i >= 0; i--)
		for (j = i + 1; j < n; j++)
			b[i] -= a[i][j] * b[j];
	return 1;
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
vector<double> pr_caculate()
{
	//使解向量之和为10000
	fill(M[0].begin(), M[0].end(), 1);

	//放解的数组
	vector<double> result(NR_PAGE);

	//初始化解向量
	result[0] = 10000;
	fill(result.begin() + 1, result.end(), 0);

	//高斯
	gauss_cpivot(M, result);
	return result;


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
/*vector<double> pr_caculate(double init, double beta)
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
*/

int main()
{
	init_listfile();
	cout << "初始化文件列表成功..."<<endl;
	init_link(0.85);
	cout << "初始化链接关系成功..." << endl;
	auto re=pr_caculate();

	ofstream fout(RESULTFILE);
#ifdef  DEBUG
	assert(fout);
#endif
	for_each(re.begin(), re.end(), [&fout](double rank) {fout << rank << endl; });
	fout.close();
	cout << "计算成功..." << endl;
	return 0;
}
