#include <stdio.h>
#include <string.h>

int main()
{
    char * strA = "Blahblahblah", * strB = "123", * strC[50];
    int x = 4;
    
    //char *strncpy(char *dest, const char *src, size_t n)
    strncpy(strC, strA, 11);
    // strC[x] = '\0';
    // strcat(strC, strB);
    // strcat(strC, strA + x);
    printf("%s\n", strC);
}

// #include <stdio.h>
// #include <string.h>

// int main () {
//    char src[40];
//    char dest[12];
  
//    memset(dest, '\0', sizeof(dest));
//    strcpy(src, "This is tutorialspoint.com");
//    strncpy(dest, src, 10);

//    printf("Final copied string : %s\n", dest);
   
//    return(0);
// }