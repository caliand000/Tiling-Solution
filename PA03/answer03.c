#include "answer03.h"
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>


char **allocate_2d_array(int n)
{
    char **floor_content;

    floor_content = malloc(sizeof(*floor_content) * n);
    if(floor_content == NULL){
        free(floor_content);
        return NULL;
    }
    
    for(int i = 0; i < n; i++)
    {
        floor_content[i] = malloc(sizeof(*(floor_content[i])) * (n));
        if(floor_content[i] == NULL)
        {
          for(int j = 0; j < i; j++){
            free(floor_content[j]);
          } 
          free(floor_content); 
          return NULL;
        }
    }
    
   

    return floor_content;
}


void free_2d_array(char **array, int n)
{
    
    for(int j = 0; j < n; j++){
            free(array[j]);
        } 
    free(array); 
}

int determine_tiling_solution_category(char *filename, tiling_solution *ts)
{
    FILE *fp = fopen(filename, "r");
   if(fp == NULL){
      fprintf(stderr,"input could not be opened\n");
      //fclose(fp);
      return EXIT_FAILURE;
   }

    // check if file is potential solution like in pe05
    int length;
    errno = 0;
    fseek(fp, 0, SEEK_END);
    int fpos_end = ftell(fp);

    fseek(fp, 0, SEEK_SET);
    int c = fgetc(fp);
    if(c == 32 || c == 48 || c == 10){
        fclose(fp);
        return INVALID_FLOOR_LENGTH;
    }
    fseek(fp, 0, SEEK_SET);

    if(fscanf(fp," %d", &length) == 0){
        fclose(fp);
        return INVALID_FLOOR_LENGTH;
    }
    int length_store = length;

    int d = fgetc(fp);
    if(d != 10){
        fclose(fp);
        return INVALID_FLOOR_LENGTH;
    }

    if(errno != 0){
        fclose(fp);
        return INVALID_FLOOR_LENGTH;
    }
    else{
        while(length > 1){
            if(length % 2 != 0){
                fclose(fp);
                return INVALID_FLOOR_LENGTH;
            }
            length = length / 2;
        }
    }
    length = length_store;
   //the position is now at the first coordinate where it should be
    int coordinate = 1;

   d = fgetc(fp);
    if(d == 32 || d == 48 || d == 10){
        fclose(fp);
        return INVALID_COORDINATES;
    }
    fseek(fp,-1,SEEK_CUR);

    if(fscanf(fp," %d", &coordinate) == 0){
        fclose(fp);
        return INVALID_COORDINATES;
    }

    int comma = fgetc(fp);
    if(comma != 44){
        fclose(fp);
        return INVALID_COORDINATES;
    }

    int coord_2 = fgetc(fp);
    if(coord_2 == 32 || coord_2 == 48 || d == 10){
        fclose(fp);
        return INVALID_COORDINATES;
    }
    fseek(fp,-1,SEEK_CUR);
    if(fscanf(fp," %d", &coord_2) == 0){
        fclose(fp);
        return INVALID_COORDINATES;
    }
 
    d = fgetc(fp);
    if(d != 10){
        fclose(fp);
        return INVALID_COORDINATES;
    }

    if((coordinate > length) || (coord_2 > length)){
        fclose(fp);
        return INVALID_COORDINATES;
            }


    // the position is now at the first value of the floor values
 
    int fpos = ftell(fp);

    int file_size = (length * length) + length + fpos;
    if(fpos_end != file_size){
        fclose(fp);
        return INVALID_FILE_SIZE;
    }
    else{
         file_size = 1;
    }

    //should still be at first value of the floor values
    //int floor_content = 1;

    d = fgetc(fp);
    if(d == 32 || d == 48 || d == 10){
        fclose(fp);
         return INVALID_FLOOR_CONTENT;
    }
    fseek(fp,-1,SEEK_CUR);

    int floor_values  = ftell(fp);
     c = fgetc(fp);
     fpos = ftell(fp);

    int four_position;
    fseek(fp, coordinate * (length + 1) + coord_2,SEEK_CUR);
    four_position = ftell(fp);

    int file_position;
    fseek(fp,floor_values,SEEK_SET);

    for(fpos = floor_values; fpos < fpos_end; fpos += (length + 1)){
        while(((c=fgetc(fp)) != 10)){
            if(c == EOF){
                break;
            }
            file_position = ftell(fp);
            if(file_position == four_position){
   
            }
            else if(((c == 48) ||(c == 49) ||(c == 50) ||(c == 51)|| (c == 10)) == 0){
                fclose(fp);
                return INVALID_FLOOR_CONTENT;  
            }
        }
    }
    // check if arguments match up with values within the file

    if(length != ts->floor_length){
        fclose(fp);
        return INVALID_FLOOR_LENGTH;
    }

    if(coordinate != ts->row || coord_2 != ts->column){
        fclose(fp);
        return INVALID_COORDINATES;
    }

    // convert file into an array for reading
    ts->floor_content = allocate_2d_array(length);

   int i = 0;
   int j = 0;
   fseek(fp,floor_values,SEEK_SET);
   fpos = ftell(fp);
   //printf("Thi is the position:%d\n",fpos);

    while((c = fgetc(fp)) != EOF){
        if(c != '\n'){
            ts->floor_content[j][i] = c; 
            i++;
        }
        else if(c == '\n'){
            j++;
            i = 0;
        }
    }

    //check if array is an actual solution by verifying the tiles within it

    int row;
    int col;
    
    int valid_counter = 0;
    int tile_marker = 0;
    //printf("before testing the array\n");
    for(row = 0;row < length;row++){
        for(col = 0; col < length;col++){
            if(ts->floor_content[row][col] == '0'){
                //printf("row and col for 0:%d,%d\n",row,col);
                if((col > 0 && row < length - 1) && (ts->floor_content[row + 1][col - 1] == '0' && ts->floor_content[row + 1][col] == '0')){
                    valid_counter++;
                }
                if((row > 0 && col < length - 1) && (ts->floor_content[row - 1][col + 1] == '0' && ts->floor_content[row][col + 1] == '0')){
                    col++;
                    valid_counter++;
                }
                if((row > 0 && col > 0) && (ts->floor_content[row][col - 1] == '0' && ts->floor_content[row - 1][col] == '0')){
                    valid_counter++;
                }
                if(valid_counter == 0){
                    fclose(fp);
                    free_2d_array(ts->floor_content, length);
                    //printf("reached the 0 tile check\n");
                    return INVALID_FLOOR_CONTENT;
                }
                if(tile_marker == 1){
                    col++;
                }
                valid_counter = 0;
                tile_marker = 0;
            }

            else if(ts->floor_content[row][col] == '1'){
                //printf("row and col for 1:%d,%d\n",row,col);
                if ((row < length - 1 && col < length - 1) && (ts->floor_content[row + 1][col] == '1' && ts->floor_content[row + 1][col + 1] == '1')){
                    valid_counter++;
                }
                if((row > 0 && col < length - 1) && (ts->floor_content[row - 1][col] == '1' && ts->floor_content[row][col + 1] == '1')){
                    col++;
                    valid_counter++;
                }
                if((row > 0 && col > 0) && (ts->floor_content[row - 1][col - 1] == '1' && ts->floor_content[row][col - 1] == '1')){
                    valid_counter++;
                }
                if(valid_counter == 0){
                    fclose(fp);
                    free_2d_array(ts->floor_content, length);
                    //printf("reached the 1 tile check\n");
                    return INVALID_FLOOR_CONTENT;
                }
                if(tile_marker == 1){
                    col++;
                }
                valid_counter = 0;
                tile_marker = 0;
            }
            else if(ts->floor_content[row][col] == '2'){
                //printf("row and col for 2:%d,%d\n",row,col);
                if((row < length - 1 && col < length - 1) && (ts->floor_content[row][col + 1] == '2' && ts->floor_content[row + 1][col + 1] == '2')){
                    tile_marker = 1;
                    valid_counter++;
                }
                if((col > 0 && row < length - 1) && (ts->floor_content[row][col - 1] == '2' && ts->floor_content[row + 1][col] == '2')){
                    valid_counter++;
                }
                if((row > 0 && col > 0) && (ts->floor_content[row - 1][col - 1] == '2' && ts->floor_content[row - 1][col] == '2')){
                    valid_counter++;
                }
                if(valid_counter == 0){
                    fclose(fp);
                    free_2d_array(ts->floor_content, length);
                    //printf("reached the 2 tile check\n");
                    return INVALID_FLOOR_CONTENT;
                }
                if(tile_marker == 1){
                    col++;
                }
                valid_counter = 0;
                tile_marker = 0;
            }
            else if(ts->floor_content[row][col] == '3'){
                //printf("row and col for 3:%d,%d\n",row,col);
                if((row < length - 1 && col < length - 1) && ts->floor_content[row][col + 1] == '3' && ts->floor_content[row + 1][col] == '3'){
                    valid_counter++;
                    tile_marker = 1;
                }
                if((row < length - 1 && col > 0) && (ts->floor_content[row][col - 1] == '3' && ts->floor_content[row + 1][col - 1] == '3')){
                    valid_counter++;
                }
                if((row > 0 && col < length - 1) && (ts->floor_content[row - 1][col] == '3' && ts->floor_content[row - 1][col + 1] == '3')){
                    valid_counter++;
                }
                if(valid_counter == 0){
                    fclose(fp);
                    free_2d_array(ts->floor_content, length);
    
                    return INVALID_FLOOR_CONTENT;
                }
                if(tile_marker == 1){
                    col++;
                }
                
                valid_counter = 0;
                tile_marker = 0;
            }
        }
    }

    free_2d_array(ts->floor_content, length);
   fclose(fp);
    return VALID_SOLUTION;
}


