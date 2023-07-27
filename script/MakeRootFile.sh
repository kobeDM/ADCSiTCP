BINARY=$1
FILE_START_NUM=$2
FILE_END_NUM=$3
OUT_DIR=$4

ln -s ../../ADCSiTCP/ShUtil
source ShUtil/script/setup_shutil.sh 

if [ -d ${OUT_DIR} ]; then
    echo ${OUT_DIR} already exists. delete.
    rm -rf ${OUT_DIR}
fi
mkdir ${OUT_DIR}

for i in `seq $((${FILE_START_NUM})) $((${FILE_END_NUM}))`
    do
	echo "now Filenumber:${i}"
	if [ -f ${OUT_DIR}_${i}.root ]; then
    	    echo ${OUT_DIR}.root already exists. delete.
    	    rm ${OUT_DIR}_${i}.root
	fi
	${ADC_SITCP_WFDM_DEC}/decode ${BINARY}_${i}.dat ${OUT_DIR}_${i}.root
    done

ls ${OUT_DIR}_* | xargs hadd ${BINARY}_${FILE_START_NUM}_${FILE_END_NUM}.root
mv ${OUT_DIR}_* ./${OUT_DIR}/

mv ${BINARY}_${FILE_START_NUM}_${FILE_END_NUM}.root ./${OUT_DIR}/
cd ${OUT_DIR}
ln -s ../../../ADCSiTCP/ShUtil
source ShUtil/script/setup_shutil.sh 

#EXE=${ADC_SITCP_WFDM_MAC}/dumpAllWaveformsRoot5.cc
EXE=${ADC_SITCP_WFDM_MAC}/dumpAllWaveformsRoot_ofuji.cc
root -q -b -l "${EXE}(\"${BINARY}_${FILE_START_NUM}_${FILE_END_NUM}.root\")"

cd ..
