#!/bin/python3

import trdecode
import trencode
import detectEnglish
import getopt
import sys


def main():
    try:
        opts, args = getopt.getopt(sys.argv[1:], "ic:m:t:h")
        maxsize = None
        ciphertext = None
    except getopt.GetoptError:
        sys.exit(1)

    for opt, arg in opts:
        if opt == "-i":
            maxsize = int(input("Enter max size: "))
            ciphertext = input("Enter cipher text: ")
            break_cipher(ciphertext, maxsize)
            exit(1)
        elif opt in '-h':
            print(
                'Usage: ' + sys.argv[0] + ' -c <cipher> -m <maxsize> -t <lowthresh>:<highthresh> -i for interactive input')
            sys.exit()
        elif opt in ("-c", "--ctext"):
            ciphertext = arg
        elif opt in ("-m", "--maxsize"):
            maxsize = int(arg)
        elif opt in ("-t"):
            lt, ht = arg.split(":")
            lt = int(lt)
            ht = int(ht)
            thresh = ((min(lt, ht), max(lt, ht)))
            print(thresh)
    break_cipher(ciphertext, maxsize, thresh)


# attempts to decode once and returns the result
def attempt_decode(cipher: str, size: int, thresh: tuple):
    plaintext = trdecode.decryptMessage(size, cipher)
    return (detectEnglish.FindEnglish(plaintext, wordPercentage=thresh[0], letterPercentage=thresh[1]), plaintext, size)


# continuously attempts to decode from 1:len(cipher)
def break_cipher(cipher: str, maxsize: int, thresh: tuple = (20, 80)):
    for size in range(1, min(maxsize, len(cipher))):
        result = attempt_decode(cipher, size, thresh)
        if result[0] == True:
            uinput = ""
            while True:
                uinput = input("Is this the decoded message?\n[\"" + str(
                    result[1]) + "\"] with a key size of: " + str(result[2]) + "\n[y/n]>")
                print(uinput)
                if uinput == "y":
                    exit(1)
                else:
                    print("Trying Again")
                    break


if __name__ == "__main__":
    main()
