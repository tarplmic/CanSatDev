file1 = open('test.txt', 'r')
file2 = open('test2.txt', 'w')
Lines = file1.readlines()
 
count = 0
# Strips the newline character
for line in Lines:
    count += 1
    if(line != ""):
        print(line.strip(), ",")
        file2.write(line.strip() + ",\n")

file2.close()