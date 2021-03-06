#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include <signal.h>
#include <sys/mman.h>

#define BUFF_SIZE 200

int parseFile(int fd, char* map, int file_size, off_t *strings_arr){

  int count = 0; 
  strings_arr[0] = 0;
  unsigned int i = 0;
  while(i < file_size){
    if(map[i] == '\n'){
      strings_arr[0] = i;
      count ++;
    }
    i++;
  }
  return count;
}


void printLine(off_t *enteries, int str_number, char *map){

  off_t bytes_amount = enteries[str_number - 1] - enteries[str_number - 2] - 1;
  off_t starter_byte = enteries[str_number - 2] + 1;
 
  if(str_number == 1){
    bytes_amount = enteries[0];
    starter_byte = 0;
  }
 
  off_t i = 0;
  while(i < bytes_amount){
    printf("%c", map[starter_byte + i]);
    i ++;
  }
 printf("\n");
}

int closefile(int fd){

  if(close(fd) == -1){
      if(errno == EINTR){
          printf("The close() call was interrupted by a signal\n");
          return -1;
      }else{
          if(errno == EIO){
              printf("An I/O error occured\n");
              return -1;
          }else{
              printf("Close() ended with error\n");
              return -1;
          }
      }
  }
  return 0;
}


int user_interaction(char *map, int fd, off_t *enteries, int strings_amount){

 int str_number = 1;
 
 fd_set rfds;
 struct timeval timeout;
 FD_ZERO (&rfds);
 FD_SET(0, &rfds);
 timeout.tv_sec = 5; //seconds
 timeout.tv_usec = 0; //microseconds
 
 printf("Please, enter non-negative string number from 1 to %d: \n", strings_amount);
 printf("You have 5 sec.      Enter 0 to exit.\n");
 
 int retval = select(1, &rfds, NULL, NULL, &timeout);//reurns the number of fds contained in the three returned descriptor sets
 if(retval == -1){
  printf("Error in select( ).\n");
  return -1;
 }
 if(FD_ISSET(0, &rfds)){
  while(str_number){
    scanf("%d", &str_number);
  
    if(str_number == 0){
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
    printLine(enteries, str_number, map);
   }
 }else {
   printf ("Time is over! The whole file is printed:>>   \n");
   int k = 1;
   while(k<= strings_amount){
     printLine(enteries, k, map);
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
    printf("File cannot be opened.\n");
    return -1;
  }
  off_t enteries[BUFF_SIZE];
 
  struct stat file_info;
  if(fstat(fd, file_info) == -1){
    if(errno == EACCES){//???????????????? ????????????
      printf("Acces to file is denied./n");
      return -1;
    }else{
      printf("error while getting the size of file.\n");
      return -1;
    }
  }
  if(file_info.st_size == 0){
    printf("File is empty.\n");
    return -1;
  }
 
  char *map = mmap(0, file_info.st_size, PROT_READ, MAP_SHARED, fd, 0L);
  if(map == MAP_FAILED){
    printf ("Error while mapping the file\n");
    return closefile(fd);
  }
  
  int stings_amount = parseFile(fd, map, file_info.st_size, enteries);
  if(strings_amount == -1){
    printf("Errors while file parsing.\n");
    return closefile(fd);
  } 
  int user_status = 1;
  while(user_status == 1)
    user_status = user_interaction(char* map, enteries, strings_amount);
 if(nummap(map, file_info.st_size) == -1){
   printf("Error in un-mupping file/n");
   return -1;
 }
 return closefile(fd);
}
