python ${ADC_SITCP_WFDM_REG}/set_registers.py

echo 'Start data taking for 1 sec. (10 times)'
for i in `seq 1 10`
do
    timeout 1 nc 192.168.10.16 24 > trigRateTest1sec
    ${ADC_SITCP_WFDM_DEC}/decode trigRateTest1sec trigRateTest1sec.root
done

echo 'Start data taking for 5 sec. (10 times)'
for i in `seq 1 10`
do
    timeout 5 nc 192.168.10.16 24 > trigRateTest5sec
    ${ADC_SITCP_WFDM_DEC}/decode trigRateTest5sec trigRateTest5sec.root
done

echo 'Start data taking for 10 sec. (10 times)'
for i in `seq 1 10`
do
    timeout 10 nc 192.168.10.16 24 > trigRateTest10sec
    ${ADC_SITCP_WFDM_DEC}/decode trigRateTest10sec trigRateTest10sec.root
done

echo 'Start data taking for 20 sec. (10 times)'
for i in `seq 1 10`
do
    timeout 20 nc 192.168.10.16 24 > trigRateTest20sec
    ${ADC_SITCP_WFDM_DEC}/decode trigRateTest20sec trigRateTest20sec.root
done

echo 'Start data taking for 30 sec. (10 times)'
for i in `seq 1 10`
do
    timeout 30 nc 192.168.10.16 24 > trigRateTest30sec
    ${ADC_SITCP_WFDM_DEC}/decode trigRateTest30sec trigRateTest30sec.root
done

echo 'Start data taking for 40 sec. (10 times)'
for i in `seq 1 10`
do
    timeout 40 nc 192.168.10.16 24 > trigRateTest40sec
    ${ADC_SITCP_WFDM_DEC}/decode trigRateTest40sec trigRateTest40sec.root
done

echo 'Start data taking for 50 sec. (10 times)'
for i in `seq 1 10`
do
    timeout 50 nc 192.168.10.16 24 > trigRateTest50sec
    ${ADC_SITCP_WFDM_DEC}/decode trigRateTest50sec trigRateTest50sec.root
done

echo 'Start data taking for 60 sec. (10 times)'
for i in `seq 1 10`
do
    timeout 60 nc 192.168.10.16 24 > trigRateTest60sec
    ${ADC_SITCP_WFDM_DEC}/decode trigRateTest60sec trigRateTest60sec.root
done

echo 'Start data taking for 70 sec. (10 times)'
for i in `seq 1 10`
do
    timeout 70 nc 192.168.10.16 24 > trigRateTest70sec
    ${ADC_SITCP_WFDM_DEC}/decode trigRateTest70sec trigRateTest70sec.root
done

echo 'Start data taking for 80 sec. (10 times)'
for i in `seq 1 10`
do
    timeout 80 nc 192.168.10.16 24 > trigRateTest80sec
    ${ADC_SITCP_WFDM_DEC}/decode trigRateTest80sec trigRateTest80sec.root
done

echo 'Start data taking for 90 sec. (10 times)'
for i in `seq 1 10`
do
    timeout 90 nc 192.168.10.16 24 > trigRateTest90sec
    ${ADC_SITCP_WFDM_DEC}/decode trigRateTest90sec trigRateTest90sec.root
done

echo 'Start data taking for 100 sec. (10 times)'
for i in `seq 1 10`
do
    timeout 100 nc 192.168.10.16 24 > trigRateTest100sec
    ${ADC_SITCP_WFDM_DEC}/decode trigRateTest100sec trigRateTest100sec.root
done
