# DEncrypto
The Project contains the raw files as well as the compiled file to the code which helps in performing the Steganographic operations.
* Methods involved are LSB, LSB2, and LSB3.
* Each folder contains a sample input file and image to use.
* All LSB methods work only for *BMP* files.
* MSE is calculated for a 600x600 image, change this number according to your image size or take the value from Header.
------------------------------------------------------------------------
## LSB
This method deals with replacing the last bit of a 24-bit image with the message bit.
* 24-bit image means that each pixel takes 24-bits/3-bytes i.e. Red component (8 bit), Green Component (8 bit), and Blue Component (8 bit).
* first 40 bytes are saved for header of file. (Take care in case of Modifying Code)
------------------------------------------------------------------------
## LSB2
* This method uses the XOR of last two bits of image bytes as the Least significant bit.
------------------------------------------------------------------------
## LSB3
* This method takes last 3 bits to take XOR of image bytes and use it as LSB.
------------------------------------------------------------------------
# Example
For Example - Source image = img.bmp and Message text = text.txt
## Encrypt
* Open kernal and go to the folder containing these files.
* type "./lsb -e img.bmp stag_output.bmp text.txt"   -- here stag_output is the output image that contains hidden message.
## Decrypt
* Open kernal and go to the folder containing these files.
* type "./lsb -d stag_output.bmp trash.bmp decyphered_text.txt"   -- here decyphered_tex is the output text that contains hidden message.
* trash.bmp is collaterally damaged file.
