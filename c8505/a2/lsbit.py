import sys
import numpy
from util import usage, bits_to_bin, bin_to_bits, set_bit
from PIL import Image
from encrypt import AESCipher


'''
Desc: Encrypts a payload and hides it inside of the lease
      significant bits of the container.
container-> image which we will hide data inside of
payload-> file which will be hidden
key-> key for the aes-256 encryption
'''
def hide(container, payload, key):
	img = Image.open(container)
	(width, height) = img.size
	conv = img.convert("RGBA").getdata()
	print("Input image size: %dx%d pixels." % (width, height))
	max_size = width*height*3.0/8/1024		
	print ( "Usable payload size: %.2f KB." % (max_size))

	f = open(payload, "rb")
	data = f.read()
	f.close()
	print("Payload size: %.3f KB " % (len(data)/1024.0))
		
	cipher = AESCipher(key)
	data_enc = cipher.encrypt(data)

	v = bin_to_bits(data_enc)
	
	while(len(v)%3):
		v.append(0)

	payload_size = len(v)/8/1024.0
	print ("Payload size after encryption: %.3f KB " % (payload_size))
	if (payload_size > max_size - 4):
		print ("File too large")
		sys.exit()

	steg_img = Image.new('RGBA',(width, height))
	data_img = steg_img.getdata()

	idx = 0

	for h in range(height):
		for w in range(width):
			(r, g, b, a) = conv.getpixel((w, h))
			if idx < len(v):
				r = set_bit(r, 0, v[idx])
				g = set_bit(g, 0, v[idx+1])
				b = set_bit(b, 0, v[idx+2])
			data_img.putpixel((w,h), (r, g, b, a))
			idx = idx + 3
    
	steg_img.save(container + "_encrypted.png", "PNG")
	
	print("%s hidden" % payload)


"""
Desc: This function reveals the hidden image passed in as in_file
Args:
in_file => the carrier file which contains the hidden image or message
out_file => the file into which we will output the hidden message
key => password for the encryption module
"""
def reveal(in_file, out_file, key):

	img = Image.open(in_file)
	(width, height) = img.size
	data = img.convert("RGBA").getdata()
	print ("Image size: %dx%d pixels." % (width, height))

	v = []
	for h in range(height):
		for w in range(width):
			(r, g, b, a) = data.getpixel((w, h))
			v.append(r & 1)
			v.append(g & 1)
			v.append(b & 1)
			
	data_out = bits_to_bin(v)

	cipher = AESCipher(key)
	data_dec = cipher.decrypt(data_out)

	out_f = open(out_file, "wb")
	out_f.write(data_dec)
	out_f.close()
	
	print (" Wrote the data to: %s." % out_file)




	
if __name__ == "__main__":
	if len(sys.argv) < 3:
		usage(sys.argv[0])
		
	if sys.argv[1] == "hide":		
		hide(sys.argv[2], sys.argv[3], sys.argv[4])
	elif sys.argv[1] == "reveal":
		reveal(sys.argv[2], sys.argv[3], sys.argv[4])
	elif sys.argv[1] == "help" :
		usage(sys.argv[0])
	else:
		usage(sys.argv[0])
