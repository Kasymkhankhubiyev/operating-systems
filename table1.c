#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> //for lseek

#define MAX_BUF 200
#define MAX_LINES 50

typedef sctruct {

 off_t offset;
 unsingend int length;

}Line_entry;

int build_table(int fd, Line_entry *table, unsigned int max_size){
int current_line = 1;
int current_position = 0;
table[1].offset = 0L; //интерпретируем как long int
while (true){
 int count = read(fd, buffer, MAX_BUF);//получим кол-во считанный байтов.
 if (count == 0){
  printf(stderr, "END of FILE.\n");
  return -1;
 }
 if (count == -1){
  if(errno == EINTR || errno == EAGAIN) // EINTR - прерван до начала считывания; EAGAIN - файл временно не доступен по каким-то причинам
   continue; //повторим попытку
  else{
   printf(stderr, "The case the File cannot be read caused table cannot be build.");
   return -1;
  }
 }
 for(unsigned int i = 0; i< count; i++){
  current position ++;
  if(buffer[i] == '/n'){
   table[current_line].length = current_position - 1;
   if(current_line == max_size){
    printf(stderr, "Too long string number %d");
    return current_line -1;
   }
   table[current_line + 1].offset = table[current_line].offset + current_position;//задаем сдвиг
   current_line ++;
   current_position = 0
  }
 }
}
table[current_line].length = current_position.
return current_line;
}

void print_line (int fd, Line_entry *table, int line_number){
 

}


main(){
 FILE *file;
 int fd = open("list_table", O_RDONLY); //открываем файл с доступом read_only и получаем file_descriptor
 if(fd == -1){
  printf(stderr, "File is not opened.\n");
  return EIO;
 }
 Line_entry *table = malloc (sizeof(Line_entry)*(MAX_LINES+1));
 if(table == NULL){ //проверка успешного выделения памяти
  printf(stderr, "Cannot allocate memory.\n");
  return -1;
 }
 printf("Hellow! Let's build search_table!");
 int table_status = build_table(fd, table, MAX_LINES);
 if(table_status == -1){
  printf(stderr, "Table is not build.\n");
  return -1;
 }
 //print table
 
 



return 0;
}
