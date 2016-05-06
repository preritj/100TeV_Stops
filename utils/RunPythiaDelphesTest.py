#!/usr/bin/env python
# Note : Place this file in the Delphes base directory and keep DIR (see below) unchanged
from subprocess import Popen, call, PIPE

run_name    = "001"  # run-name of the root files 
DIR			= "/Users/prerit/Physics/Delphes/Delphes-3.3.2" # your Delphes base directory (do not change if you place this script in Delphes base directory)
OUTDIR      = "/Users/prerit/Physics/100TeV/Data/tt/" # Directory where root output files will be stored (temporrarily if sending to remote machine)
#SlhaFile    = "/Users/prerit/Physics/100TeV/Stops/Cards/default.slha" # default slha file with path included
PythiaCMND  = "/Users/prerit/Physics/100TeV/Stops/Cards/ttTest.cmnd" # default command file for Pythia run with path included
DelphesCard = "/Users/prerit/Physics/100TeV/Stops/Cards/100TeV.tcl" # Delphes card with path included
HOSTDIR     = "100TeV/Stops" # Directory in host machine where root files are stored

# function to write slha files for grid  using default slha file
def WriteSLHA(OutFile, Mst, Mgl) :
	f=open(SlhaFile,'r')
	fout=open(OutFile,'w')
	for line in f.readlines():
		if not line.split():
			out=line
		elif line.split()[0] == "1000006" :
			out = "      1000006 "+str(Mst)+".000e+00 #  ~t_1\n"
		elif line.split()[0] == "1000021" :
			out = "      1000021 "+str(Mgl)+".000e+00 #  ~g\n"
		else :
			out=line
		fout.write(out)
	f.close()
 	fout.close()

# function to write Pythia cmnd for grid  using default cmnd file
def WriteCMND(OutFile, NewSlhaFile) :
	f=open(PythiaCMND,'r')
	fout=open(OutFile,'w')
	for line in f.readlines():
		if line[:9] == "SLHA:file" :
			out = "SLHA:file = "+NewSlhaFile+"\n"
		else :
			out=line
		fout.write(out)
	f.close()
 	fout.close()

# Send files to remote server to save disk space
def SendFiles(Mst, Mgl):
	process=str(Mst)+"_"+str(Mgl)
	# create a directory for grid point in remote machine
	call("RunBrown mkdir -p "+HOSTDIR+process ,shell=True)
	# send root files
	RootFile=OUTDIR+process+"/"+run_name+".root"
	call("sendfiles "+RootFile+" "+HOSTDIR+process, shell=True)
	# delete root files on own machine to save disk space
	call("rm "+RootFile,shell=True)

if not DIR.endswith("/") : 
	DIR=DIR+"/"
if not OUTDIR.endswith("/") : 
	OUTDIR=OUTDIR+"/"
if not HOSTDIR.endswith("/") : 
	HOSTDIR=HOSTDIR+"/"
for Mst in range(8000,8001,500) :
#	for Mgl in range(3000,Mst-499,500) :
	for Mgl in range(3000,3001,500) :
		process="test"
		# Create directory for the grid point (if it doesn't exist)
		call("mkdir -p "+OUTDIR+process ,shell=True)
		# remove old root files with the same run-name
		RootFile=OUTDIR+process+"/"+run_name+".root"
		call("rm "+RootFile,shell=True)
		# Make slha file for the parameter point
		#NewSlhaFile=OUTDIR+process+"/"+process+".slha"
		#WriteSLHA(NewSlhaFile, Mst, Mgl)
		# Make cmnd file for Pythia 8
		PyFile=PythiaCMND
		#WriteCMND(PyFile, NewSlhaFile)
		# Run Delphes
		call("cd "+DIR+" && ./DelphesPythia8 "+DelphesCard+" "+PyFile+" "+RootFile,shell=True) 
		# send files to remote server
		#SendFiles(Mst, Mgl)


