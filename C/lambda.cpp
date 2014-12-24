// Compile with -std=c++11

#include <iostream>

#define ABS(x) ( x > 0 ? x : -x )

void quicksort( int *arr, int start, int end, bool (*compar)(int a, int b) ) {
	if ( start >= end ) return;

	int midpt = start + (end-start)/2;
	std::swap( arr[start], arr[midpt] );
	
	int last_small = start;
	for ( int i = start; i <= end; ++i ) {
		if ( compar( arr[i], arr[start] ) ) {
			last_small++;
			std::swap( arr[last_small], arr[i] );
		}
	}
	std::swap( arr[last_small], arr[start] );

	quicksort( arr, start, midpt-1, compar );
	quicksort( arr, midpt+1, end, compar );
}


int main() {
	int arr[] = { 9, 3, 1, -4, 0, 13, -5, -15, 10, 1 };
	int sz = 10;

	auto print_ls = []( int* arr, int sz ) {
		for ( int i = 0; i < sz; ++i ) {
			std::cout << arr[i] << " ";
		}
		std::cout << std::endl;
	};

	quicksort( arr, 0, sz-1, []( int a, int b ){ return a < b; } );
	print_ls( arr, sz );
	
	quicksort( arr, 0, sz-1, []( int a, int b ){ return a > b; } );
	print_ls( arr, sz );

	auto func_in_func = [=]( int x ) {
		// sz is available with '=', which captures all *to-be-used* variables, i.e. arr is not captured
		const int c = 2;

		return [x, sz, c]( int y ) mutable { 
			std::cout << "prechanged_x: " << x << std::endl;
			x = x + 7; // Copy by value, changeable with mutable
			std::cout << "postchanged_x: " << x << std::endl;
			std::cout << "y: " << y << std::endl;
			std::cout << "sz: " << sz << std::endl;
			std::cout << "c: " << c << std::endl;
			// c++; // Not changeable 
			return c + sz + x + y; };
	};

	int a = 4; // Cannot be captured by func_in_func
	std::cout << func_in_func(1)(4) << std::endl << std::endl;

	auto f = func_in_func(11);
	std::cout << f(13) << std::endl << std::endl; // Note x: 11->18
	std::cout << f(15) << std::endl << std::endl; // Note x: 18->25

	auto nochange_cnt = [](){
		int c = 5;
		return [&c]() { return --c; };
	};

	// f = nochange_cnt(); // Cannot reuse f?
	auto h = nochange_cnt();
	std::cout << h() << std::endl;
	std::cout << h() << std::endl; // Weird number appears?
	std::cout << std::endl;

	auto decrement_cnt = []( int c ) { return [c]( int d ) mutable { return c - d; }; };

	auto g = decrement_cnt(5);
	int x = 0;
	do {
		x = ABS(x);
		x = g(x);
		std::cout << x << std::endl;
	} while ( x > 0 );

	return 0;
}