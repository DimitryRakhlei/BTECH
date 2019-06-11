from sys import exit
import struct

"""
Desc: Writes bits to a binary and returns it
"""
def bits_to_bin(v):    
	data = b""

	length = len(v)
	for idx in range(0, int(len(v)/8)):
		byte = 0
		for i in range(0, 8):
			if (idx*8+i < length):
				byte = (byte<<1) + v[idx*8+i]                
		data = data + bytes([byte])

	payload_size = struct.unpack("i", data[:4])[0]

	return data[4: payload_size + 4]

"""
Desc: Splits a binary into bits and returns them
"""
def bin_to_bits(data):
	v = []
	# Pack file len in 4 bytes
	fSize = len(data)
	bytes = [b for b in struct.pack("i", fSize)]
	
	bytes += [b for b in data]

	for b in bytes:
		for i in range(7, -1, -1):
			v.append((b >> i) & 0x1)

	return v

def set_bit(n, i, x):
	mask = 1 << i
	n &= ~mask
	if x:
		n |= mask
	return n


def usage(progName):
	print ("Usage:")
	print ("  %s hide <img_file> <payload_file> <password>" % progName)
	print ("  %s reveal <stego_file> <out_file> <password>" % progName)
	exit()