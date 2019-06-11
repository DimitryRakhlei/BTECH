
/*----------------------------------------------------------------------------------------------------------------------
--	Source File:		brkrand.c - Code breaker for the random substitution Cipher
--
--	Functions:		BruteForce: Given the correct random string it locates the index pointing to the numerical 
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
--					- Modified and modernized the BruteForce and LocateIndex functions. 
--	DESIGNERS:		Aman Abdulla
--				
--
--	PROGRAMMER:		Aman Abdulla
--
--	NOTES:
--	The program isa simple variation of the random.c program. The ciphertext is read from a file and 
--	aand the user is prompted for a key (randomized alphabet). the ciphertext is written to another 
--	user-specified file.    
--	
--	To compile : gcc -Wall -o brkrand brkrand.c
--			
-------------------------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>

#define MAXSUB		28
#define MAXLINE	 	80

//Function Prototypes
void BruteForce (char *);
int LocateIndex (char *, char );

char sub[MAXSUB];
char alphabet[MAXSUB] = "abcdefghijklmnopqrstuvwxyz";

int main (int argc, char **argv)
{
      unsigned char resp;
      char ch;
      int i;
  
      if (argc != 2)
      {
	  printf ("Usage: <ciphertext file>\n");
	  exit (1);
      }
      
      do
      {
	  fflush (stdin);
	  i = 0;
	  printf ("\nEnter test alphabet: ");
	  while ((ch = getchar()) != '\n')
	  {
                if (i < MAXSUB)    // stay within the buffer
		  sub[i++] = ch;
	  }
	  sub[i] = '\0';
	  BruteForce (argv[1]);
	  fflush (stdin);
	  printf ("\nDecoded Yet? (y/n): ");
	  system ("stty raw"); 	// Avoid the messy Enter/Newline stuff!
	  resp = getchar ();
	  system ("stty cooked");
      } while (resp != 'y');
      printf ("\n");
      exit (0);
}

/*-------------------------------------------------------------------------------------------------------------------------*/

void BruteForce (char *InFile)
{
      FILE *fp1;
      unsigned char ch;
           
      if ((fp1 = fopen (InFile, "r")) == NULL)
      {
	  perror ("InFile");
	  exit (1);
      }

      while ((ch = fgetc (fp1)) != EOF)
      {
	  if (ch == '\n')
	    break;
	  ch = tolower (ch);
	  if (isalpha (ch) || ch == ' ') 			// ignore numerals and whitespace
	      printf ("%c", alphabet[LocateIndex(sub, ch)]); 	// get the correct index
	  fflush (stdout);
      }
      fclose (fp1);
}

/*-------------------------------------------------------------------------------------------------------------------------*/

int LocateIndex (char *str, char ch)
{
      static int i;
      
      for (i = 0; i < MAXSUB; i++)
	  if (ch == str[i])
	        return i;
	  return -1;
}