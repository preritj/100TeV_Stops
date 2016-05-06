#!/usr/bin/env python
#This script facilitates download of snowmass root files from the web
import urllib, urllib2
import bs4 # beautiful soup
import os, sys

# Read number of events in the root files
def GetEvents(filename): 
    events = 0
    with open(filename,'r') as f:
        for line in f: 
            if line.startswith('Input Events'): 
                events =  int(line.split(':')[1].strip())
                break
    return events

# Downloads root files from the web in the directory outDIR
# process can be for example 'ttB', 'tt', 'tB', etc.
# HTbin is the HT bin for a given process for example '9000-100000'
# nEvents is the approx number of events you want to download 
def DownloadRoots(process, HTbin, nEvents, outDIR='.'):
    # create directory to store root files
    procDIR = os.path.join(outDIR, process, HTbin)
    procDIR = os.path.abspath(procDIR)
    if not os.path.exists(procDIR):
        os.makedirs(procDIR)
    # connect to the url where root files are stored    
    BaseURL = 'http://red-gridftp11.unl.edu/Snowmass/HTBinned/Delphes-3.0.9.2/NoPileUp/'+process+'-4p-'+HTbin+'_100TEV/'
    url = BaseURL + '?C=S;O=D' # arrange files by size
    print 'Connecting to ' + url
    source = urllib2.urlopen(url).read()
    soup = bs4.BeautifulSoup(source,'lxml')
    link_list = [l.get('href') for l in soup.findAll('a')]
    RootFiles = [BaseURL+l for l in link_list if l is not None and l.startswith(process) and l.endswith('root')]
    txtFiles = [f[:-5]+'.txt' for f in RootFiles]
    sumEvents=0
    for i in range(len(txtFiles)):
        tag = str(i+1).zfill(3)
        txtfile = os.path.join(procDIR, tag+'.txt')
        rootfile = os.path.join(procDIR, tag+'.root')
		# check if root file already downloaded
        if os.path.isfile(rootfile) : 
            sumEvents = sumEvents + GetEvents(txtfile)
            if sumEvents > nEvents : break
            continue
        print 'Downloading '+RootFiles[i]
        urllib.urlretrieve(txtFiles[i],txtfile)
        urllib.urlretrieve(RootFiles[i],rootfile)
        sumEvents = sumEvents + GetEvents(txtfile)
        if sumEvents > nEvents : break
    print '***********************************************'
    print 'Downloaded ~ '+str(sumEvents)+' events'
    print 'Files downloaded in ' + os.path.abspath(procDIR)
    print '***********************************************'

#-----------------------------------------------------------
if __name__ == "__main__":
	# to download root files with ~100,000 events for 'ttB' background in the HT bin '9000-100000' : 
	DownloadRoots('ttB','9000-100000',100000,'your_data_dir')
