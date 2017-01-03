#include <vector>
#include <string>
#include <iostream>

struct Posting
{
	int docId;
	std::vector<int> positions;
	Posting() = default;
	// how to remove the overhead of copying the vector ?
	Posting(int id, const std::vector<int> &ivec) { docId = id; positions = ivec; }
};


void create_inverted_index(std::istream &);

void print_inverted_index(std::ostream &);

void read_inverted_index(std::istream &);

std::vector<Posting> search_inverted_index(std::string);