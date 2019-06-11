// Basic Caesar substitution Cipher

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>


//Function Prototypes
void encode (char *, char *, int);
void decode (char *, char *, int);


int main (int argc, char **argv)
{
          
      if (argc != 5)
      {
	  printf ("Usage: <plaintext file> <ciphertext file> <(e)ncode/(d)ecode> <offset>\n");
	  exit (1);
      }
      
      if ((*argv[3] == 'e'))
	  encode (argv[1], argv[2], atoi(argv[4]));
      else
	  decode (argv[1], argv[2], atoi(argv[4]));
      exit (0);
}

void encode (char *InFile, char *OutFile, int offset)
{
      int ch;
      FILE *fp1, *fp2;
      
      if ((fp1 = fopen (InFile, "r")) == NULL)
      {
	  perror ("InFile");
	  exit (1);
      }
      if ((fp2 = fopen (OutFile, "w")) == NULL)
      {
	  perror ("OutFile");
	  exit (1);
      }
  
      while ((ch = fgetc (fp1)) != EOF)
      {
	   ch = tolower (ch);
	   if (isalpha (ch)) 	// don't shift numerals
	  {
	      ch += offset; // shift the character position forward by "offset" positions
	      if (ch > 'z')
		  ch -= 26;	// wrap around after 'z'
	  }
	  putc (ch, fp2);
      }
      fclose (fp1);
      fclose (fp2);
}

void decode (char *InFile, char *OutFile, int offset)
{
     int ch;
      FILE *fp1, *fp2;
      
      if ((fp1 = fopen (InFile, "r")) == NULL)
      {
	  perror ("InFile");
	  exit (1);
      }
      if ((fp2 = fopen (OutFile, "w")) == NULL)
      {
	  perror ("OutFile");
	  exit (1);
      }
    
       while ((ch = fgetc (fp1)) != EOF)
      {
	   ch = tolower (ch);
	   if (isalpha (ch)) 	// don't shift numerals
	  {
	      ch -= offset; // shift the character position forward by "offset" positions
	      if (ch < 'a')
		  ch += 26;	// wrap around after 'z'
	  }
	  putc (ch, fp2);
      }
      fclose (fp1);
      fclose (fp2);
}
