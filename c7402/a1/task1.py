#!/bin/python3

from sys import argv
from os import path, chdir, getcwd, mkdir

import glob
import random

try:
    import numpy as np
except ImportError:
    print("numpy is not installed")

try:
    from matplotlib import pyplot as plt
except ImportError:
    print("matplotlib is not installed")

CAN_PLOT = False


def read_file(fname: str):
    return [l.lower() for l in open(fname, "r").read() if l.isalpha() and ord(l) < 123]


def count_letters(text: str):
    storage = {}
    for l in text:
        if not storage.__contains__(l):
            storage[l] = 0
        else:
            storage[l] += 1

    return sorted(storage.items(), key=lambda x: x[1], reverse=True)


def output_csv(name_ending: str, data: dict):
    try:
        mkdir("output")
    except:
        pass
    for book in data:
        with open("output/" + book+name_ending, "w") as outfile:
            print("* Writing CSV to ", book+name_ending)
            outfile.write("Letter,Count\n")
            outfile.flush()
            for (key, value) in data[book]:
                outfile.write(str(key) + "," + str(value) + "\n")
                outfile.flush()


def total(data: dict):
    total = 0
    for (key, value) in data:
        total += value
    return total


def plot_books(storage: dict):
    ax = plt.subplot(111)
    w = (1/len(count_storage)) - (1/(len(count_storage)*5))
    w_pos = (-0.5 * len(count_storage)) * w
    for book in count_storage:
        print("\n* Attempting to plot: ", book)
        count = total(count_storage[book])
        labels = [k for (k, v) in count_storage[book]]
        values = [v/count for (k, v) in count_storage[book]]
        y_pos = np.arange(len(labels))
        test = 0.0
        for v in values:
            test+=v
        plt.bar(y_pos + w_pos, values,  width=w, edgecolor='blue', label=book + " || P(M)=" + str(round(test, 2)))
        w_pos += w
        plt.xticks(y_pos, labels)
    plt.title('Letter Probability Distributions')
    plt.xlabel('Letters')
    plt.ylabel('% Occurance Rate')
    plt.legend()
    plt.show()


if __name__ == "__main__":
    if len(argv) < 2:
        exit()
    elif len(argv) == 2:
        try:
            print("* Import worked", plt)
        except NameError:
            print("! To plot with python install Tkinter python3-tk")
            CAN_PLOT = False
        else:
            print("* We can plot with Python")
            CAN_PLOT = True

        if path.isdir(argv[1]):
            print("* Directory Passed", argv[1])
            print("* Grabbing all text files")
            olddir = getcwd()
            chdir(argv[1])
            text_files = glob.glob("*.txt")
            print("* Got: ", text_files)
            count_storage = {}
            for fname in text_files:
                text = read_file(fname)
                count_storage[fname] = count_letters(text)
            output_csv("_out.csv", count_storage)
            if CAN_PLOT:
                plot_books(count_storage)

        else:
            print("* File reqested:", argv[1])
            try:
                txt = read_file(argv[1])
            except Exception:
                print("! File error")
            else:
                print("* File read, counting letters")
                count_storage = {}
                count_storage[argv[1]] = {}
                count_storage[argv[1]] = count_letters(
                    txt)
                output_csv("_out.csv", count_storage)
                if CAN_PLOT:
                    plot_books(count_storage)
