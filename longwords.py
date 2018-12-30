# Patrick t Jong
# September 2017
# run by 'python longwords.py'

word11 = []
word13 = []
words = []
fout = raw_input("Enter output name: ")

# Adding words to array
def readwords( file ):
	fh = open(file)
	count = 0
	for line in fh:
		for word in line.split():
			if len(word) == 11 and word.islower():
				word11.append(word)
			if len(word) == 13 and word.islower():
				word13.append(word)
	fh.close()
	#for item11 in word11:
	for item13 in word13:
			words.append(item13)
	return;

# Printing array to file
def printwords():
	thefile = open(fout, 'w')
	for item in words:
		thefile.write("%s\n" % item)
	thefile.close()
	return;

# Main
reading = True;
while reading:
	fname = raw_input("Enter input name: ")
	readwords( fname )
	carry = raw_input("Read more [Y/N]: ")
	if carry == "N":
		reading = False;
printwords()
print "Completed."

