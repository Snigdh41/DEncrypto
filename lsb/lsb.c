#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Calculate Header Offset
int img_offset(FILE* img)
{
	fseek(img,14,0);
	int offset;
	offset = (int)fgetc(img);
	return offset;
}

// Calculate Message Length of Text
int msg_size(FILE *text)
{
	fseek(text, 0L, SEEK_END);
	int size = ftell(text);
	fseek(text, 0L, SEEK_SET);
	return(size);
}

// To Get a Specific bit of Message bytes
int get_bit(char bytee,int pos)
{
	return((bytee>>8-pos)&1);
}

// Main - Runner of Code
int main(int argc,char** argv)
{
	// img_read - The input Image ( or Source Image)
	// msg_file - The File Containing the Text to be Hidden
	// img_write - The Image file that will be created after hiding the message
	FILE *img_read, *msg_file, *img_write;
	//-------------------------------------------------Input According to a certain Commanline Pattern-------------------------------------------------
	if(argc != 5)
	{
		printf("~~~~~~~~~Steganography by LSB substitution~~~~~~~~~\nUsage: %s [-e][-d] <Input Image> <OutPut Image> <Message Text File>\n-e : Add text to image\n -d : Get text from image\n",argv[0]);
		exit(1);
	}

	// Showing Options according to Encryption and Decryption choices made
	int action;
	if(!strcmp(argv[1],"-e"))
		action = 1;
	else if(!strcmp(argv[1],"-d"))
		action = 0;
	else
	{
		printf("~~~~~~~~~Steganography by LSB substitution~~~~~~~~~\nUsage: %s [-e][-d] <Input Image> <OutPut Image> <Message Text File>\n-e : Add text to image\n -d : Get text from image\n",argv[0]);
		exit(1);
	}

	//-------------------------------------------------Checking Existance of File Names Provided-------------------------------------------------

	// HANDLING IMAGE OPENING AND ERRORS
	img_read = fopen(argv[2],"r");
	if (img_read == NULL)
	{
		fprintf(stderr, "Unable open input image %s\n",argv[2]);
		exit(1);
	}

	// HANDLING IMAGE WRITING AND ERRORS
	img_write = fopen(argv[3],"w");
	if (img_write == NULL)
	{
		fprintf(stderr, "Unable create output image %s\n",argv[3]);
		exit(1);
	}

	// Calculating Image Header Offset and resetting file in Stream i.e. Cursor at Start
	char cpy_header;
	int offset = img_offset(img_read);
	rewind(img_read);

	// Copying the Header Of Original Image
	for(int i = 0;i<offset;i++)
	{
		cpy_header = fgetc(img_read);
		fputc(cpy_header,img_write);
	}

	char img_char,  msg_char;		// Temporary Variables to Store character of Image and Message
	int count = 0;
	//-------------------------------------------------Action ENCRYPT-------------------------------------------------
	if(action)
	{
		msg_file = fopen(argv[4],"r");
		if (msg_file == NULL)
		{
			fprintf(stderr, "Unable open message text file %s\n",argv[4]);
			exit(1);
		}

    // Calculating total length of MESSAGE
		int length = msg_size(msg_file);

	// Writing Message Length in Image when header is finished ( helps to identify how many bytes to read at the time of DECRYPTING)
		fputc(length,img_write);

    // Skiping till first Pixel Byte i.e. Red byte
    fseek(img_read,14,SEEK_CUR);
    fseek(img_write,13,SEEK_CUR);

		do
		{
			int msg_bit;

		// feof - checks if file ended, returns zero if not
			if(!feof(msg_file))
			{
				// Get Byte from Written Text
				msg_char = fgetc(msg_file);
				for(int i = 1;i <= 8;i++)
				{  //Do this for every bit in every byte of the image

					img_char = fgetc(img_read);
					int lsb_bit = img_char & 1;
					msg_bit = get_bit(msg_char,i);

					// Comparing LSB bit with the message bit to be written and Writing in file
					if(lsb_bit == msg_bit)
					{
						fputc(img_char,img_write);
					}
					else
					{
						if(lsb_bit == 0)
							img_char = (img_char | 1);
						else
							img_char = (img_char & 0);
						fputc(img_char,img_write);
						++count;
					}
				}// For ended here-
			}// if(!feof) ended here-
			else
			{
				// Writing if anything else left in file - - - FAIL SAFE METHOD - - -
				cpy_header = fgetc(img_read);
				fputc(cpy_header,img_write);
			}
		} while(!feof(img_read));
		fclose(msg_file);
    printf("Changes in Bytes = %d\n",count);
    double mse = count/(600.0*600.0);
    printf("Mean Squared Error = %g\n",mse);
	}

  //-------------------------------------------------Action DECRYPT-------------------------------------------------
	else
	{
		// Opening and Creating file ( if does not exist already )
		msg_file = fopen(argv[4],"w");
		if (msg_file == NULL)
		{
			fprintf(stderr, "Unable to open text file %s\n",argv[4]);
			exit(1);
		}

		// Fetching Previously Saved Message Length After Header. Readmore at line 101-102
		int message_length = (int)fgetc(img_read);

		// Setting Cursor at Starting point i.e. at the first edited pixel Position. Readmore at line 104-106
		fseek(img_read,13,SEEK_CUR);
		for(int i = 0;i<message_length;i++)
		{
			char temp_ch = '\0'; // declaring null character i.e. 0000 0000 ( or 00 hex)

			// Fetching all 8 bits of Image replaced LSB
			for( int j = 0;j<8;j++)
			{
				temp_ch = temp_ch<<1;
				img_char = fgetc(img_read);
				int lsb_bit = img_char & 1;
				temp_ch |= lsb_bit;
			}
			fputc(temp_ch,msg_file);
		}
		fclose(msg_file);
	}

	// Close Files To prevent Errors
	fclose(img_read);
	fclose(img_write);
}
