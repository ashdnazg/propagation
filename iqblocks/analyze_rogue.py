in_file = open("rogue.txt", "r")

content = in_file.read()

content = content.strip().replace("\n","")
content = "".join(content[i * 8 + j] for j in range(8) for i in range(8))
nums = []
for i in range(10):
    num_string = "".join("1" if c == str(i) else "0" for c in content)
    nums.append(int(num_string, 2))

for num in nums:
    print(num)