#include <system.h>
#include <string.h>

/* ************************************ *
 * Author:  Forrest Desjardins    	*
 * Creation Date:  2014           	*
 * 				  	*
 *   DISCLAIMER!!!!!:  		  	*
 *  The following methods are provided	*
 *  for your use as part of the MPX	*
 *  project. While we have tested these	*
 *  We make certification of 	 	*
 *  complete correctness.		*
 *   Additionally not all methods are 	*
 *  implemented :  See:  itoa		*
 * ************************************ */


/*
  Procedure..: strlen
  Description..: Returns the length of a string.
  Params..: s-input string
*/
int strlen(const char *s) {
    int r1 = 0;
    if (*s) while (*s++) r1++;
    return r1;//return length of string
}

/*
  Procedure..: strcpy
  Description..: Copy one string to another.
  Params..: s1-destination, s2-source
*/
char *strcpy(char *s1, const char *s2) {
    char *rc = s1;
    while ((*s1++ = *s2++));
    return rc; // return pointer to destination string
}

/*
  Procedure..: atoi
  Description..: Convert an ASCII string to an integer
  Params..: const char *s -- String
*/
int atoi(const char *s) {
    int res = 0;
    int charVal = 0;
    char sign = ' ';
    char c = *s;


    while (isspace(&c)) {
        ++s;
        c = *s;
    } // advance past whitespace


    if (*s == '-' || *s == '+') sign = *(s++); // save the sign


    while (*s != '\0') {
        charVal = *s - 48;
        res = res * 10 + charVal;
        s++;

    }


    if (sign == '-') res = res * -1;

    return res; // return integer
}

/*
  Procedure: itoa
  Description: Converts integers into strings, base 10 only
  Params: s - where converted value will be stored, num - number to be converted
*/
char *itoa(char *s, int num) {
    int i = 0, remainder, temp;

    if (num == 0) {
        s[i] = '0';
        s[i += 1] = '\0';
        return s;
    }

    if (num < 0) {
        s[i] = '-';
        i++;
    }

    if (num > 0) {
        //Gets Ones place
        remainder = num % 10;

        //If num has 3 digits
        if (num > 99 && num <= 999) {
            //Gets 100s place
            temp = num / 100;
            s[i] = temp + '0';

            //Isolates 10s and 1s
            temp = num % 100;
            if (temp < 10)
                s[i += 1] = '0';
            else {
                //Gets 10s Place
                temp = temp / 10;
                s[i += 1] = temp + '0';
            }
            i++;
        }

            //If num has 4 digits
        else if (num > 999) {
            //Gets 1000s place
            temp = num / 1000;
            s[i] = temp + '0';

            //Isolates 100s place down
            temp = num % 1000;
            if (temp < 100)
                s[i += 1] = '0';
            else {
                //Gets 100s place
                temp = temp / 100;
                s[i += 1] = temp + '0';
            }

            //Isolates 10s place
            temp = num % 100;
            if (temp < 10)
                s[i += 1] = '0';
            else {
                //Gets 10s Place
                temp = temp / 10;
                s[i += 1] = temp + '0';
            }
            i++;
        }

            //Num has 1 or 2 digits
        else {
            num = num / 10;
            s[i] = num + '0';
            i += 1;
        }

        s[i] = remainder + '0';
        i++;
    }
    s[i] = '\0';
    return s;
}

/*
  Procedure..: strcmp
  Description..: String comparison
  Params..: s1-string 1, s2-string 2
*/
int strcmp(const char *s1, const char *s2) {

    // Remarks:
    // 1) If we made it to the end of both strings (i. e. our pointer points to a
    //    '\0' character), the function will return 0
    // 2) If we didn't make it to the end of both strings, the function will
    //    return the difference of the characters at the first index of
    //    indifference.
    while ((*s1) && (*s1 == *s2)) {
        ++s1;
        ++s2;
    }
    return (*(unsigned char *) s1 - *(unsigned char *) s2);
}


/* ---------------------------------------
    Functions below this point are given.
    No need to tamper with these!
   --------------------------------------- */

/*
  Procedure..: strcat
  Description..: Concatenate the contents of one string onto another.
  Params..: s1-destination, s2-source
*/
char *strcat(char *s1, const char *s2) {
    char *rc = s1;
    if (*s1) while (*++s1);
    while ((*s1++ = *s2++));
    return rc;
}

/*
  Procedure..: isspace
  Description..: Determine if a character is whitespace.
  Params..: c-character to check
*/
int isspace(const char *c) {
    if (*c == ' ' ||
        *c == '\n' ||
        *c == '\r' ||
        *c == '\f' ||
        *c == '\t' ||
        *c == '\v') {
        return 1;
    }
    return 0;
}

/*
  Procedure..: memset
  Description..: Set a region of memory.
  Params..: s-destination, c-byte to write, n-count
*/
void *memset(void *s, int c, size_t n) {
    unsigned char *p = (unsigned char *) s;
    while (n--) {
        *p++ = (unsigned char) c;
    }
    return s;
}

/*
  Procedure..: strtok
  Description..: Split string into tokens
  Params..: s1-string, s2-delimiter
*/
char *strtok(char *s1, const char *s2) {
    static char *tok_tmp = NULL;
    const char *p = s2;

    //new string
    if (s1 != NULL) {
        tok_tmp = s1;
    }
        //old string cont'd
    else {
        if (tok_tmp == NULL) {
            return NULL;
        }
        s1 = tok_tmp;
    }

    //skip leading s2 characters
    while (*p && *s1) {
        if (*s1 == *p) {
            ++s1;
            p = s2;
            continue;
        }
        ++p;
    }

    //no more to parse
    if (!*s1) {
        return (tok_tmp = NULL);
    }

    //skip non-s2 characters
    tok_tmp = s1;
    while (*tok_tmp) {
        p = s2;
        while (*p) {
            if (*tok_tmp == *p++) {
                *tok_tmp++ = '\0';
                return s1;
            }
        }
        ++tok_tmp;
    }

    //end of string
    tok_tmp = NULL;
    return s1;
}



/* And finally....
   For the brave ones! (Note: you'll need to add a prototype to string.h)
   sprintf must work properly for the following types to receive extra credit:
     1) characters
     2) strings
     3) signed integers
     4) hexadecimal numbers may be useful
     ...
     \infty) Or feel free to completely implement sprintf
             (Read the man Page: $ man sprintf)
   int sprintf(char *str, const char *format, ...); 
*/

