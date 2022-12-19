#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main()
{
    char* username = malloc(sizeof(char) * 50) ;          // username
    char* room_id = malloc(sizeof(char) * 50) ;            // room id

    printf("Welcome to chatting system!\n");
    printf("Please enter your name: ");
    fgets(username, 20, stdin);
    //scanf("%[^\n]s",username);
    //scanf("%m[^\n]%*c", username);
    //printf("%s\n", username);
    printf("Please enter room id: ");
    fgets(room_id, 20, stdin);
    //scanf("%m[^\n]%*c", room_id);
    printf("%s : %s\n",username, room_id);
    return 0;
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