int save_tiling_solution(char *filename, tiling_solution *ts)
{
    FILE *fpo = fopen(filename, "w");
   if(fpo == NULL){
      fprintf(stderr,"input could not be opened\n");
      fclose(fpo);
      return INVALID_FLOOR_CONTENT;
   }

    fprintf(fpo,"%d",ts->floor_length);
    fputc('\n',fpo);
    fprintf(fpo,"%d,%d",ts->row,ts->column);
    fputc('\n',fpo);
   for(int row = 0;row < ts->floor_length;row++){
        fwrite(ts->floor_content[row], ts->floor_length, sizeof(char), fpo);
        fputc('\n',fpo);
    
    }

    fclose(fpo);
    return 1;
}

void recursion_function(int origin_row, int origin_col, int floor_length, int tile_row, int tile_col, tiling_solution *ts){

     if(floor_length == 1){
        return;
    }

    floor_length = (floor_length / 2);

    if(tile_row < (origin_row + floor_length)){ // top of coordinate plane
        if(tile_col < (origin_col + floor_length)){ // left of coordinate plane
            //quadrant 2
            ts->floor_content[origin_row + floor_length][origin_col + floor_length - 1] = '0';
            ts->floor_content[origin_row + floor_length - 1][origin_col + floor_length] = '0';
            ts->floor_content[origin_row + floor_length][origin_col + floor_length] = '0';
            
                //first quad
                recursion_function(origin_row, origin_col + floor_length, floor_length,
                                         origin_row + floor_length - 1, origin_col + floor_length, ts);
                //second quad
                recursion_function(origin_row, origin_col, floor_length, tile_row, tile_col, ts);
                //third quad
                recursion_function(origin_row + floor_length, origin_col, floor_length,
                                         origin_row + floor_length, origin_col + floor_length - 1, ts);
                //fourth quad
                recursion_function(origin_row + floor_length, origin_col + floor_length, floor_length, 
                                        origin_row + floor_length, origin_col + floor_length, ts);
            return;
        }
        else{                                          // right of coordinate plane
            //quadrant 1
            ts->floor_content[origin_row + floor_length - 1][origin_col + floor_length - 1] = '1';
            ts->floor_content[origin_row + floor_length][origin_col + floor_length - 1] = '1';
            ts->floor_content[origin_row + floor_length][origin_col + floor_length] = '1';
                //first quad
                recursion_function(origin_row, origin_col + floor_length, floor_length,
                                         tile_row, tile_col, ts);
                //second quad
                recursion_function(origin_row, origin_col, floor_length, 
                                        origin_row + floor_length - 1, origin_col + floor_length - 1, ts);
                //third quad
                recursion_function(origin_row + floor_length, origin_col, floor_length,
                                         origin_row + floor_length, origin_col + floor_length - 1, ts);
                //fourth quad
                recursion_function(origin_row + floor_length, origin_col + floor_length, floor_length, 
                                        origin_row + floor_length, origin_col + floor_length, ts);
            return;
        }
    }
    else{                                              //bottom of coordinate plane
        if(tile_col < (origin_col + floor_length)){ // left of coordinate plane
           // quadrant = 3;
           ts->floor_content[origin_row + floor_length - 1][origin_col + floor_length - 1] = '2';
           ts->floor_content[origin_row + floor_length - 1][origin_col + floor_length] = '2';
           ts->floor_content[origin_row + floor_length][origin_col + floor_length] = '2';
                //first quad
                recursion_function(origin_row, origin_col + floor_length, floor_length,
                                         origin_row + floor_length - 1, origin_col + floor_length, ts);
                //second quad
                recursion_function(origin_row, origin_col, floor_length, 
                                        origin_row + floor_length - 1, origin_col + floor_length - 1, ts);
                //third quad
                recursion_function(origin_row + floor_length, origin_col, floor_length,
                                         tile_row, tile_col, ts);
                //fourth quad
                recursion_function(origin_row + floor_length, origin_col + floor_length, floor_length, 
                                        origin_row + floor_length, origin_col + floor_length, ts);
            return;
        }
        else{                                           // right of coordinate plane
            //quadrant = 4;
            ts->floor_content[origin_row + floor_length - 1][origin_col + floor_length - 1] = '3';
            ts->floor_content[origin_row + floor_length - 1][origin_col + floor_length] = '3';
            ts->floor_content[origin_row + floor_length][origin_col + floor_length - 1] = '3';
                //first quad
                recursion_function(origin_row, origin_col + floor_length, floor_length,
                                         origin_row + floor_length - 1, origin_col + floor_length, ts);
                //second quad
                recursion_function(origin_row, origin_col, floor_length, 
                                        origin_row + floor_length - 1, origin_col + floor_length - 1, ts);
                //third quad
                recursion_function(origin_row + floor_length, origin_col, floor_length,
                                         origin_row + floor_length, origin_col + floor_length - 1, ts);
                //fourth quad
                recursion_function(origin_row + floor_length, origin_col + floor_length, floor_length, 
                                        tile_row, tile_col, ts);
            return;
        }
    }    
}





void generate_tiling_solution(tiling_solution *ts){


    ts->floor_content = allocate_2d_array(ts->floor_length);

    for(int i = 0; i < ts->floor_length; i++){
        for(int j = 0; j < ts->floor_length; j++){
            ts->floor_content[i][j] = 'X';
        }
    }

    ts->floor_content[ts->row][ts->column] = '4';

    if(ts->floor_length == 1){
        return;
    }
    recursion_function( 0, 0, ts->floor_length, ts->row, ts->column, ts);                                      
}