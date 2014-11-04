# 20
# 0 1 1
# 0 4 1
# 1 2 1
# 1 5 1
# 2 3 2
# 2 6 1
# 3 8 1
# 4 5 1
# 4 11 2
# 5 9 1
# 6 7 1
# 7 8 1
# 7 10 1
# 8 15 2
# 9 10 2
# 9 12 1
# 10 14 1
# 11 12 1
# 11 16 1
# 12 17 1
# 13 14 1
# 13 18 1
# 14 15 1
# 15 19 1
# 16 17 1
# 17 18 1
# 18 19 2
# 0 19
# ^^^ Because I'm too stingy. Lol.

#!/bin/python

import sys

def generate(filename):
	f = open(filename,'r')

	s = f.readline() # First line of actual input
	sz = int(s.split(' ')[1])

	al = [ [] for i in range(0,sz) ]

	while True:
		s = f.readline().split(' ') # # n1 n2 w
		if len(s) < 4:
			break
		al[int(s[1])].append( ( int(s[2]), int(s[3]) ) )
		al[int(s[2])].append( ( int(s[1]), int(s[3]) ) )

	src, dst = int(s[1]), int(s[2]) # # src dst
	f.close()

	return al, src, dst

def dfs( g, src, dst, h, curr, curr_p ):
	# Need to redo this... too clunky!
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
	al, src, dst = generate( sys.argv[0] ) # Read itself for input
	found, res, p = max_path( al, src, dst )
	if not found:
		print "No path found."
	else:
		print src, "to", dst, ":", res
		p.reverse()
		print p

if __name__ == "__main__":
	main()