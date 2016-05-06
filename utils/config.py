#!/usr/bin/env python
import os, sys 

# Read configuration files to get 
# the directories for Delphes and event files


def paths():
	owd = os.getcwd() # save the current path
	os.chdir(os.path.abspath(os.path.dirname(__file__)))	
	paths = {}
	# check if the directory 'Cards' exists
	if not os.path.exists("../Cards") :
		print "Could not locate the directory 'Cards', aborting..."
		sys.exit()
	# check if the file 'config' is present
	if not os.path.isfile("../Cards/config") :
		print "Could not locate the file 'config', aborting..."
		sys.exit()
	configFile = "../Cards/config"
	f = open(configFile, 'r')
	lines = f.readlines()
	for line in lines:
		l = line.partition('#')[0]
		l = l.split('=')
		if not l : continue
		tag = l[0].strip()
		# Read Delphes path
		if tag == "DelphesDIR":
			paths['DelphesDIR'] = (l[1].strip()).rstrip(str(os.sep))
			if not os.path.exists(paths['DelphesDIR']) :
				print "Could not locate Delphes directory, aborting..."
				sys.exit()
			else :
				print "Using Delphes libraries from : "+paths['DelphesDIR']
		# Read path for event files
		if tag == "DataDIR":
			paths['DataDIR'] = (l[1].strip()).rstrip(str(os.sep))
			if not os.path.exists(paths['DataDIR']) :
				print "Could not locate data directory, aborting..."
				sys.exit()
			else :
				print "Reading root files from (base directory) : "+paths['DataDIR']
		# Read path for output files
		if tag == "OutDIR":
			paths['OutDIR'] = (l[1].strip()).rstrip(str(os.sep))
			if not os.path.exists(paths['OutDIR']) :
				print "Could not locate output directory, aborting..."
				sys.exit()
			else :
				print "Storing output root files in (base directory) : "+paths['OutDIR']
	f.close()
	os.chdir(owd) #return to original path
	# set path variables in shell
	os.environ["DELPHES"] = paths['DelphesDIR']
	os.environ["LD_LIBRARY_PATH"] = paths['DelphesDIR']+":"+os.environ["LD_LIBRARY_PATH"]
	return paths

paths = paths()

#---------------------------------
#debug
if __name__ == "__main__":
	print paths

