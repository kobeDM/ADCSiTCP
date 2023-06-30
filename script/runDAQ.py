#!/usr/bin/python3

import subprocess, os, sys
import datetime
import time
import glob
import argparse

def parser():
    argparser=argparse.ArgumentParser()
    argparser.add_argument("-r","--run",     help="run type (DM, 252Cf, cal, ...)",           default="work"          )
    argparser.add_argument("-i","--ip",      help="SiTPC IP",                                 default="192.168.10.16" )
    argparser.add_argument("-n","--nevent",  help="The number of events per file",  type=int, default=1000            )
    argparser.add_argument("-t","--timeout", help="DAQ timeout",                    type=int, default=1000            )
    opts=argparser.parse_args()
    return(opts)


def main( ):

    print("### run ADCSiTCP DAQ ###")

    args = parser()
    if( args.nevent != None ):
        num_event = args.nevent
    else:
        num_event = 1000

    if( args.timeout != None ):
        timeout = args.timeout
    else:
        timeout = 1000

    if( args.ip != None ):
        sitcp_ip = args.ip
    else:
        sitcp_ip = "192.168.10.16"
        
    run_type = args.run

    daq_dir = os.environ['ADC_SITCP_WFDM_DAQ']
    daq_cmd = daq_dir + '/daq'

    # loop for preparation to "period"
    while( 1 ):
        period_num=0
        while( os.path.isdir( "per" + str(period_num).zfill(4) ) ):
            period_num+=1

        period_dir = "per" + str(period_num).zfill(4)
        cmd = "mkdir "+period_dir
        proc = subprocess.run( cmd, shell = True )
        print( "New directory: ", period_dir, "is made." )
        max_num_files_per_dir = 100

        file_index=0
        while file_index < max_num_files_per_dir:
            print( file_index, "/", max_num_files_per_dir)
            cmd = daq_cmd + " " + sitcp_ip + " " + period_dir + "/" + run_type + "_" + str(file_index) + ".dat " + str(num_event)
            print( cmd )

            start_time = time.time( )
            proc = subprocess.run( cmd, shell = True )
            end_time = time.time( )

            realtime = end_time - start_time
            rate = float(num_event) / float(realtime)

            rate_filepath = period_dir + "/" + run_type + ".rate"
            insert_line_rate_file = str(end_time) + "\t" + str(rate) + "\n"
            with open( rate_filepath, mode='a' ) as f:
                f.write( insert_line_rate_file )
            
            file_index += 1

    
if __name__ == "__main__":
    main( )





