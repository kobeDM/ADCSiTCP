#!/usr/bin/env python3

import os, sys, subprocess

PLOT_SRC = os.environ["ADC_SITCP_WFDM_MAC"] + "/drawOneEvtWF.cc"

def main():
    print("### run One Event Viewer ###")

    print()
    print("--- Check arguments ---")

    if (len(sys.argv) < 2):
        print("Usage: oneEventViewer.py [root file] [evt ID]")
        exit(1)

    rootfile_path = sys.argv[1]
    evt_id = sys.argv[2]

    print("Root file:", rootfile_path)
    print("Event Id:", evt_id)

    print()
    print("--- Plot waveforms ---")

    shell_cmd = "root"
    options = "-b -l -q"
    root_cmd = "\'" + PLOT_SRC + "(\"" + rootfile_path + "\", " + evt_id + ")\'"
    
    cmd = shell_cmd + " " + options + " " + root_cmd
    subprocess.run(cmd, shell=True)

    # print("test")

if __name__ == "__main__":
    main()