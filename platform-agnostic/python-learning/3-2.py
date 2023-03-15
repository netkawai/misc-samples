import re

handle = open('regex_sum_1531520.txt')
text = handle.read()
nums = re.findall('[0-9]+',text)
total = 0
for num in nums:
    total += int(num)

print(total)     