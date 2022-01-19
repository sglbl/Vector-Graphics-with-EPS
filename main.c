#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "library.h"

void free_fig(Figure* rootp){
	Figure *temp2, *temp = rootp;
	for(;temp->nextptr != NULL;temp = temp->nextptr){
		temp2 = temp;
		if(temp2 != NULL)	free(temp2);
	}
}

int main(){
	Figure *fig;
	int counter = 1;
	//-----------------------------PART2-------------------------------//
	printf("Part2's Program is running\n");
	fig=NULL;
	read_file(&fig);

	Figure *info = fig;
	for(;  info != NULL; info=info->nextptr, counter++){
		if(counter==1)	printf("Please wait for One Second!\n\n");
		printf("-------INFO %d-------\n", counter);
		export_eps(info, info->filename);
	
		printf("background_color:(%.1f,%.1f,%.1f)\n",info->background.red, info->background.green, info->background.blue);
		printf("foreground_color:(%.1f,%.1f,%.1f)\n",info->foreground.red, info->foreground.green, info->foreground.blue);
		printf("Grid: %d (0 for False, 1 for True)\n",info->show_grid );
		printf("tile_size: %d\n",info->tile_size);
		printf("archetype: %d (4 for square, 6 for hexagon)\n",info->archetype );
		printf("pattern_thickness:%d\n",info->pattern_thick);
		printf("angle_offset:(%.1f,%.1f)\n",info->angle_offset.c1, info->angle_offset.c2);
		printf("canvas_size:(%.1f,%.1f)\n",info->canvas_size.c1, info->canvas_size.c2);
		printf("Filename:  %s\n\n",info->filename );
	}	
	free_fig(fig);

	return 0;
}