#!/usr/bin/env python
import config, combine, processes
import glob, os, subprocess, sys

paths = config.paths
# Get the path for main Delphes analysis script
RunScript = os.path.join(os.path.dirname(__file__), "..", "analysis","RunDelphes")
RunScript = os.path.abspath(os.path.normpath(RunScript))


def RunDelphes(inFile, outFile, FlagWt):
	owd = os.getcwd() # save current working directory 
	os.chdir(paths['DelphesDIR']) # change directory to Delphes
	subprocess.call([RunScript, inFile, outFile, FlagWt])
	os.chdir(owd) # revert back to original directory 

def RunAnalysis(process, bin, FlagWt, REWRITE=True):
	InDIR = os.path.join(paths['DataDIR'] , process , bin)
	# exit if input directory does not exist
	if not os.path.exists(InDIR):
		print "Did not find : " + InDIR
		return
	# make a list of all root files 
	Files = glob.glob(os.path.join(InDIR , "*.root"))
	# exit if no root files present
	if not Files:
		print "No root files found in " + InDIR
		return
	OutDIR = os.path.join(paths['OutDIR'] , process, bin)
	# create output directory if it doesn't exist already
	if not os.path.exists(OutDIR):
		os.makedirs(OutDIR)
	for InFile in Files:
		OutFile = os.path.join(OutDIR , os.path.basename(InFile))
		if REWRITE and os.path.isfile(OutFile):
			os.remove(OutFile)
		if not os.path.isfile(OutFile): 
			RunDelphes(InFile, OutFile, FlagWt)	
			
def RunSMbkg(process, Lum=3000., REWRITE=True):
	if not process in processes.SMbkgs:
		print "Invalid SM background process. Aborting..."
		sys.exit()	
	xs = processes.xs[process]
	HTbins = xs.keys()
	HTbins.sort()
	for HTbin in HTbins[::-1]:
		RunAnalysis(process, HTbin, str(1), REWRITE)
		combine.CombineEvents(process, HTbin, Lum)	
	combine.CombineHTbins(process)

def RunSig(GridPara, Lum=3000., REWRITE=True):
	RunAnalysis("t~t~", GridPara, str(0), REWRITE)
	combine.CombineEvents("t~t~", GridPara, Lum)

def RunTest(GridPara, Lum=3000., REWRITE=True):
	RunAnalysis("tt", GridPara, str(0), REWRITE)
	combine.CombineEvents("tt", GridPara, Lum)

#--------------------------------------------
#debug	
if __name__ == "__main__":
	# RunSMbkg combines events from all HT bins
	# before running this code, set paths in Cards/config
	# first argument is process : e.g. 'ttB', 'tt', etc
	# second argument : luminosity in inverse fb
	# third argument : 
	#		if True, rerun analysis routine.  
	#		if False, run analysis only if it hasn't been run before
	#RunSMbkg("Bj", REWRITE=True)

	#RunSig("test2", REWRITE=True)
	RunTest("test", REWRITE=True)

