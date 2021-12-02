#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include <signal.h>

#define BUFF_SIZE 200

int parseFile(int fd, int file_size, off_t *strings_arr){

 int count = 0;
 char buff[BUFF_SIZE];
 int read_str_size = 0;
 unsigned int position = 0;
 int indicator = 0;
 
 strings_arr[0] = 0;
 while(1){
   lseek(fd, position, SEEK_SET);
   read_str_size = read(fd, buff, BUFF_SIZE);
   if(read_str_size == 0){
     break;
   }
   if(read_str_size == -1){
     if(errno == EINTR || errno == EAGAIN) //maybe file was used by another process
        continue;
     else {
       perror("Error while reading.\n");
       return -1;
     }
   }
  
   unsigned int i = 0;
   while(i < read_str_size){
     if(buff[i] == '\n'){
       if(indicator == 0){
         strings_arr[0] = i;
         indicator = 1;
       }else{
          count ++;
          strings_arr[count] = position + i;
       } 
     }
     i++;
     if(i == read_str_read){
       position = position + string_arr[count] + 1;
     }
   }
   if(position > file_size)
     break;
 }
 return count + 1;
}


void printLine(int fd, off_t *enteries, int str_number){

 off_t bytes_amount = enteries[str_number - 1] - enteries[str_number - 2] - 1;
 off_t starter_byte = enteries[str_number - 2] + 1;
 
 if(str_number == 1){
  bytes_amount = enteries[0];
  starter_byte = 0;
 }
 
 lseek(fd, starter_byte, SEEK_SET);
 char buff[bytes_amount];
 
 while(bytes_amount > 0){
  int bytes_read = read(fd, buff, bytes_amount);
  bytes_amount = bytes_amount - bytes_read;
  if(bytes_amount >= 0)
   printf("%s", buff);
  else{
   int i = 0;
   while(i< bytes_read){
    printf("%c", buff[i]);
    i ++;
   }  
  } 
 }
 printf("\n");
}

int closefile(int fd){

  if(close(fd) == -1){
      if(errno == EINTR){
          printf("The close() call was interrupted by a signal/n");
          return -1;
      }else{
          if(errno == EIO){
              printf("An I/O error occured/n");
              return -1;
          }else{
              printf("Close() ended with error");
              return -1;
          }
      }
  }
  return 0;
}


int user_interaction(int fd, off_t *enteries, int strings_amount){

 int str_number = 1;
 
 max_fd = 0;
 fd_set rfds;
 struct timeval timeout;
 FD_ZERO (&rfds);
 FD_SET(0, &rfds);
 timeout.tv_sec = 5; //seconds
 timeout.tv_usec = 0; //microseconds
 int fd_amount = select(max_fd + 1, &rfds, NULL, NULL, &timeout);//reurns the number of fds contained in the three returned descriptor sets
 if(fd_amount == -1){
  printf("Error in select( ).\n");
  return -1;
 }
 if(FD_ISSET(fd, &rfds)!= 0){
  while(str_number){
  
   printf("Please, enter non-negative string number: \n");
   printf(" Enter 0 to exit.\n");
   scanf("%d", &str_number);
  
   if(str_number == 0){
    if(close(fd) == -1){
     perror("File close error.\n");
     return -1;
    }
    break;
   }
  
   if(str_number < 0){
    printf("You inputed a negative number. Try agan \n");
    continue; 
   }
  
   if(str_number > strings_amount){
    printf("You inputed a number over the file bound. \n Please, enter from 1 to %d \n", strings_amount);
    alarm(5);
    continue;
   }
   printLine(fd, *enteries, str_number);
   alarm(5); //reset alarm for 5 sec.
  }
 }else printf("fd absent in fd_set"\n);
 if(fd_amount == 0){
  printf ("Time is over! The whole file is printed:>>   \n");
  int k = 1;
  while(k<= strings_amount){
   printLine(fd, *enteries, k);
   k++;  
  }
 }
 
 return 0;
}

int main (int argc, char *argv[]){

 if(argc != 2){
  printf("not enough arguments.\n");
  return -1; 
 }
 
 int fd = open(argv[1], O_RDONLY);
 
 if(fd == -1){
  printf("File cannot be opened./n");
  return -1;
 }
 off_t enteries[BUFF_SIZE];
 int file_size = lseek(fd, 0L, SEEK_END);
 if(file_size == -1){
  printf("Something went wrong while seeking");
  return -1;
 }
 lseek(fd, 0L, SEEK_SET);
 int stings_amount = parseFile(fd, file_size, enteries);
 if(strings_amount == -1){
  printf("Errors while file parsing.\n");
  close(fd);
  return -1;
 } 
  int user_status = 1;
  while(user_status == 1)
   user_status = user_interaction(fd, enteries, strings_amount);
 
 return 0;
}
