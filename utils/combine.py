#!/usr/bin/env python
import config
from ROOT import TFile, TTree,TChain
import processes
import numpy as np
from glob import glob
import os, subprocess

paths = config.paths

def CombineEvents(process, bin, Lum=3000.):
	# events = xsec (in pb) x luminosity (in ifb) * 1000
	#norm = luminosity * 1000. * processes.xs[process][bin]
	DIR = os.path.join(paths['OutDIR'] , process , bin)
	# exit if input directory does not exist
	if not os.path.exists(DIR):
		print "Did not find : " + DIR
		return
	OutFile = os.path.join(DIR, "combined.root")
	# remove combined root file if one already exists
	if os.path.isfile(OutFile):
		os.remove(OutFile) 
	# make a list of all root files 
	Files = glob(os.path.join(DIR , "0*.root"))
	# exit if no root files present
	if not Files:
		print "No root files found in " + DIR
		return
	# 'chain' all the root files together
	event = TChain("event")
	# wtTotal : sum of weights in a given root file
	# wtCut   : sum of weights after pre-selection (PS) cuts are applied 
	wtTotal = wtCut = 0.
	for rootFile in Files:
		inputFile = TFile(rootFile)
		eventTree = inputFile.Get("event")
		# get weights (pre and post pre-selection cuts)
		weights = inputFile.Get("PScuts")
		wtTotal = wtTotal + weights[0]
		wtCut = wtCut + weights[1]
		event.Add(rootFile)
	# efficiency for acceptance after pre-selection cuts
	eff = wtCut/max(wtTotal, 1e-10)
	# total number of entries in entire chain
	nEntries = event.GetEntries()
	newFile = TFile(OutFile, "RECREATE")
	# clone the 'chain' to a new 'tree'
	tree = event.CloneTree(0)
	# add a new 'branch' in 'tree' which stores the 
	# weight which is correctly normalized
	cWeight = np.zeros(1, dtype=float)
	tree.Branch('cWeight', cWeight, 'cWeight/D')
	xs = processes.xs[process]
	# final scaling factor for each event weight:
	norm = xs[bin]*Lum*eff
	for i in xrange(nEntries):
		event.GetEntry(i)
		cWeight[0] = event.weight*norm
		tree.Fill()
	tree.GetCurrentFile().Write()
	tree.GetCurrentFile().Close()
	


def CombineHTbins(process):
	xs = processes.xs[process]
	DIR = os.path.join(paths['OutDIR'] , process)
	if not os.path.exists(DIR):
		print "Did not find : " + DIR
		print "Aborting... "
		sys.exit()
	OutFile = os.path.join(DIR, "combined.root")
	# remove combined root file if one already exists
	if os.path.isfile(OutFile):
		os.remove(OutFile) 
	HTbins = xs.keys()
	HTbins.sort()
	HTbinFiles = []
	for HTbin in HTbins[::-1]:
		InFile = os.path.join(DIR , HTbin, "combined.root")
		if not os.path.isfile(InFile):
			print "Did not find the file : " + InFile
			continue
		else :
			HTbinFiles.append(InFile)
	subprocess.call(["hadd",OutFile]+HTbinFiles)
		
		

#debug
if __name__ == "__main__":
	#CombineEvents("ttB", "9000-100000", 3000)
	CombineHTbins("ttB")

