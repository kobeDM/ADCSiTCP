python ../register/set_registers.py

timeout 1 nc 192.168.10.16 24 > data

./decode -t data
cp ch_*.txt ./tmp/
root -q -b -l 'dumpWaveform.cc("tmp",2)'
