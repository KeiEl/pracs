#include "myvector.h"
#include <cstdio>
#include <cassert>
#include <stdexcept>
#include <cstring>
#include <typeinfo>

#define SIZE 20

int main() {

	// Set up
	MyVector<int> v;
	assert( v.size() == 0 );
	assert( v.capacity() == MyVector<int>::DEFAULT_SIZE );
	int max_sz;

	// Test push_back and at
	for ( int i = SIZE; i > 0; --i ) 
		v.push_back(i);

	assert( v.size() == SIZE );
	assert( v.capacity() == SIZE );
	for ( int i = 0; i < v.size(); ++i ) 
		assert( v.at(i) == SIZE - i );

	// Test resize (shrink)
	max_sz = v.capacity();
	v.resize( 10 );
	assert( v.size() == 10 );
	assert( v.capacity() == max_sz );

	// Test push_front and operator []
	for ( int i = 0; i < SIZE/2; ++i ) 
		v.push_front(5);

	assert( v.size() == SIZE );
	assert( v.capacity() == SIZE );
	for ( int i = 0; i < SIZE/2; ++i ) 
		assert( v[i] ==  5 );

	// Test clear
	max_sz = v.capacity();
	v.clear();
	assert( v.size() == 0 );
	assert( v.capacity() == max_sz );

	try {
		v.at(10);
	} catch ( const std::out_of_range &e ) {
		assert( strcmp( e.what(), "MyVector:: _M_range_check" ) == 0 );
	}

	// Test resize (expand)
	max_sz = v.capacity();
	v.resize( max_sz + 10 );
	assert( v.size() == max_sz + 10 );
	assert( v.capacity() != max_sz );
	for ( int i = 0; i < v.size(); ++i )
		assert( v.at(i) == 0 );

	printf( "All tests passed; woohoo! \\(^o^)/\n" );

	return 0;
}