import sys

def parse_log(log_path):
	log_file = open(log_path, "rb")
	results = []

	skip = True

	for line in log_file:
		line = line.strip()
		if line.startswith("Instance"):
			results.append({})
			continue

		if line.startswith("Starting"):
			skip = False
			continue

		cur_dict = results[-1]

		if line.startswith("Doing"):
			skip = True
			cur_dict[last_h].append(int(line.split()[3]))
			continue

		if line.startswith("Solutions"):
			cur_dict[last_h].append(int(line.split()[-1]))
			continue

		if line.startswith("C*"):
			cur_dict["C*"] = int(line.split()[-1])
			continue

		if line.startswith("IDA*"):
			cur_dict["C*"] = int(line.split()[-1])

		if "," in line:
			if skip:
				continue
			skip = True

			parts = line.split(":")
			last_h = int(parts[0])

			if last_h == 0:
				cur_dict.clear()

			parts = parts[1].split(",")
			cur_dict[last_h] = [int(p.strip()) for p in parts]

	return results


def analyze_results(results):
	pass


def main(log_path):
	results = parse_log(log_path)
	print len(results)
	print results[0]


if __name__ == '__main__':
	main(sys.argv[1])