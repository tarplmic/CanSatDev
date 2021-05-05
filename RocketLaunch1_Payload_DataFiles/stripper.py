file1 = open('CombinedPayloadData.txt', 'r')
file2 = open('CombinedPayloadDataStripped.txt', 'w')
Lines = file1.readlines()
 
# Strips the newline character
for line in Lines:
    if(line != "\n"):
        print(line.strip())
        file2.write(line.strip() + "\n")

#file2.close()