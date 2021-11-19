#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>

#define MAX_BUFF 200
#define MAX_LINES 100


int parseFile(int fd, int file_size, off_t *strings_arr){

 int count = 0;
 char buff[MAX_LINES];
 int read_str_size = 0;
 int position = 0;
 
 strings_arr[0] = 0;
 while(1){
  lseek(fd, position, SEEK_SET);
  read_str_size = read(fd, buff, MAX_LINES);
  if(read_str_size == 0){
   printf("FILE is empty./n");
   close(fd);
   return -1;
  }
  if(read_str_size == -1){
   if(errno == EINTR || errno == EAGAIN) //maybe file was used by another process
    continue;
   else {
    perror("Error while reading.\n");
    close(fd);
    return -1;
   }
  }
  
  int indicator = 0; 
  int i = 0;
  while(i < read_str_size){
   if(buff[i] == '\n'){
    if(indicator == 0){
     strings_arr[0] = i;
     indicator = 1;
    }
    count ++;
    strings_arr[count] = position + i;
   }
   i++;  
  }
  if(position > file_size)
   break;
  position += read_str_size; 
 }
 
 return count;

}


void printLine(int fd, off_t *enteries, int str_number){

 off_t bytes_amount = enteries[str_number] - enteries[str_number - 1] - 1;
 off_t starter_byte = enteries[str_number - 1] + 1;
 
 if(str_number == 1){
  bytes_amount = enteries[1];
  starter_byte = 0;
 }
 
 lseek(fd, starter_byte, SEEK_SET);
 char buff[MAX_LINES];
 
 while(bytes_amount > 0){
  int bytes_read = read(fd, buff, MAX_LINES);
  bytes_amount -= bytes_read;
  if(bytes_amount >= 0)
   printf("%s", buff);
  else{
   int i = 0;
   while(i< bytes_amount + bytes_read){
    printf("%c", buff[i]);
    i ++;
   }  
  } 
 }
 printf("\n");
}


int user_interaction(int fd, off_t *enteries, int strings_amount){

 int str_number = 1;
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
   continue;
  }
  printLine(fd, enteries, str_number);
 }
 
 return 0;
}

int main (){

 int fd = open("my_file.txt", O_RDONLY);
 
 if(fd == -1){
  printf("File cannot be opened./n");
  return -1;
 }
 off_t enteries[MAX_LINES];
 int file_size = lseek(fd, 0L, SEEK_END);
 if(file_size == -1){
  printf("Something went wrong while seeking");
  return -1;
 }
 lseek(fd, 0l, SEEK_SET);
 int strings_amount = parseFile(fd, file_size, enteries);
 if(strings_amount == -1){
  printf("Errors while file parsing.\n");
  return -1;
 } 
  int user_status = 1;
  while(user_status == 1)
   user_status = user_interaction(fd, enteries, strings_amount);
 
 return 0;
}

