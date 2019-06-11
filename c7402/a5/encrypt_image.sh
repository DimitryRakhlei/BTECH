#!/bin/sh

if [ $# -ne 3 ]; then
	echo "./encrypt_image.sh <image viewer path, ex /bin/feh> <image file> <encryption key 4 chars, ex 'abcd'>"
	echo "example: ./encrypt_image.sh /bin/feh cat.bmp abcd"
	exit
fi

mkdir -p display

rm display/*

touch display/cat.hdr
touch display/cat.data


dd if=$2 of=display/cat.hdr bs=1 count=54 status=progress
dd if=$2 of=display/cat.data bs=1 skip=54 status=progress




./bin/feistel -e -m ecb -i display/cat.data -o display/cat.enc -k $3

cat display/cat.hdr display/cat.enc > display/cat_ecb.bmp

$1 display/cat_ecb.bmp &



./bin/feistel -e -m cbc -i display/cat.data -o display/cat.enc -k $3

cat display/cat.hdr display/cat.enc > display/cat_cbc.bmp

$1 display/cat_cbc.bmp &


./bin/feistel -e -m ctr -i display/cat.data -o display/cat.enc -k $3

cat display/cat.hdr display/cat.enc > display/cat_ctr.bmp

$1 display/cat_ctr.bmp &

$1 $2 &
