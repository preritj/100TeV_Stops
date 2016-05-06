### Installing Madgraph :
* Download madgraph from the web : 
```
wget https://launchpad.net/mg5amcnlo/2.0/2.3.0/+download/MG5_aMC_v2.3.3.tar.gz 
tar -zxvf MG5_aMC_v2.3.3.tar.gz
```

* To include S<sub>T</sub>  cut, some madgraph files need to be modified. I have placed these modified files (with paths) in `MadGraph` folder, replace your original MadGraph files with the ones included here. To download files from github, click on the `Raw` button and copy the url, and then use `wget <url>` in terminal.

* Finally check that everything works by doing :
```
./bin/mg5_aMC
```
and try a simple test process : 
```
generate p p > z
output test
quit
```

### Installing Pythia 6 :
* If the above step works, install Pythia 6 inside Madgraph by issuing the following command at *Madgraph prompt* (i.e. after running `./bin/mg5_aMC` )
```
install pythia-pgs
```

### Installing Delphes :
* Next we want to install Delphes, but outside Madgraph to get the latest version. Do the following steps *outside* of your Madgraph directory :
```
wget http://cp3.irmp.ucl.ac.be/downloads/Delphes-3.3.2.tar.gz
tar -zxvf Delphes-3.3.2.tar.gz
```

* Delphes requires `root`, which is installed in the cluster *but* it has to be loaded. To load `root`, use the following command in terminal :
```
module load root/5.34
``` 

* Now install Delphes. Go inside the Delphes directory and do :
```
./configure
make
```

* One last step, we have to make Madgraph aware of the Delphes path. Open the file `input/mg5_configuration.txt` in the Madgraph directory. Then, uncomment and edit the following line 
```
delphes_path = <your Delphes directory>
``` 
