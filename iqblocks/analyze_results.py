in_file = open("results6.txt", "r")

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

    
    
nums = set([int(line) for line in in_file.read().strip().split("\n")])
print(len(nums))
filtered = set()
all_nums = set()
for num in nums:
    rotations = all_rotations(num, 8, 8)
    found = False
    for r in rotations:
        if r in all_nums:
            found = True
            break
    
    if not found:
        filtered.add(num)
        for r in rotations:
            all_nums.add(r)
    
    
print(len(filtered))
print(len(all_nums))