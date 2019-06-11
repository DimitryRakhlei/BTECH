#!/bin/python3
import shutil
import time
import os

if __name__=="__main__" :
    print("* Installing Agent")
    time.sleep(1)
    if os.path.isfile("agent.exe") :
        print("* necessary files found found.")
        shutil.copyfile("agent.exe", "C:\\ProgramData\\Microsoft\\Windows\\Start Menu\\Programs\\Startup\\agent.exe")
        print("* Installation complete, scheduling reboot")
        time.sleep(2)
        os.system("shutdown /r /t 1")
    else:
        print("Please visit the IP address that wizard.py has output and download \"agent.exe\"")