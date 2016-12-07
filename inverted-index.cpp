#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
//#include <unordered_map>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

struct Posting
{
	int docId;
	vector<int> positions;
	// how to remove the overhead of copying the vector ?
	Posting(int id, const vector<int> &ivec) { docId = id; positions = ivec; }
};

map<wstring, vector<Posting>> inverted_index;

void create_inverted_index(wistream &fin)
{
	wstring line;
	while (getline(fin, line))
	{
		int page_no = stoi(line);
		getline(fin, line);
		wistringstream words(line);
		wstring word;
		// <word, position list>
		map <wstring, vector<int>> temp_index;
		int position = 0;
		while (words >> word)
		{
			temp_index[word].push_back(position++);
		}
		// merge temp_index into our main index
		for (const auto &e : temp_index)
		{
			inverted_index[e.first].emplace_back(page_no, e.second);
		}
	}
}

void print_inverted_index(wostream &out)
{
	for (const auto &entry : inverted_index)
	{
		out << entry.first << '|';
		for (const auto &posting : entry.second)
		{
			out << posting.docId << ':';
			for_each(posting.positions.cbegin(), posting.positions.cend() - 1,
				[&out](int p) { out << p << ','; });
			out << posting.positions.back() << ';';
		}
		out << endl;
	}
}

int main()
{
	wifstream fin("result0.txt");
	create_inverted_index(fin);
	fin.close();
	wofstream fout("inverted_index.txt");
	print_inverted_index(fout);
	fout.close();
	return 0;
}