#include "inverted-index.h"
#include <fstream>
using namespace std;

// define CREATE to create inverted index
//#define CREATE
// define READ to test the implementation of read_inverted_index()
#define READ

#ifdef CREATE
int main()
{
	ifstream fin("F:\\Data structures\\大作业\\result2.txt");
	create_inverted_index(fin);
	fin.close();
	ofstream fout("F:\\Data structures\\大作业\\inverted_index2.txt");
	print_inverted_index(fout);
	fout.close();
	return 0;
}

#elif defined READ
// test read_inverted_index()
int main()
{
	ifstream fin("F:\\Data structures\\大作业\\inverted_index2.txt");
	read_inverted_index(fin);
	cout << "Reading inverted index finished.\n";
	vector<Posting> postings_list;
	string buf;
	while (cin >> buf)
	{
		postings_list = search_inverted_index(buf);
		if (!postings_list.empty())
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