# ADCSiTCP

## SETUP (assuming you are in na8)

- Go to DATA directory. 
```
$ cd /home/msgc/work/cn1/data // if you want to take data with C/N-1.0
```

```
$ cd /home/msgc/work/cn_tc/data // if you want to take data with C/N-TC
```

- Setup ADCSiTCP package

```
$ ln -s /home/msgc/sw/repo/ADCSiTCP ./
$ cd ADCSiTCP
$ source ./setup.sh
$ cd ../
```

- Create run directory

```
$ mkdir runYYYYMMDD
$ cd runYYYYMMDD
$ mkdir YYYYMMDD // if you want to define sub-run by dates
$ cd YYYYMMDD
````

## START DAQ

```
$ runDAQ.py -r [RUN TYPE e.g. DM, 252Cf, 241Am, ... (default: work)] -n [#events per file (default: 1000)] 
```
