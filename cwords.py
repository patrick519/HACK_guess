# Patrick t Jong
# September 2017
# run by 'python cwords.py'

words = []
fout = raw_input("Enter output name: ")

# Adding words to array
def readwords( file ):
	fh = open(file)
	for line in fh:
		for word in line.split():
			#Getting rid of special characters
			word = filter(str.isalnum, word)
			if word not in words:
				words.append(word)
	fh.close()
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

