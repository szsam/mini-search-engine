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
struct Matrixelem;


/************************************************************************/
/* 一些参数设定                                                         */
/************************************************************************/
const string FILEDIR = "D:/计科/大二/数据结构/大作业—搜索引擎/Spider-master/nju";
const string LISTFILE = FILEDIR + "/pagelist.txt";
const string RESULTFILE = FILEDIR + "/pagerank.txt";

/*限制读入网页个数，-1为不限制*/
unsigned NR_Page_Limit = -1;

const double eps = 1e-6;
const double beta = 0.85;
const int amplification_factor = 10000;
/************************************************************************/

#define fabs(x) ((x)>0?(x):-(x))

/*some global variable*/
int NR_PAGE = 0;
map<string, int> mp;
vector<Matrixelem> M;

struct Matrixelem
{
	int row, col;
	double value;
	Matrixelem()
	{
		this->row = -1;
		this->col= -1;
		this->value = 0;
	}
	Matrixelem(int a, int b, double value) :row(a), col(b), value(value) {}

	bool operator == (const Matrixelem & right)
	{
		if (this->col == right.col&&this->row == right.row&&this->value == right.value)
			return 1;
		else
			return 0;
	}
};


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
}

string int2str(int temp)
{
	stringstream ss;
	string num_str;
	ss << temp;
	ss >> num_str;
	return num_str;
}



//************************************
// Method:    vec_operate
// FullName:  vec_operate
// Access:    public 
// Returns:   std::vector<double>
// Qualifier: 向量间运算
// Parameter: const vector<double> & v1
// Parameter: const vector<double> & v2
// Parameter: double fun
// Parameter: double
// Parameter: double
//************************************
vector<double> vec_operate(const vector<double>& v1, const vector<double>& v2, double fun(double, double))
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

	for (int father_index = 0; father_index < NR_PAGE; father_index++)
	{
		cout << "正在读入第" << father_index << "个文件" << endl;
		string linkfile = FILEDIR + "/childrenlink/" + int2str(father_index) + ".txt";
		ifstream fin(linkfile);
#ifdef DEBUG	
		assert(fin);
#endif
		string child;
		int nr_children = 0;
		while (fin >> child)
		{
			map<string, int>::iterator it = mp.find(child);
			if (it != mp.end()) {
				//vector<Matrixelem>::iterator el=find(M.begin(),M.end(),Matrixelem(it->second, father_index, 1));
				//if (el == M.end())
				//	M.push_back(Matrixelem(it->second, father_index, 1));
				//else
				//	el->value += 1;
				M.push_back(Matrixelem(it->second, father_index, 1));
				nr_children++;
			}

		}
		if (nr_children)
		{
			for_each(M.begin(), M.end(), [&](Matrixelem & it) {if (it.col == father_index)it.value /= nr_children; });
		}
		fin.close();
	}
}


//************************************
// Method:    matrix_mul_vector
// FullName:  matrix_mul_vector
// Access:    public 
// Returns:   std::vector<double>
// Qualifier: This function mul one n*n matrix with n*1 vector
// Parameter: vector<Matrixelem> A
// Parameter: vector<double>x
// Parameter: int n
//************************************
vector<double> matrix_mul_vector(vector<Matrixelem>&A, vector<double>&x, int n)
{
	vector<double> result(n);
	fill(result.begin(), result.end(), 0);
	for_each(M.begin(),M.end(),
		[&](auto it){
		result[it.row] += it.value*x[it.col];
	});
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
vector<double> pr_caculate()
{
	//放解的数组
	vector<double> result(NR_PAGE);

	//初始化解向量
	fill(result.begin(), result.end(), 1.0/NR_PAGE);
	double diff = 1 << 20;

	while (diff>eps)
	{
		cout << ".";
		vector<double> temp(NR_PAGE);
		temp = matrix_mul_vector(M, result, NR_PAGE);
		for_each(temp.begin(), temp.end(), [=](double &a) {a = beta*a+(1 - beta) * 1.0/ NR_PAGE; });
		diff = sqrt_of_pinfangsum(vec_operate(temp, result, [](double a, double b) {return a - b; }));
		result = temp;
	}

	for_each(result.begin(), result.end(), [=](double &a) {a *= amplification_factor; });
	return result;


}


int main()
{
	init_listfile();
	cout << "初始化文件列表成功..." << endl;
	init_link();
	cout << "初始化链接关系成功..." << endl;
	auto re = pr_caculate();

	ofstream fout(RESULTFILE);
#ifdef  DEBUG
	assert(fout);
#endif
	for_each(re.begin(), re.end(), [&fout](double rank) {fout << rank << endl; });
	fout.close();
	cout << "计算成功..." << endl;

	return 0;
}
