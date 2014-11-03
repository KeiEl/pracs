#!/bin/python

import sys

def generate(filename):
	f = open(filename,'r')
	s = f.readline()
	sz = int(s)
	al = dict()
	for i in range(0,sz):
		al[i] = []
	while True:
		s = f.readline().split(' ')
		if len(s) < 3:
			break
		al[int(s[0])].append( ( int(s[1]), int(s[2]) ) )
		al[int(s[1])].append( ( int(s[0]), int(s[2]) ) )
	src, dst = int(s[0]), int(s[1])
	f.close()
	return al, src, dst

def dfs( g, src, dst, h, curr, curr_p ):
	if src == dst:
		return True, curr, [dst]
	max_d = -1
	max_p = []
	for n,w in g[src]:
		if h[n] == 0:
			h[n] = 1
			found, d, p = dfs( g, n, dst, h, w, [n] )
			if found and max_d < d:
				max_d = d
				max_p = p
			h[n] = 0
	if max_d == -1:
		return False, curr, curr_p
	return True, max_d+curr, max_p+curr_p

def max_path( g, src, dst ):
	h = [ 0 for x in range(0,len(g)) ]
	h[src] = 1
	p = [ src ]
	return dfs( g, src, dst, h, 0, p )

def main():
	if len(sys.argv) < 2:
		print "Usage", sys.argv[0], "[pathname]"
	al, src, dst = generate( sys.argv[1] )
	found, res, p = max_path( al, src, dst )
	if not found:
		print "No path found."
	else:
		print src, "to", dst, ":", res
		p.reverse()
		print p

if __name__ == "__main__":
	main()