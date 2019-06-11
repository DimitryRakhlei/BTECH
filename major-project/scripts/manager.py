#!/bin/python3
import libvirt
from pexpect import *
import os
import errno
import sys
import time
import subprocess
import argparse
import json

def systemctl_start(process):
    if subprocess.call(['sudo', '/bin/systemctl', 'status', process], stdout=subprocess.DEVNULL) != 0:
        if subprocess.call(['sudo', '/bin/systemctl', 'start', process]) == 0:
            print("\n-> Restarting {}\n".format(process))
            time.sleep(1)

def systemctl_status(process):
    subprocess.call(['sudo', '/bin/systemctl', 'status', process])

def virsh_call(args, null=False):
    if null:
        if subprocess.call(['sudo', '/usr/bin/virsh'] + args, stdout=subprocess.DEVNULL) != 0:
            pass
    else:
        if subprocess.call(['sudo', '/usr/bin/virsh'] + args) != 0:
            pass


if __name__ == "__main__":
    if os.getuid() != 0:
        print("need to be root to manage virtual machines, try: \"sudo " +
              sys.argv[0] + "...\"")
        exit(1)

    proj_root_path = "./"  # root path assumed to be here

    parser = argparse.ArgumentParser()
    main_group = parser.add_argument_group("Arguments")
    
    main_group.add_argument("-net-status", action="store_true", 
                            help="Status of the Default Network")
    
    main_group.add_argument("-virt-start", action="store_true", 
                            help="Start libvirt services.")
    
    main_group.add_argument("-virt-status", action="store_true", 
                            help="Show Status of libvirt services.")
    
    main_group.add_argument("--run", help="Launch VM and Viewer")
    main_group.add_argument("--command", help="Command to run at execution")
    
    main_group.add_argument("-initialize", action="store_true",
                            help="Automatically download and configure the project. May require a few actions from the user.")
    
    main_group.add_argument("--project-root", type=str,
                            help="the path to this project  if running from project root can use ./")
    
    args = parser.parse_args()

    print(args)
    if args.initialize:
        if args.project_root:
            proj_root_path = os.path.abspath(args.project_root)
        else:
            proj_root_path = os.path.abspath(proj_root_path)
        
        if os.path.isfile(proj_root_path + "/scripts/wizard.sh"):
            call_args = [proj_root_path +
                         "/scripts/wizard.sh", proj_root_path]
            subprocess.run(call_args)
 
    if args.run:
        if (args.run is not None and args.command is not None):
            command = [os.path.abspath("scripts/sandbox.py"), "-k", os.path.abspath("./config/hooks/default.json"), 
            "-c", "\"" + args.command + "\"", os.path.abspath("./config/contexts/win7-goat.json"), args.run]
            print(command)
            subprocess.call(command)
        else:
            print("Error: You must specify the binary under --run and the comand under --command. Default command is \'start {sample}\'")
                
    if args.net_status:
        virsh_call(["net-info", "default"])
    
    if args.virt_start:
        systemctl_start("libvirtd")
        systemctl_start("libvirt-guests")
        virsh_call(["net-start", "default"])
    
    if args.virt_status:
        systemctl_status("libvirtd")
        systemctl_status("libvirt-guests")
