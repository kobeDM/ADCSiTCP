# ADCSiTCP

## SETUP (assuming you are in na12)

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
$ source setup.sh
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
$ runAllWaveforms.sh [BINARY FILE NAME] [OUTPUT DIRECTORY] [DAQ TIME]

e.g. ...
$ runAllWaveforms.sh data runPer1 100 ## DAQ will run for 100s 
```
