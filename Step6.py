import sys
import csv

f1 = open(sys.argv[1], 'r')
reader = csv.reader(f1)
headings = next(reader)

data = []

for l in reader:
    data.append(float(l[1]))  # Assuming temperatures are in the second column

print(f"array: {data}")

f1.close()

min_num = None
max_num = None

for i in range(len(data)):
    if i == 0:
        min_num = data[i]
        max_num = data[i]
        continue

    if data[i] > max_num:
        max_num = data[i]

    if data[i] < min_num:
        min_num = data[i]

curSum = 0

for i in range(len(data)):
    curSum += data[i]

average = curSum / len(data)

print(f"Avg: {average} Min: {min_num} Max: {max_num}")

