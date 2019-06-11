#!/bin/python3
import getopt
import sys

KEY = 66

def f(t, i):
    return pow((t * i * 2), 12) % 22

def run_block(block):
    L = block[0:4]
    R = block[4:8]
    LOut = [0,0,0,0]
    ROut = [0,0,0,0]
    for i in range(4):
        LOut[i] = f(R[i], i+1)
    return LOut, L

def rounds(blocks):
    out = []
    for block in blocks:
        print("\nBlock: ", block)
        prev_block = block
        for i in range(8):
            L,R = run_block(prev_block)
            prev_block = L+R
        out += prev_block
    return out

def decrypt(cipher):
    pass

def main():
    try:
        opts, args = getopt.getopt(sys.argv[1:], "i:ed")
        text = None
        encrypt = False
        decrypt = False
    except getopt.GetoptError:
        sys.exit(1)

    for opt, arg in opts:
        if opt == "-i":
            text = str(arg)
            text = [ord(i) for i in text]
        elif opt == "e":
            encrypt = True
            decrypt = False
        elif opt == "d":
            decrypt = True
            encrypt = False
    
    if text == None:
        exit()

   
    if len(text) < 8:
        text += [0] * (8 - len(text))
    elif len(text) % 8 != 0:
        text += [0] * (8 - (len(text) % 8))


    blocks = [text[i:i+8] for i in range(0, len(text), 8)]

    cipher = rounds(blocks)
    output = " "
    for c in cipher:
        output += str(c)
        output += " "
    with open("out.cip", "w+") as file:
        file.write(output)
        file.flush()

if __name__ == "__main__":
    main()