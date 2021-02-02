python ../register/set_registers.py

timeout 10 nc 192.168.10.16 24 > data

./decode -t data
mv ch_*.txt ./tmp2/
root -q -b -l 'dumpAllWaveforms.cc("tmp2/ch_0.txt")'
