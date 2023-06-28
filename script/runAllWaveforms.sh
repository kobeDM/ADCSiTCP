BINARY=$1
OUT_DIR=$2
DAQ_TIME=$3

python ${ADC_SITCP_WFDM_REG}/set_registers.py

timeout ${DAQ_TIME} nc 192.168.10.16 24 > ${BINARY}

${ADC_SITCP_WFDM_DEC}/decode ${BINARY} ${OUT_DIR}.root

if [ -d ${OUT_DIR} ]; then
    echo dir-${OUT_DIR} already exists. delete.
    rm -rf ${OUT_DIR}
fi
mkdir ${OUT_DIR}
mv ${BINARY} ./${OUT_DIR}/
mv ${OUT_DIR}.root ./${OUT_DIR}/
cd ./${OUT_DIR}
ln -s ../../ADCSiTCP/ShUtil
source ShUtil/script/setup_shutil.sh



#EXE=${ADC_SITCP_WFDM_MAC}/dumpAllWaveformsRoot.cc
#EXE=${ADC_SITCP_WFDM_MAC}/dumpAllWaveformsRoot4.cc
#root -q -b -l "${EXE}(\"${OUT_DIR}/${OUT_DIR}.root\",\"${OUT_DIR}/${OUT_DIR}\")"
#root -q -b -l "${EXE}(\"${OUT_DIR}/${OUT_DIR}.root\")"
#root -q -b -l "${EXE}(\"${OUT_DIR}.root\")"
