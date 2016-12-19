from math import factorial

N = 3
N_FACT = factorial(N)

def pretty_perm(u):
	read_list = [0]
	for i in range(2, N + 1):
		read_list.append(u % i)
		u /= i

	print read_list
	available = list(range(N))
	pretty = []
	for index in read_list[::-1]:
		pretty.append(available[index])
		del available[index]

	return pretty

def ugly_perm(p):
	available = list(range(N))

	options = N
	u = 0
	for i in p:
		index = available.index(i)
		del available[index]
		u *= options
		u += index
		options -= 1

	return u


def main():
	for u in range(N_FACT):
		print u, N_FACT - u - 1
		print pretty_perm(u)
		print pretty_perm(N_FACT - u - 1)
		print ""


if __name__ == '__main__':
	main()