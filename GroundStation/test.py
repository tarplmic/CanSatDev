file1 = open('test.txt', 'r')
Lines = file1.readlines()
 
count = 0
# Strips the newline character
for line in Lines:
    count += 1
    if(line != ""):
        print(line.strip(), ",")