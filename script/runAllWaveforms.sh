BINARY=$1
OUT_DIR=$2

python ${ADC_SITCP_WFDM_REG}/set_registers.py

timeout 1 nc 192.168.10.16 24 > ${BINARY}

${ADC_SITCP_WFDM_DEC}/decode ${BINARY}

if [ -d ${OUT_DIR} ]; then
    echo dir-${OUT_DIR} already exists. delete.
    rm -rf ${OUT_DIR}
fi
mkdir ${OUT_DIR}
mv ch_*.txt ./${OUT_DIR}/
mv ${BINARY} ./${OUT_DIR}/

EXE=${ADC_SITCP_WFDM_MAC}/dumpAllWaveforms.cc
root -q -b -l "${EXE}(\"${OUT_DIR}/ch_0.txt\",\"${OUT_DIR}/ch_0\")"
root -q -b -l "${EXE}(\"${OUT_DIR}/ch_1.txt\",\"${OUT_DIR}/ch_1\")"
root -q -b -l "${EXE}(\"${OUT_DIR}/ch_2.txt\",\"${OUT_DIR}/ch_2\")"
root -q -b -l "${EXE}(\"${OUT_DIR}/ch_3.txt\",\"${OUT_DIR}/ch_3\")"
root -q -b -l "${EXE}(\"${OUT_DIR}/ch_4.txt\",\"${OUT_DIR}/ch_4\")"
root -q -b -l "${EXE}(\"${OUT_DIR}/ch_5.txt\",\"${OUT_DIR}/ch_5\")"
root -q -b -l "${EXE}(\"${OUT_DIR}/ch_6.txt\",\"${OUT_DIR}/ch_6\")"
root -q -b -l "${EXE}(\"${OUT_DIR}/ch_7.txt\",\"${OUT_DIR}/ch_7\")"
