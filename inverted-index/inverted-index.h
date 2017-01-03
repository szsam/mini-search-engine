//#include <vector>
#include "vector.h"
#include <string>
#include <iostream>

struct Posting
{
	int docId;
	xmh::vector<int> positions;
	Posting() = default;
	// how to remove the overhead of copying the vector ?
	Posting(int id, const xmh::vector<int> &ivec) { docId = id; positions = ivec; }
};


void create_inverted_index(std::istream &);

void print_inverted_index(std::ostream &);

void read_inverted_index(std::istream &);

xmh::vector<Posting> search_inverted_index(std::string);