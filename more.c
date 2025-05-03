#include <math.h>
#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <fcntl.h> 
#include <string.h>
#include<sys/stat.h>
#include<sys/types.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <ctype.h>
#include <termios.h>
#include <stdbool.h>
char filepath[100] = "a.txt";
char non_echo_non_canon() // sets mode in non-echo non-canonical mode
{
    char buf = 0;
    struct termios old = {0};
    if (tcgetattr(0, &old) < 0)
         {   perror("tcsetattr()"); }

    old.c_lflag &= ~ICANON;

    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if (tcsetattr(0, TCSANOW, &old) < 0)
           { perror("tcsetattr ICANON"); }

    if (read(0, &buf, 1) < 0)
           { perror ("read()"); }

    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;

    if (tcsetattr(0, TCSADRAIN, &old) < 0)
           {  perror ("tcsetattr ~ICANON"); }

    return (buf);
}



void output_file_lim_6(FILE *fp) // display percentage
{

    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    //printf ("lines %d\n", w.ws_row);
  
    
    int total_lines = 0;
    //FILE *fp = fopen(filepath, "r"); // this creates empty file for both reading and writing 
    char ch;
    char command[500];
    char vim[200] = "vim ";

    int num_lines = 0;  
    char letter = 'a';
    int line_count = 0;
    int a;
    int b;
    double c;
    char br = ')';
 while ((ch = fgetc(fp)) != EOF)
        {
            if (ch == '\n')
             total_lines++; 
        }        
    fseek(fp, 0, SEEK_SET);

   if (fp == NULL)
    {
        printf("File is not available \n");
    }
    else
    {
      
        while ((ch = fgetc(fp)) != EOF)
        {
           if (num_lines < w.ws_row)
           {
            printf("%c", ch);
            if (ch == '\n')
            { 
             num_lines++; line_count++;
                
            }
            
           }   
           
           else if (num_lines >= w.ws_row)
           {  
               
                a = line_count; b = total_lines;
                double c = a/1.;
                c = c * 100;
                c = trunc(c/b);
              
                printf("\033[7m --more-- (%.lf%%) \033[m\n", c);
               
               
                
              //printf("\033[7m --more-- (%%) \033[m\n"); 
              //printf("\033[7m --more-- (%%)");
              letter = non_echo_non_canon();
              
              if (letter == 32) // space
              {printf("\033[1A \033[1G \033[2K"); 
              num_lines = 0; }
              
              else if (letter == 10) // enter
              {//printf("\033[1A \033[2K \033[1G");
              printf("\033[1A \033[1G \033[2K");
              num_lines--;}
              
              else if (letter == 113) // q
              { 
               printf("\033[1A \033[1G \033[2K");
                return; 
              }
              else if (letter == 118) // v
              { 
               printf("\033[1A \033[1G \033[2K");
               
               strcat(vim, filepath);
               strcpy(command, vim );
               system(command); 
              }
           }
        }
    }

    fclose(fp);
} 

void output_file_lim_7() // kyb denotes keyboard 
{   
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    int stdin_copy = dup(STDIN_FILENO);
    char ch;
    char file[20] = "output.txt";
    FILE *fp=fopen(file,"w+");
    double c = 0.0;
    FILE *fid = stdin;
    
    char line[1024];

    if(fp==NULL)   //exit program if file doesn’t open
    {
        printf("File could not open!\n");
        return ;
    }

   while(fgets(line, sizeof line, stdin) != NULL)
    {
        fputs(line, fp);
    }
  
   while(fgets(line, sizeof line, fp) != NULL)
    {
        puts(line);
    }

    int total_lines = 0;
    //FILE *fp = fopen(filepath, "r"); // this creates empty file for both reading and writing 

    char command[500];
    char vim[200] = "vim ";

    int num_lines = 0;  
    char letter = 'a';
    int line_count = 0;
    int a;
    int b;
   
    char br = ')';


    //fseek(fp, 0, SEEK_SET);
    fclose(fp);
    fp = fopen(file,"r");

    while ((ch = fgetc(fp)) != EOF)
            {
                if (ch == '\n')
                total_lines++; 
            }        
            
    //fseek(fp, 0, SEEK_SET);
    fclose(fp);
    fp = fopen(file,"r");

    strcpy(filepath, "output.txt");

   if (fp == NULL)
    {
        printf("File is not available \n");
    }
    else
    {
      
        while ((ch = fgetc(fp)) != EOF)
        {
           if (num_lines < w.ws_row)
           {
            if (ch == EOF) return;
            printf("%c", ch);
            if (ch == '\n')
            { 
             num_lines++; line_count++;
                
            }
           
           }   
           
           else if (num_lines >= w.ws_row && ch != EOF)
           {  
            if (ch == EOF) return;   
            a = line_count; b = total_lines;
            c = a/1.;
            c = c * 100;
            c = trunc(c/b);
            if (c == 100.0) return;

            printf("\033[7m --more-- (%.lf%%) \033[m\n", c);



            letter = non_echo_non_canon();

            if (letter == 32) // space
            {printf("\033[1A \033[1G \033[2K \033[1G"); 
            num_lines = 0; }

            else if (letter == 10) // enter
            {//printf("\033[1A \033[2K \033[1G");
            printf("\033[1A \033[1G \033[2K \033[1G");
            num_lines--;}

            else if (letter == 113) // q
            { 
            printf("\033[1A \033[1G \033[2K \033[1G");
            return; 
            }
            else if (letter == 118) // v
            { 

            strcat(vim, "output.txt");
            strcpy(command, vim );
            system(command);
            printf("\033[1A \033[1G \033[2K \033[1G"); 
              }
           }
        }
    }

    fclose(fp);
} 
int main(int argc , char *argv[])
{  


    if (argc == 1)
     { output_file_lim_7();}

    if (argc == 2)
     {
      FILE *fp = fopen(argv[1], "r");
      output_file_lim_6(fp);
     }

    if (argc >= 2)
     {
      FILE *fp;
      for (int counter = 1; counter < argc; counter++)
      {
        fp = fopen(argv[counter], "r");
        output_file_lim_6(fp);
        fclose(fp);
      }

     }
    return 0;
}
























