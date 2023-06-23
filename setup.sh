TMP=`pwd -P`
export PATH=$TMP/script:$PATH
export ADC_SITCP_WFDM_REG=$TMP/wfdm16/register
export ADC_SITCP_WFDM_DEC=$TMP/wfdm16/decode
export ADC_SITCP_WFDM_DAQ=$TMP/wfdm16/daq
export ADC_SITCP_WFDM_MAC=$TMP/wfdm16/macros

export PATH=$ADC_SITCP_WFDM_DAQ:$PATH
