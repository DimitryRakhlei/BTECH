#!/usr/bin/python3.5

# Vigenere Cipher (Polyalphabetic Substitution Cipher)
# This program will encrypt or decrypt a text strings from a file 
# and write the ouput to a file, based on a keyword.

# example usage: ./vgcipher.py -k freedom -e encrypt -i plato2.txt -o crypto2.txt 
# example usage: ./vgcipher.py -k freedom -e decrypt -i crypto2.txt -o foo.txt 

import sys, getopt

LETTERS = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'

def main (argv):
    
    try:
        opts, args = getopt.getopt (argv, "hk:e:i:o:",["keyword=", "mode=", "inputfile=", "outputfile="])
    
    except getopt.getoptError:
        sys.exit (1)
    
    if len (sys.argv[1:]) != 8:
        print ('Usage: ./vgcipher.py -k <keyword> -e <mode> -i <inputfile> -o <outputfile>')
        sys.exit(1)
    for opt, arg in opts:
        if opt == '-h':
            print ('Usage: ./vgcipher.py -k <keyword> -e <mode> -i <inputfile> -o <outputfile>')
            sys.exit (1)
        elif opt in ("-k", "--keyword"):
            KeyWord = arg
        elif opt in ("-e", "--mode"):
            Mode = arg
        elif opt in ("-i", "--inputfile"):
            InFile = arg
        elif opt in ("-o", "--outputfile"):
            OutFile = arg
        
    fpr = open (InFile, 'r')
    TextInput = fpr.read()
    
    if Mode == 'encrypt':
        TextOutput = encryptMessage (KeyWord, TextInput)
    elif Mode == 'decrypt':
        TextOutput = decryptMessage (KeyWord, TextInput)

    print ()
    fp2 = open (OutFile, 'w')
    fp2.write (TextOutput)
    print ('%sed Text has been written to: %s' % ((Mode.title()), OutFile))


def encryptMessage (key, message):
    return translateMessage (key, message, 'encrypt')


def decryptMessage (key, message):
    return translateMessage(key, message, 'decrypt')


def translateMessage (key, message, mode):
    TextOutput = [] # stores the encrypted/decrypted string

    keyIndex = 0
    key = key.upper()

    for symbol in message: # Process each character in the string
        num = LETTERS.find (symbol.upper()) # ensure symbol is an alphabetic character
        if num != -1: 
            if mode == 'encrypt':
                num += LETTERS.find (key[keyIndex]) # add if encrypting
            elif mode == 'decrypt':
                num -= LETTERS.find (key[keyIndex]) # subtract if decrypting

            num %= len (LETTERS) #handle a potential wrap-around

            # append the encrypted/decrypted symbol to the translated string
            if symbol.isupper():
                TextOutput.append (LETTERS[num])
            elif symbol.islower():
                TextOutput.append (LETTERS[num].lower())

            keyIndex += 1 # move to the next letter in the key
            if keyIndex == len (key):
                keyIndex = 0
        else:
            # The symbol is not an alphabetic character; ignore it.
            TextOutput.append (symbol)

    return ''.join (TextOutput)


# main() function.
if __name__ == "__main__":
    main (sys.argv[1:])
