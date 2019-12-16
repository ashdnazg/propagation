in_file = open("pieces.txt", "r")

def all_placements(num, width, height):
    variants = []
    for x_shift in range(8 - width + 1):
        for y_shift in range(8 - height + 1):
            variants.append(num << (x_shift + 8 * y_shift))
    
    return variants
    
def all_rotations(num, width, height):
    rotations = []
    for k in range(8):
        new_num = 0
        for i in range(width):
            for j in range(height):
                b = num >> (j * 8 + i) & 1
                if k & 2:
                    x = width - i - 1
                else:
                    x = i
                    
                if k & 4:
                    y = height - j - 1
                else:
                    y = j
                
                if k & 1:
                    x, y = y, x
                
                new_num |= b << (y * 8 + x)
        
        if k & 1:
            new_width, new_height = height, width
        else:
            new_width, new_height = width, height
            
        rotations.append((new_num, new_width, new_height))
    
    return set(rotations)

bitfields = [line.strip() for line in in_file.read().split("\n\n")]

nums = []
widths = []
heights = []
for bitfield in bitfields:
    bitfield2 = bitfield.replace("\n","")
    max_i = -1
    max_j = -1
    for i in range(8):
        for j in range(8):
            if bitfield2[i*8+j] == "1":
                max_i = max(i, max_i)
                max_j = max(j, max_j)

    widths.append(max_j + 1)
    heights.append(max_i + 1)
    nums.append(int(bitfield2[::-1], 2))

piece_triplets = []
for num, width, height in zip(nums, widths, heights):
    piece_triplets.append(all_rotations(num, width, height))


all_nums = []
for triplets in piece_triplets:
    piece_nums = set()
    for num, width, height in triplets:
        piece_nums = piece_nums.union(all_placements(num, width, height))
    all_nums.append(piece_nums)
    
# new_nums = []
# for i in range(4):
    # nums_a = all_nums[i * 2]
    # nums_b = all_nums[i * 2 + 1]
    # l = set()
    # for num_a in nums_a:
        # for num_b in nums_b:
            # if num_a & num_b == 0:
                # l.add(num_a | num_b)
    
    # print(len(l))
    # new_nums.append(l)

# print()
# new_new_nums = []
# for i in range(2):
    # nums_a = new_nums[i * 2]
    # nums_b = new_nums[i * 2 + 1]
    # l = set()
    # for num_a in nums_a:
        # for num_b in nums_b:
            # if num_a & num_b == 0:
                # l.add(~(num_a | num_b))
    
    # print(len(l))
    # new_new_nums.append(l)

# print()
# print(len(new_new_nums[0].intersection(new_new_nums[1])))

all_nums.sort(key=lambda x:len(x))
# print("{")
# for piece_nums in all_nums[::-1]:
    # for num in piece_nums:
        # print("\t%sL," % (num,))
# print("}")
lens = [len(l) for l in all_nums[::-1]]
running_sum = 0
for l in lens:
    print("{%s, %s}," % (running_sum, l))
    running_sum += l
