in_file = open("solutions2.txt", "r")
placements_file = open("placements.txt", "r")

lines = [line.strip().split(",") for line in in_file.read().strip().split("\n")]
placements = [int(s) for s in placements_file.read().strip().split("\n")]

print(len(lines))
print(len(placements))

offsets = [
    2040,
    1944,
    1800,
    1656,
    1432,
    1192,
    952,
    672,
    336,
    0
]


solutions = [[int(p[i + 1]) - int(p[i]) for i in range(10)] for p in lines]

for solution in solutions:
	sol = 0
	arr = [-1] * 64
	for i, num in enumerate(solution):
		index = offsets[i] + num
		sol |= placements[index]
		for i in range(64):
			arr[i] += (sol >> i) % 2
	if sol != (1 << 64) - 1:
		print("oh no")
		exit()
	print(arr)
