// Based on http://burakkanber.com/blog/machine-learning-in-js-k-nearest-neighbor-part-1/

#ifndef NODELIST_H
#define NODELIST_H

#include <vector>
#include <utility>
#include <cfloat>

struct Node {
	int r;
	float a;
	char t;
};

class NodeList {
	private:
		float r_bounds[2];
		float a_bounds[2];
		std::vector<Node> data;
		std::vector<Node> unknowns;
		int k;
	public:
		NodeList( const char *fname, int k_val );
		~NodeList();
		void compute();
		void updateBounds( const Node &n );
		std::vector< std::pair<float,char> > getDistances( const Node &unknown ) const;
		void printType( const std::vector< std::pair<float,char> > &ls, const Node &unknown ) const;
};

#endif