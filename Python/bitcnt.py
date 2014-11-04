def init():
	# There's probably a better way, but eh...
	lookup = []
	lookup.append(0)
	lookup.append(1)
	for i in range(2, 256):
		ones = 0
		num = i
		while num >= len(lookup):
			bit = num & 1
			ones += bit
			num = num >> 1
		ones += lookup[num]
		lookup.append(ones)
	return lookup

def bitcnt( num, lookup ):
	n = 0b11111111
	total = 0
	for i in range(0,4):
		tmp = num & n
		total += lookup[tmp]
		num = num >> 8
	return total

def main():
	lookup = init()
	while True:
		try:
			inpt = raw_input( "Enter a number: " )
			print "Number of bits:", bitcnt(int(inpt),lookup)
		except EOFError:
			break
	print ''

if __name__ == "__main__":
	main()