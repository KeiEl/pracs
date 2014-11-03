// Based on http://burakkanber.com/blog/machine-learning-in-js-k-nearest-neighbor-part-1/

#include "nodelist.h"
#include <cstdio>
#include <cstring>
#include <algorithm>

#define MAX_READ 256 // Size of buffer
#define TYPES 3 // Number of types/classifications

using namespace std;

NodeList::NodeList( const char *fname, int k_val ): k(k_val) {
	r_bounds[0] = FLT_MAX;
	a_bounds[0] = FLT_MAX;
	r_bounds[1] = 0.0;
	a_bounds[1] = 0.0;

	FILE *f = fopen( fname, "r" );
	if ( f == NULL ) {
		fprintf( stderr, "Could not read %s", fname );
		// Throw exception?
	}

	char buf[MAX_READ];
	fgets( buf, MAX_READ, f );
	while ( fgets( buf, MAX_READ, f ) != NULL ) {
		Node n;
		sscanf( buf, "%d,%f,%c", &(n.r), &(n.a), &(n.t) );
		switch ( n.t ) {
			case '?':
				unknowns.push_back(n);
				break;
			default:
				data.push_back(n);
		}
		updateBounds(n);
	}

	printf( "Read all data...\n" );

	fclose(f);
}

NodeList::~NodeList() {}

void NodeList::compute() {
	for ( int i = 0; i < unknowns.size(); ++i ) {
		printf( "Calculate d for r: %d, a: %f\n", unknowns.at(i).r, unknowns.at(i).a );
		vector< pair<float,char> > d_ls = getDistances( unknowns.at(i) );
		printType( d_ls, unknowns.at(i) );
	}
}

void NodeList::updateBounds( const Node &n ) {
	if ( n.r < r_bounds[0] ) {
		r_bounds[0] = n.r;
	}
	if ( n.r > r_bounds[1] ) {
		r_bounds[1] = n.r;
	}

	if ( n.a < a_bounds[0] ) {
		a_bounds[0] = n.a;
	}
	if ( n.a > a_bounds[1] ) {
		a_bounds[1] = n.a;
	}
}

bool compar( const pair<float,char> &a, const pair<float,char> &b ) {
	return a.first < b.first;
}

vector< pair<float,char> > NodeList::getDistances( const Node &unknown ) const {
	vector< pair<float,char> > ls;
	float r_range = r_bounds[1] - r_bounds[0];
	float a_range = a_bounds[1] - a_bounds[0];

	for ( int i = 0; i < data.size(); ++i ) {
		float delta_room = ( unknown.r - data.at(i).r ) / r_range;
		float delta_area = ( unknown.a - data.at(i).a ) / a_range;
		float d = delta_area*delta_area + delta_room*delta_room;
		ls.push_back( pair<float,char>( d, data.at(i).t ) );
	}

	sort( ls.begin(), ls.end(), compar );
	return ls;
}

void NodeList::printType( const vector< pair<float,char> > &ls, const Node &unknown ) const {
	int freq[TYPES] = {0};
	for ( int i = 0; i < k; ++i ) {
		switch ( ls.at(i).second ) {
			case 'a':
				freq[0]++;
				break;
			case 'h':
				freq[1]++;
				break;
			case 'f':
				freq[2]++;
				break;
		}
	}

	printf( "apartment: %.2f\n", (float)freq[0]/k );
	printf( "house: %.2f\n", (float)freq[1]/k );
	printf( "flat: %.2f\n", (float)freq[2]/k );
}

int main() {

	int k = 3;
	char s[] = "knn_tmp";

	NodeList n_ls = NodeList( s, k );
	n_ls.compute();
	
	return 0;
}