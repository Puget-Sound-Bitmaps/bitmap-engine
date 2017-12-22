##Binary to Hex and Hex to Binary Script
##Works on string representations of binary and hex values in .txt files, prints results to your terminal
##Ian White 2017

import os 
import sys
import binascii

global byte_array
global init_byte
global hex_list
global bin_list
global convert

def main():
	dir_path = os.getcwd()
	try:
		file_name = sys.argv[1]	
		convert = sys.argv[2]
		name_num = sys.argv[3]
	except:
		print("Usage: ")
		print("binhextrans.py [filename] [hex/bin] [num]")
		print("where [filename] is the name of the file,")
		print("[hex/bin] are to translate from hex to binary or from binary to hexadecimal, respectively")
		print("and [num] is the number to he appended the the output filename, for identification purposes.")
		sys.exit()


	full_path = dir_path + "/" + file_name
	file = open(full_path, "r");
	content = file.read();
	content = content.split()
	content = ''.join(content)
	res_file = open('converted_test'+name_num+'.dat', 'w')

	if convert == 'bin':
		print("Converting from binary to hexadecimal...")
		len_block = len(content)
		num_bytes = len_block/8
		print("Number of bytes: " + str(num_bytes))
		print("Raw data:")
		print(content)
		i = 0
		j = 0
		byte_array = []
		init_byte = []
		for bit in content:
			i = i + 1
			init_byte.append(bit)
			if i == 8:
				#print(init_byte)
				byte_array.append(init_byte)
				i = 0
				j = j + 1
				init_byte = []
			#4 bits at a time, convert those four bits to hex representation.
			#for i in range(0,7):

		#print(byte_array)

		hex_list = []
		for byte in byte_array:
			byte = ''.join(byte)
			#print(byte)
			int_rep = int(byte, 2)
			hex_rep = '%02X' % int_rep
			hex_list.append(hex_rep)
			res_file.write(hex_rep)

		#print(hex_list)
		res_file.close()
		hex_string = ''.join(hex_list)
		hex_list = hex_string.split('x')
		hex_string = ''.join(hex_list)
		print("Hex output:")
		print(hex_string)
		#print("Creating test file bbc_tester2.dat")

	elif convert == 'hex':
		print("Converting from hexadecimal to binary...")
		len_block = len(content)
		num_bytes = len_block/2
		print("Number of bytes: " + str(num_bytes))
		print("Raw data:")
		print(content)
		i = 0
		j = 0
		byte_array = []
		init_byte = []
		for bit in content:
			i = i + 1
			init_byte.append(bit)
			if i == 2:
				#print(init_byte)
				byte_array.append(init_byte)
				i = 0
				j = j + 1
				init_byte = []
			#4 bits at a time, convert those four bits to hex representation.
			#for i in range(0,7):

		#print(byte_array)

		bin_list = []
		num_of_bits = 8
		for byte in byte_array:
			byte = ''.join(byte)
			#print(byte)
			#int_rep = int(byte, 2)
			#hex_rep = '%02X' % int_rep
			bin_rep = bin(int(byte, 16))[2:].zfill(8)
			bin_list.append(bin_rep)
			res_file.write(bin_rep)

		res_file.close()
		bin_string = ' '.join(bin_list)
		print("Binary output:")
		print(bin_string)




# display some lines

if __name__ == "__main__": main()