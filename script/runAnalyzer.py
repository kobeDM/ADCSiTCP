#!/usr/bin/env python3

import os, sys, subprocess
import glob

DECODE_BIN = os.environ["ADC_SITCP_WFDM_DEC"] + "/decode"
CHAIN_SRC = os.environ["ADC_SITCP_WFDM_MAC"] + "/chain.cc"
PLOT_WF_SRC = os.environ["ADC_SITCP_WFDM_MAC"] + "/dumpAllWaveforms.cc"

chain_list_name = "chainList.txt"


def make_dir(path):
    if os.path.exists(path):
        print("Directory", path, "already exists")
        print("Deleting directory", path, "...")
        cmd = ["rm", "-rf", path]
        subprocess.run(cmd)
    print("Creating directory", path, "...")
    os.makedirs(path)


def main():

    print("### run ADCSiTCP Analyzer ###")

    print()
    print("--- Read configurations ---")

    if (len(sys.argv) < 2):
        print("Usage: runAnalyzer [data dir] [analyze dir]")
        exit(1)

    data_dir = sys.argv[1]
    ana_dir = sys.argv[2]

    print("data directory:", data_dir)
    print("output directory:", ana_dir)
    print()

    print("--- Decode data files ---")

    rootfile_dir = ana_dir + "/rootfile"
    chain_list_path = rootfile_dir + "/" + chain_list_name
    make_dir(rootfile_dir)

    datafiles = glob.glob(data_dir+"/*.dat")

    with open(chain_list_path, "w") as file:
        for datafile in datafiles:
            print()
            print("Processing", datafile, "...")
            rootfile = rootfile_dir + datafile.replace(data_dir, "").replace(".dat", ".root")
            cmd = [DECODE_BIN, datafile, rootfile]
            subprocess.run(cmd)
            file.write(rootfile + "\n")
        
    print()
    print("--- Chain root files ---")

    shell_cmd = "root"
    options = "-b -l -q"
    root_cmd = "\'" + CHAIN_SRC + "(\"" + chain_list_path + "\")\'"
    
    cmd = shell_cmd + " " + options + " " + root_cmd
    subprocess.run(cmd, shell=True)

    cmd = ["mv", "chain.root", rootfile_dir]
    subprocess.run(cmd)

    print()
    print("--- Plot waveforms ---")

    plot_dir = ana_dir + "/plot"
    make_dir(plot_dir)

    rootfile_path = rootfile_dir + "/chain.root"
    root_cmd = "\'" + PLOT_WF_SRC + "(\"" + rootfile_path + "\", \"" + plot_dir + "\")\'"

    cmd = shell_cmd + " " + options + " " + root_cmd
    subprocess.run(cmd, shell=True)

if __name__ == "__main__":
    main()