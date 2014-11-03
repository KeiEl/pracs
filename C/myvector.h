#ifndef MYVECTOR_H
#define MYVECTOR_H
#include <cstdlib>
#include <stdexcept>

template<class T>
class MyVector {
	private:
		T *ls;
		unsigned int sz;
		unsigned int max_sz;
	public:
		static const unsigned int DEFAULT_SIZE = 10;
		MyVector();
		~MyVector();
		void push_back( T item );
		void push_front( T item );
		void clear();
		void resize( int new_sz, T item = T() );
		void pop();
		void remove( int idx );
		T& at( int idx ) const;
		unsigned int size() const;
		unsigned int capacity() const;
		T& operator[]( int idx );
};

template<class T>
MyVector<T>::MyVector(): sz(0), max_sz(DEFAULT_SIZE) {
	ls = (T*)malloc( sizeof(T)*max_sz );
}

template<class T>
MyVector<T>::~MyVector() {
	for ( int i = 0; i < sz; ++i ) {
		ls[i].~T();
	}
	std::free(ls);
}

template<class T>
void MyVector<T>::push_back( T item ) {
	if ( sz >= max_sz ) {
		max_sz = max_sz*2;
		T* tmp = (T*)std::realloc( ls, (max_sz)*sizeof(T) );
		if ( tmp == NULL ) throw std::runtime_error( "MyVector:: Cannot reallocate memory (push_back)" );
		ls = tmp;
	}
	ls[sz] = item;
	sz++;
}

template<class T>
void MyVector<T>::push_front( T item ) {
	for ( int i = 0; i < sz; ++i ) {
		std::swap( ls[i], item );
	}
	push_back( item );
}

// WARNING: clear does not call delete on the element(s)!
template<class T>
void MyVector<T>::clear() {
	for ( int i = 0; i < sz; ++i ) {
		ls[i].~T();
	}
	sz = 0;
}

// WARNING: resize does not call delete on the element(s)!
template<class T>
void MyVector<T>::resize( int new_sz, T item ) {
	if ( new_sz < sz ) {
		for ( int i = new_sz; i < sz; ++i ) {
			ls[i].~T();
		}
	} else if ( new_sz > sz ) {
		if ( new_sz > max_sz ) {
			T *tmp = (T*)realloc( ls, sizeof(T)*new_sz );
			if ( tmp == NULL ) throw std::runtime_error( "MyVector:: Cannot reallocate memory (resize)" );
			ls = tmp;
			max_sz = new_sz;
		}

		for ( int i = sz; i < new_sz; i++ ) {
			ls[i] = item;
		}
	}
	sz = new_sz;
}

template<class T>
void MyVector<T>::pop() {
	if ( sz > 0 ) {
		ls[sz].~T();
		sz--;
	}
}

template<class T>
T& MyVector<T>::at( int idx ) const {
	if ( idx >= sz || idx < 0 ) {
		throw std::out_of_range( "MyVector:: _M_range_check" );
	}
	return ls[idx];
}

template<class T>
unsigned int MyVector<T>::size() const {
	return sz;
}

template<class T>
unsigned int MyVector<T>::capacity() const {
	return max_sz;
}

// WARNING: using this performs no check! :O
template<class T>
T& MyVector<T>::operator[]( int idx ) {
	return ls[idx];
}

#endif