#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
//#include <unordered_map>
#include <vector>
#include <string>
#include <algorithm>

#include "inverted-index.h"

using namespace std;

static map<string, vector<Posting>> inverted_index;

void create_inverted_index(istream &fin)
{
	string line;
	while (getline(fin, line))
	{
		int page_no = stoi(line);
		getline(fin, line);
		istringstream words(line);
		string word;
		// <word, position list>
		map <string, vector<int>> temp_index;
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

void print_inverted_index(ostream &out)
{
	// format:
	// term|docID1:pos1,pos2;docID2:pos3,pos4,pos5;бн
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

// read the inverted index saved on disk into memory
void read_inverted_index(istream &in)
{
	// format:
	// term|docID1:pos1,pos2;docID2:pos3,pos4,pos5;бн
	string line;
	while (getline(in, line))
	{
		istringstream line_stream(line);
		// read term
		string term;
		getline(line_stream, term, '|');
		// Postings are separated by ';'
		string posting_str;
		vector<Posting> posting_list;
		while (getline(line_stream, posting_str, ';'))
		{
			istringstream posting_stream(posting_str);
			string docId;
			getline(posting_stream, docId, ':');
			vector<int> positions;
			string pos_str;
			// position numbers are separated by comma
			while (getline(posting_stream, pos_str, ','))
			{
				positions.push_back(stoi(pos_str));
			}
			posting_list.emplace_back(stoi(docId), positions);
		}
		inverted_index[term] = posting_list;
	}
	
}

vector<Posting> search_inverted_index(string term)
{
	auto it = inverted_index.find(term);
	if (it != inverted_index.cend())
	{
		return it->second;
	}
	else
		return vector<Posting>();
}

