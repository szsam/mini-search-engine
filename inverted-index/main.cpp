#include "inverted-index.h"

#include <iostream>
#include <fstream>
using std::ifstream; using std::ofstream; using std::endl;
using std::cin; using std::cout; using std::cerr;
using std::string;
//using namespace std;

#include "vector.h"
using xmh::vector;

// define CREATE to create inverted index
#define CREATE
// define READ to test the implementation of read_inverted_index()
//#define READ

#ifdef CREATE
int main()
{
	ifstream fin("F:\\Data structures\\big-project\\result2.txt");
	if (!fin.is_open()) { cerr << "Fail to open file!\n"; exit(-1); }

	create_inverted_index(fin);
	cout << "Succeed to create inverted-index" << endl;
	fin.close();

	ofstream fout("F:\\Data structures\\big-project\\inverted_index22.txt");
	if (!fout.is_open()) { cerr << "Fail to open file!\n"; exit(-1); }

	print_inverted_index(fout);
	cout << "Output finish" << endl;
	fout.close();

	return 0;
}

#elif defined READ
// test read_inverted_index()
int main()
{
	ifstream fin("F:\\Data structures\\big-project\\inverted_index2.txt");

	if (!fin.is_open()) { cerr << "Fail to open file!\n"; exit(-1); }

	read_inverted_index(fin);
	cout << "Reading inverted index finished.\n";
	vector<Posting> postings_list;
	string buf;
	while (cin >> buf)
	{
		postings_list = search_inverted_index(buf);
		if (postings_list.size() != 0)
		{
			for (const auto &e : postings_list)
				cout << e.docId << " ";
			cout << endl;
		}
		else
			cout << "Not found!\n";
	}
	//ofstream fout("output.txt");
	//print_inverted_index(fout);
	return 0;
}

#else
 #error
#endif