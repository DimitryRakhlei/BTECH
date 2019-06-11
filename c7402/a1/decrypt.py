#!/bin/python3
import task1
from sys import argv

if __name__ == "__main__":
    if len(argv) < 2:
        exit()
    elif len(argv) == 2:
        print("* File reqested:", argv[1])
        try:
            txt = task1.read_file(argv[1])
        except Exception:
            print("! File error")
        else:
            print("* File read, counting letters")
            count_storage = {}
            count_storage[argv[1]] = {}
            count_storage[argv[1]] = task1.count_letters(
                txt)
            count_storage["clear"] = {}
            count_storage["clear"] = task1.count_letters(task1.read_file("books/a_modest_proposal.txt"))
            for book in count_storage:
                total = task1.total
                for letter in count_storage[book]:
