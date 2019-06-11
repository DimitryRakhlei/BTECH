
/*----------------------------------------------------------------------------------------------------------------------
--	Source File:		random.c - Random alphabet substitution Cipher
--
--	Functions:		encode: Substitutes each character in the plaintext with a character from the randomized 
--					string, in the same numerical position to generate the ciphertext.
--				decode: Given the correct random string it locates the index pointing to the numerical 
--					position of the ciphertext character and locates the plaintext in the same numerical 
--					position in the alphabet string. 
--				LocateIndex: Function locate the plaintext and ciphertext indices. 
--				
--
--	Date:			October 16, 1988
--
--	Revisions:		(Date and description)
--				
--				November 16, 2012 - Ported from DOS to Linux
--						  - Cleaned up and modernized original code. 
--			
--				November 17, 2012
--					- Modified to ignore whitespace.
--					- Wrote a separate function to locate the indices. 
--
--	DESIGNERS:		Aman Abdulla
--				
--
--	PROGRAMMER:		Aman Abdulla
--
--	NOTES:
--	The program improves the simple substitution cipher by implementing a randomized substitution string. The plaintext 
--	is read from a file and the ciphertext is written to another user-specified file.    
--	
--	To compile : gcc -Wall -o random random.c
--			
-------------------------------------------------------------------------------------------------------------------------*/
#include "random.h"

int main (int argc, char **argv)
{
      if (argc != 4)
      {
	  printf ("Usage: %s <plaintext file> <ciphertext file> <(e)ncode/(d)ecode>\n", argv[0]);
	  exit (1);
      }
      
      if ((*argv[3] == 'e'))
	  encode (argv[1], argv[2]);
      else
	  decode (argv[1], argv[2]);
      exit (0);
}

/*----------------------------------------------------------------------------------------------------------------------*/
void encode (char *InFile, char *OutFile)
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
	  if (isalpha (ch) || ch == ' ') 		// ignore numerals and whitespace
	      ch = sub[LocateIndex(alphabet, ch)];	// get the correct index
	  if (ch != '\0')
	      putc (ch, fp2);
      }
      fclose (fp1);
      fclose (fp2);
}


/*----------------------------------------------------------------------------------------------------------------------*/
void decode (char *InFile, char *OutFile)
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
	  if (isalpha (ch) || ch == ' ') 		// ignore numerals and whitespace
	      ch = alphabet[LocateIndex(sub, ch)]; 	// get the correct index
	  putc (ch, fp2);
      }
      fclose (fp1);
      fclose (fp2);
}

/*----------------------------------------------------------------------------------------------------------------------*/

int LocateIndex (char *str, char ch)
{
      static int i;
      
      for (i = 0; i < MAXSUB; i++)
	  if (ch == str[i])
	      return i;
      return 0;
}