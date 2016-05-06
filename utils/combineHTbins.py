#!/usr/bin/env python

from ROOT import TFile, TTree,TChain
import processes
import numpy as np
from glob import glob
import os, subprocess


process="Bj"
OutDIR = "/Users/prerit/Physics/100TeV/Stops/Results"
Lum=3000.
xs = processes.xs
HTbins = xs[process].keys()
HTbins.sort()
sampleFilePATH = None
for HTbin in  HTbins[::-1] :
    sampleFilePATH = os.path.join(OutDIR, process, HTbin, "001.root")
    if sampleFilePATH : break
if not sampleFilePATH : print "******************"
#sampleFile = TFile(sampleFilePATH)
sampleChain = TChain("event")
sampleChain.Add(sampleFilePATH)
outFile = os.path.join(OutDIR, process, "combined.root")
newFile = TFile(outFile,"RECREATE") 
tree = sampleChain.CloneTree(0)
cWeight = np.zeros(1, dtype=float)
tree.Branch('cWeight', cWeight, 'cWeight/D')
for HTbin in  HTbins[::-1] :
    # Get list of input root files from data directory
    # filenames : 001.root, 002.root, .....
    filesPATH = os.path.join(OutDIR, process, HTbin, "0*.root")
    files = glob(filesPATH)
    # create a chain to which the input files will be added
    event = TChain("event")
    print "Processing HT bin : "+HTbin+" GeV"
    # make sure there is at least one file in the list
    if not files : continue
    # wtTotal : sum of weights in a given root file
    # wtCut   : sum of weights after pre-selection (PS) cuts are applied 
    wtTotal = wtCut = 0.
    # loop through files in a gievn HT bin and 'chain' them
    # also calculate total weight and weight after PS cuts
    Tfiles = {}
    for filename in files:
        Tfiles[filename] = TFile(filename)
    for filename in Tfiles.keys():
        print "Processing file : "+filename
        tree = Tfiles[filename].Get("event")
        weights = Tfiles[filename].Get("PScuts")
        wtTotal = wtTotal + weights[0]
        wtCut = wtCut + weights[1]
        event.Add(filename)
    nEntries = event.GetEntries() # number of entries in the chain
    for branch in event.GetListOfBranches():
        tree.SetBranchAddress(branch.GetName(), event.GetBranch(branch.GetName()))
    # create a new tree with same structure as chain :
    #tree = event.CloneTree(0) 
    # cWeight : combined weight = (event weight)*xsection*luminosity*efficieny
    #cWeight = np.zeros(1, dtype=float)
    eps = 1e-10 #safeguard for division by zero
    eff = wtCut/max(wtTotal,1e-10)
    norm = xs[process][HTbin]*Lum*eff
    # add a branch in the new tree to store combined weight
    #tree.Branch('cWeight', cWeight, 'cWeight/D')
    # loop over all the entires in chain and store all the information inside  
    # the chain into the new tree and additionally, store the combined weight
    for i in xrange(nEntries): 
        event.GetEntry(i) 
        cWeight[0] = event.weight*norm
        #newFile = TFile(outFile,"UPDATE") 
        tree.Fill() 
tree.GetCurrentFile().Write()
tree.GetCurrentFile().Close() 
