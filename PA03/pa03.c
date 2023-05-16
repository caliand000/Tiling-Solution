#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include<string.h>

#include "answer03.h"


int main(int argc, char **argv)
{

     if(argc != 6){
      fprintf(stderr, "expecting 5 arguments\n");
      return EXIT_FAILURE;
   }

   char *str1 = "-v";
   char *str2 = "-g";
   int method = 0;

   if((strcmp(argv[1], str1) == 0)){
      method = 1;
   }
   else if((strcmp(argv[1], str2) == 0)){
      method = 2;
   }
   else{
      return EXIT_FAILURE;
   }

    // verify first argument of length of square floor

    errno = 0;
   char *pfloor;

   double floor_length = strtod(argv[2], &pfloor);
   if(errno != 0){
      return EXIT_FAILURE;
   }
   else if(argv[2] == pfloor){
      return EXIT_FAILURE;
   }

   //verify second argument of row coordinate of the 4-tile

    errno = 0;
   char *prow_coord;

   double row_coord = strtod(argv[3], &prow_coord);
   if(errno != 0){
      return EXIT_FAILURE;
   }
   else if(argv[3] == prow_coord){
      return EXIT_FAILURE;
   }

   //verify second argument of column coordinate of the 4-tile

    errno = 0;
   char *pcol_coord;

   double col_coord = strtod(argv[4], &pcol_coord);
   if(errno != 0){
      return EXIT_FAILURE;
   }
   else if(argv[4] == pcol_coord){
      return EXIT_FAILURE;
   }

   int category;
   int n = floor_length;

    //initialize values into structure to be passed into functions


   tiling_solution *pts = malloc(sizeof(*pts));
   if(pts == NULL){
      return EXIT_FAILURE;
   }


   pts->floor_length = floor_length;
   pts->row = row_coord;
   pts->column = col_coord;

   if(method == 1){
      category = determine_tiling_solution_category(argv[5], pts);
      fprintf(stdout,"%d\n",category);

   }
   else if(method == 2){
      int save = 0;

      if(pts->row >= pts->floor_length || pts->column >= pts->floor_length){
         free(pts);
        return EXIT_FAILURE;
      }

      int length_store = pts->floor_length;
      while(pts->floor_length > 1){
            if(pts->floor_length % 2 != 0){
                free(pts);
                return EXIT_FAILURE;
            }
            pts->floor_length = pts->floor_length / 2;
        }
        pts->floor_length = length_store;

      generate_tiling_solution(pts);
      save = save_tiling_solution(argv[5], pts);
      if(save != 1){
         free_2d_array(pts->floor_content, n);
         free(pts);
         return EXIT_FAILURE;
      }
      free_2d_array(pts->floor_content, n);
   }

   free(pts);
   return EXIT_SUCCESS;
}