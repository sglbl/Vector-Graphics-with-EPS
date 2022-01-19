#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "library.h"
#define  LINESIZE 428
#define  RADIAN_TO_DEGREE 3.14159265/180 

Figure* start_figure(double width, double height, Figure *figure){
	// (0,0) points are the center of the canvas.
	figure-> max.x = (double)width/2; //Max and min boundary points in canvas.
	figure-> max.y = (double)height/2;
	figure-> min.x = (double)-width/2;
	figure-> min.y = (double)-height/2;
	return figure;
}

void set_color(Figure* fig, Color c){
	Color *cl=&c;
	fig->col.red   = cl->red;
	fig->col.green = cl->green;
	fig->col.blue  = cl->blue;
	cl = NULL;
	free(cl);
}

int name_finder(char *piece){
	int flag=-1;
	if     (strncmp("background_color:", piece,16) ==0)	flag=1;
	else if(strncmp("foreground_color:", piece,16) ==0)	flag=2;
	else if(strncmp("tile_size:",        piece, 9) ==0)	flag=3;
	else if(strncmp("angle_offset:",     piece,12) ==0)	flag=4;
	else if(strncmp("canvas_size:",      piece,11) ==0)	flag=5;
	else if(strncmp("show_grid:",        piece, 9) ==0)	flag=6;
	else if(strncmp("archetype:",        piece, 9) ==0)	flag=7;
	else if(strncmp("pattern_thickness:",piece,16) ==0)	flag=8;
	else if(strncmp("file_name:",        piece, 8) ==0) flag=9;
	else if(strncmp("//",				 piece, 2) ==0)	flag=10;
	else if(strncmp("grid_color:",       piece, 9) ==0)	flag=11;
	else if(strncmp("grid_thickness:",   piece,13) ==0)	flag=12;
	
	return flag;
}

int essential_words(char *string){
	char *string2 = strtok(string, "/");  // For prevent reading words in comment parts after //.
	int flag1, flag2, flag3;
	int size = strlen(string2);
	//printf("Size is %d\n", size);

	const char *f1 = "archetype";
	const char *f2 = "file_name";

	flag1=0, flag2=0, flag3=0;
	char *returned = strstr(string2, f1);
	if(returned != NULL)	flag1 = 1;

	returned = strstr(string2, f2);
	if(returned != NULL)	flag2 = 1;

	if(flag2 && flag1)	flag3=1;
	return flag3;
}

Figure* read_default(char* file_name){
	int size, flag, flag2;
	char *piece, buffer[LINESIZE];
	Figure *temp;
	Figure *info = (Figure*)malloc(sizeof(Figure)); 
	FILE* filep = fopen(file_name,"r");
	if(filep == NULL){
		printf("Couldn't open file.\n");
		return 0;
	}

	flag = -1, flag2=-1;

	while( fgets(buffer,LINESIZE, filep) ){
		if(flag2 == -1){
			flag2=1;
			temp=info;	//Save head info.
		}
		info->nextptr = NULL;
		piece = strtok(buffer, " \t");
		if(piece != NULL ){
			flag = name_finder(piece);
			if(flag==-1)	continue;
			if(piece[0]=='/' || piece[0]=='\n')	 {
				flag=10;
				continue;
			}
		}
		if(flag==-1)	continue;
		switch(flag){
			case 1:
				sscanf(piece,"background_color:(%lf,%lf,%lf)",&info->background.red, &info->background.green, &info->background.blue);
				break;
			case 2:
				sscanf(piece,"foreground_color:(%lf,%lf,%lf)",&info->foreground.red, &info->foreground.green, &info->foreground.blue);
				break;
			case 3:
				sscanf(piece,"tile_size:%d",&info->tile_size);
				break;
			case 4:
				sscanf(piece,"angle_offset:(%lf,%lf)",&info->angle_offset.c1, &info->angle_offset.c2);
				break;
			case 5:
				sscanf(piece,"canvas_size:(%lf,%lf)",&info->canvas_size.c1, &info->canvas_size.c2);
				info = start_figure(info->canvas_size.c1, info->canvas_size.c2, info);
				break;
			case 6:
				if(strncmp("show_grid:True", piece, 11)==0)	info->show_grid=1;
				if(strncmp("show_grid:False",piece, 11)==0)	info->show_grid=0;
				break;
			case 7:
				if(strncmp("archetype:Square", piece, 11)==0)	info->archetype=4;
				if(strncmp("archetype:Hexagon",piece, 11)==0)	info->archetype=6;
				break;
			case 8:
				sscanf(piece,"pattern_thickness:%d",&info->pattern_thick);
				break;
			case 9:
				size = strlen(piece);
				info->filename = (char*)malloc((size-10)*sizeof(char));
				for(int j=0; j< size-10; j++){
					info->filename[j] = piece[10+j];	
				}
				if     (info->filename[size-10] == '\n')	info->filename[size-10]='\0';
				else if(info->filename[size-11] == '\n')	info->filename[size-11]='\0';
				else    info->filename[size-10] = '\0';
				break;
			case 10:
				break;
			case 11:
				sscanf(piece,"grid_color:(%lf,%lf,%lf)",&info->grid.red, &info->grid.green, &info->grid.blue);
				break;
			case 12:
				sscanf(piece,"grid_thickness:%lf",&info->gridthick);
				break;
			default:
				printf("ERROR\n");
		}

		while(piece != NULL){
			piece = strtok(NULL, " \t");
			if(piece != NULL){
				if(strncmp("//", piece,2)==0)	break;
				if(piece[0]=='/' || piece[0]=='\n')	 { flag=10; break;}
				flag = name_finder(piece);
				//printf("5F: %d && 5PIECE IS: %s \n", flag,piece );
			}
			else	break;

			switch(flag){
				case 1:
					sscanf(piece,"background_color:(%lf,%lf,%lf)",&info->background.red, &info->background.green, &info->background.blue);
					break;
				case 2:
					sscanf(piece,"foreground_color:(%lf,%lf,%lf)",&info->foreground.red, &info->foreground.green, &info->foreground.blue);
					break;
				case 3:
					sscanf(piece,"tile_size:%d",&info->tile_size);
					break;
				case 4:
					sscanf(piece,"angle_offset:(%lf,%lf)",&info->angle_offset.c1, &info->angle_offset.c2);
					break;
				case 5:
					sscanf(piece,"canvas_size:(%lf,%lf)",&info->canvas_size.c1, &info->canvas_size.c2);
					info = start_figure(info->canvas_size.c1, info->canvas_size.c2, info);
					break;
				case 6:
					if(strncmp("show_grid:True", piece, 11)==0)	info->show_grid=1;
					if(strncmp("show_grid:False",piece, 11)==0)	info->show_grid=0;
					break;
				case 7:
					if(strncmp("archetype:Square", piece, 11)==0)	info->archetype=4;
					if(strncmp("archetype:Hexagon",piece, 11)==0)	info->archetype=6;
					break;
				case 8:
					sscanf(piece,"pattern_thickness:%d",&info->pattern_thick);
					break;
				case 9:
					size = strlen(piece);
					info->filename = (char*)malloc((size-10)*sizeof(char));
					for(int j=0; j< size-10; j++){
						info->filename[j] = piece[10+j];	
					}
					if     (info->filename[size-10] == '\n')	info->filename[size-10]='\0'; 
					else if(info->filename[size-11] == '\n')	info->filename[size-11]='\0'; 
					else    info->filename[size-10] = '\0';
					break;
				case 10:
					break;
				case 11:
					sscanf(piece,"grid_color:(%lf,%lf,%lf)",&info->grid.red, &info->grid.green, &info->grid.blue);
					break;
				case 12:
					sscanf(piece,"grid_thickness:%lf",&info->gridthick);
					break;
				default:
					printf("ERROR\n");
			}

		}
		if(flag==10)
			continue;
		
	}

	fclose(filep);
	info = temp;
	return info;
}

void read_file(Figure **head){
	FILE *filep = fopen("commands.txt","r");
	char *piece, buffer[LINESIZE];	//For reading line characters.
	Point2D *temp;
	Figure *info = (Figure*)malloc(sizeof(Figure));
	int size,flag;
	Figure *end;

	end=NULL;
	flag = -1;

	while( fgets(buffer,LINESIZE, filep) ){
		if( !essential_words(buffer) ){
			printf("Error! Some essential words are missing in line below:\n""%s""\n\n", buffer);
			continue;
		}
		info = read_default("defaults.txt");
		info->nextptr = NULL;
		piece = strtok(buffer, " \t");
		if(piece != NULL )
			flag = name_finder(piece);

		switch(flag){
			case 1:
				sscanf(piece,"background_color:(%lf,%lf,%lf)",&info->background.red, &info->background.green, &info->background.blue);
				break;
			case 2:
				sscanf(piece,"foreground_color:(%lf,%lf,%lf)",&info->foreground.red, &info->foreground.green, &info->foreground.blue);
				break;
			case 3:
				sscanf(piece,"tile_size:%d",&info->tile_size);
				break;
			case 4:
				sscanf(piece,"angle_offset:(%lf,%lf)",&info->angle_offset.c1, &info->angle_offset.c2);
				break;
			case 5:
				sscanf(piece,"canvas_size:(%lf,%lf)",&info->canvas_size.c1, &info->canvas_size.c2);
				info = start_figure(info->canvas_size.c1, info->canvas_size.c2, info);
				break;
			case 6:
				if(strncmp("show_grid:True", piece, 11)==0)	info->show_grid=1;
				if(strncmp("show_grid:False",piece, 11)==0)	info->show_grid=0;
				break;
			case 7:
				if(strncmp("archetype:Square", piece, 11)==0)	info->archetype=4;
				if(strncmp("archetype:Hexagon",piece, 11)==0)	info->archetype=6;
				break;
			case 8:
				sscanf(piece,"pattern_thickness:%d",&info->pattern_thick);
				break;
			case 9:
				size = strlen(piece);
				info->filename = (char*)malloc((size-10)*sizeof(char));
				for(int j=0; j< size-10; j++){
					info->filename[j] = piece[10+j];	
				}
				if     (info->filename[size-10] == '\n')	info->filename[size-10]='\0'; 
				else if(info->filename[size-11] == '\n')	info->filename[size-11]='\0'; 
				else    info->filename[size-10] = '\0';
				break;
			case 10:
				break;
			case 11:
				sscanf(piece,"grid_color:(%lf,%lf,%lf)",&info->grid.red, &info->grid.green, &info->grid.blue);
				break;
			case 12:
				sscanf(piece,"grid_thickness:%lf",&info->gridthick);
				break;
			default:
				printf("ERROR\n");
		}

		while(piece != NULL){
			piece = strtok(NULL, " \t");
			if(piece != NULL)
				flag = name_finder(piece);
			else	break;

			if(piece[0]=='/' || piece[0]=='\n')	 {
				flag=10;
				break;
			}

			switch(flag){
				case 1:
					sscanf(piece,"background_color:(%lf,%lf,%lf)",&info->background.red, &info->background.green, &info->background.blue);
					break;
				case 2:
					sscanf(piece,"foreground_color:(%lf,%lf,%lf)",&info->foreground.red, &info->foreground.green, &info->foreground.blue);
					break;
				case 3:
					sscanf(piece,"tile_size:%d",&info->tile_size);
					break;
				case 4:
					sscanf(piece,"angle_offset:(%lf,%lf)",&info->angle_offset.c1, &info->angle_offset.c2);
					//printf("angle_offset:(%lf,%lf)",info->angle_offset.c1, info->angle_offset.c2);
					break;
				case 5:
					sscanf(piece,"canvas_size:(%lf,%lf)",&info->canvas_size.c1, &info->canvas_size.c2);
					info = start_figure(info->canvas_size.c1, info->canvas_size.c2, info);
					break;
				case 6:
					if(strncmp("show_grid:True", piece, 11)==0)	info->show_grid=1;
					if(strncmp("show_grid:False",piece, 11)==0)	info->show_grid=0;
					break;
				case 7:
					if(strncmp("archetype:Square", piece, 11)==0)	info->archetype=4;
					if(strncmp("archetype:Hexagon",piece, 11)==0)	info->archetype=6;
					break;
				case 8:
					sscanf(piece,"pattern_thickness:%d",&info->pattern_thick);
					break;
				case 9:
					size = strlen(piece);
					info->filename = (char*)malloc((size-10)*sizeof(char));
					for(int j=0; j< size-10; j++){
						info->filename[j] = piece[10+j];	
					}
					if     (info->filename[size-10] == '\n')	info->filename[size-10]='\0'; 
					else if(info->filename[size-11] == '\n')	info->filename[size-11]='\0';
					else    info->filename[size-10] = '\0';
					break;
				case 10:
					break;
				case 11:
					sscanf(piece,"grid_color:(%lf,%lf,%lf)",&info->grid.red, &info->grid.green, &info->grid.blue);
					break;
				case 12:
					sscanf(piece,"grid_thickness:%lf",&info->gridthick);
					break;
				default:
					printf("ERROR\n");
			}

		}
		if(flag==10){		
			if(*head==NULL)	 *head = info;
			else			 end->nextptr = info;
			end = info;
			continue;
		}

		if( *head==NULL ){
			temp = (Point2D*)malloc(sizeof(Point2D));
			*head = info;

		}
		else
			end->nextptr = info;
		end = info;
	}
	end->nextptr = info;
	end->nextptr->nextptr = NULL;

	fclose(filep);
	
}

void draw_grid(Figure* info, char *file_name){
	FILE *filep = fopen(file_name,"a+");
	if(filep==NULL)	return;
	double thickness, numberoftile;
	double sum, size;

	if( info-> show_grid == 0)
		return;
	if( info-> show_grid == 1){
		thickness = info-> gridthick;
	
		fprintf(filep, "%.2f setlinewidth %% width\n",info->gridthick);
		fprintf(filep, "%lf %lf %lf setrgbcolor\n",info->grid.red, info->grid.green, info->grid.blue);

		sum=0;
		size = info->tile_size;
		numberoftile = info->canvas_size.c1/info->tile_size;
		for(int i=0; i<numberoftile; i++){
			fprintf(filep, "%.2f %.2f moveto\n", info->min.x + size, info->max.y + size );
			fprintf(filep, "%.2f %.2f lineto\n", info->max.x       , info->max.y + size );
			size -= info->tile_size;
		}
		size = -info->tile_size;
		for(int i=0; i<numberoftile; i++){
			fprintf(filep, "%.2f %.2f moveto\n", info->min.x - size, info->min.y );
			fprintf(filep, "%.2f %.2f lineto\n", info->min.x -size , info->max.y );
			size -= info->tile_size;
		}

		fprintf(filep, "closepath \ngsave \n");
		fprintf(filep, "stroke\n");
	}
	fclose(filep);
}

void draw_background(Figure *fig, char *file_name){
	Figure *temp = NULL;
	FILE* filep = fopen(file_name,"a+");
	if(filep == NULL){
		printf("Couldn't open file.\n");
		return;
	}

	fprintf(filep,"\n");
	fprintf(filep, "newpath\n");
    fprintf(filep,"%lf %lf moveto\n",fig->min.x, fig->min.y);
    fprintf(filep,"%lf %lf lineto \n%lf %lf lineto\n%lf %lf lineto\n",
    		fig->min.x,fig->max.y, fig->max.x, fig->max.y, fig->max.x, fig->min.y);
    fprintf(filep,"closepath\n");
    fprintf(filep,"gsave  %% sulo  \n");
    fprintf(filep,"%lf %lf %lf setrgbcolor\nfill\nstroke\n", (double)fig->background.red/255,
    			(double)fig->background.green/255, (double)fig->background.blue/255); 

    fclose(filep);
}

void coordinate_draw_top(Figure *fig, double x, double y, double multiplier, double angle){
	Figure *temp = NULL;
	FILE *filep = fopen(fig->filename, "a+");
	if(filep==NULL)	return;

	double halfborder = fig->tile_size/2;
	fprintf(filep,"%lf %lf moveto\n", x + halfborder - fig->angle_offset.c2, y);
    fprintf(filep,"%lf %lf rlineto\n", halfborder + fig->angle_offset.c2 - 
    	multiplier*cos(fig->angle_offset.c1*RADIAN_TO_DEGREE) , -sin(fig->angle_offset.c1*RADIAN_TO_DEGREE)*angle);

    fprintf(filep,"%lf %lf moveto\n", x + halfborder + fig->angle_offset.c2, y);
    fprintf(filep,"%lf %lf rlineto\n", -(halfborder + fig->angle_offset.c2 - 
    	multiplier*cos(fig->angle_offset.c1*RADIAN_TO_DEGREE)) , -sin(fig->angle_offset.c1*RADIAN_TO_DEGREE)*angle);

	fclose(filep);
}

void coordinate_draw_left(Figure *fig, double x, double y, double multiplier, double angle){
	Figure *temp = NULL;
	FILE *filep = fopen(fig->filename ,"a+");
	if(filep==NULL)	return;

	double halfborder = fig->tile_size/2;
	fprintf(filep,"%lf %lf moveto\n", x, y - halfborder - fig->angle_offset.c2);
    fprintf(filep,"%lf %lf rlineto\n", sin(fig->angle_offset.c1*RADIAN_TO_DEGREE)*angle, 
    	halfborder + fig->angle_offset.c2 - multiplier * cos(fig->angle_offset.c1*RADIAN_TO_DEGREE));

    fprintf(filep,"%lf %lf moveto\n", x, y - halfborder + fig->angle_offset.c2);
    fprintf(filep,"%lf %lf rlineto\n", sin(fig->angle_offset.c1*RADIAN_TO_DEGREE)*angle, 
    	-(halfborder + fig->angle_offset.c2 - multiplier * cos(fig->angle_offset.c1*RADIAN_TO_DEGREE)) );

	fclose(filep);
}

void coordinate_draw_bottom(Figure *fig, double x, double y, double multiplier, double angle){
	Figure *temp = NULL;
	FILE *filep = fopen(fig->filename, "a+");
	if(filep==NULL)	return;

	double halfborder = (double)fig->tile_size/2;
	fprintf(filep,"%lf %lf moveto\n", x + halfborder - fig->angle_offset.c2 , y - halfborder*2);
    fprintf(filep,"%lf %lf rlineto\n", halfborder + fig->angle_offset.c2 - multiplier*
    	cos(fig->angle_offset.c1*RADIAN_TO_DEGREE), angle* sin(fig->angle_offset.c1*RADIAN_TO_DEGREE));


    fprintf(filep,"%lf %lf moveto\n", x + halfborder + fig->angle_offset.c2, y- halfborder*2);
    fprintf(filep,"%lf %lf rlineto\n", -(halfborder + fig->angle_offset.c2 - 
    	multiplier*cos(fig->angle_offset.c1*RADIAN_TO_DEGREE)) , sin(fig->angle_offset.c1*RADIAN_TO_DEGREE)*angle);

	fclose(filep);
}

void coordinate_draw_right(Figure *fig, double x, double y, double multiplier, double angle){
	Figure *temp = NULL;
	FILE *filep = fopen(fig->filename, "a+");
	if(filep==NULL)	return;

	double halfborder = (double)fig->tile_size/2;
	fprintf(filep,"%lf %lf moveto\n", x + halfborder*2, y - halfborder - fig->angle_offset.c2);
    fprintf(filep,"%lf %lf rlineto\n", -sin(fig->angle_offset.c1*RADIAN_TO_DEGREE)*angle, 
    	halfborder + fig->angle_offset.c2 - multiplier*cos(fig->angle_offset.c1*RADIAN_TO_DEGREE));

    fprintf(filep,"%lf %lf moveto\n", x + halfborder*2, y - halfborder + fig->angle_offset.c2);
    fprintf(filep,"%lf %lf rlineto\n", -sin(fig->angle_offset.c1*RADIAN_TO_DEGREE)*angle, 
    	-(halfborder + fig->angle_offset.c2 - multiplier*cos(fig->angle_offset.c1*RADIAN_TO_DEGREE)) );

	fclose(filep);
}

double border_length(double angle){
	//printf("angle is %lf\n",  angle);
	switch( (int)angle ){
		case 1 ... 5:
			return 999;
			break;		 
		case 6 ... 11:
			return 10.7;
			break;			
		case 12 ... 16:
			return 6;
			break;
		case 17 ... 21:
			return 4;
			break;
		case 22 ... 26:
			return 3;
			break;
		case 27 ... 31:
			return 2.5;
			break;
		case 32 ... 36:
			return 2;
			break;
		case 37 ... 42:
			return 1.5;
			break;
		case 43 ... 45:
			return 1.06;
			break;
		default:
			printf("Error in border!\n");						
			return 1;
	}
}

void drawing_square(Figure *fig, double x, double y){
	size_t size = strlen(fig->filename); 
	char *file = (char*)calloc(size, sizeof(char));
	double border, n, sqrt2 = sqrt(2);
	double angle, multiplier;
	Figure* temp;
	strncpy(file, fig->filename, size);
	FILE *filep;
	filep = fopen(file,"a+");
	if(filep==NULL){
		printf("Couldn't open file\n");
		return;
	}
	n = fig->angle_offset.c2 + fig->tile_size / 2; 
	n *= sqrt2;

	angle = cos( RADIAN_TO_DEGREE * (2*fig->angle_offset.c1 + 90) );
	border = sqrt(pow(n,2) / (2*(1-angle)));

	if(fig->angle_offset.c1 < 45){}
	else	fig->angle_offset.c1 = 90 - fig->angle_offset.c1;

	multiplier = (double)(border / border_length(fig->angle_offset.c1));

	//fclose(filep);
	coordinate_draw_top(fig, x, y, multiplier, border);
	coordinate_draw_right(fig, x, y, multiplier, border);
	coordinate_draw_bottom(fig, x, y, multiplier, border);
	coordinate_draw_left(fig, x, y, multiplier, border);
}

int is_even(int num){
	return num%2 + 1; 
}

void export_eps(Figure * fig, char * file_name){
	Point2D* temp  = NULL;
	double temp1, temp2;
	Figure *figTemp;
	int flag=0, flag2=0, counter=0;
	Color c;
	FILE* filep = fopen(file_name,"wt");
	if(filep == NULL){
		printf("Couldn't open file.\n");
		return;
	}

	figTemp = fig;
	if(figTemp != NULL && figTemp->currentPoint != NULL){
		temp = figTemp -> currentPoint;	
		if(temp==NULL){     
			printf("Fig is empty.");
			return;
		}
	}

	for(; figTemp != NULL; figTemp = figTemp->nextptr){
		temp = figTemp -> currentPoint;
		if(fig->archetype==4)	flag=1;
		if(fig->archetype==6)	flag=2;
		fprintf(filep, "%%!PS-Adobe-3.0 EPSF-3.0 \n");
		fprintf(filep, "%%%%BoundingBox: %.3f %.3f %.3f %.3f\n",fig->min.x, fig->min.y, fig-> max.x, fig-> max.y); //
		fprintf(filep, "%d setlinewidth\n%lf %lf %lf setrgbcolor %% background\n", figTemp->pattern_thick,
		    		(double)figTemp->background.red/255.9, (double)figTemp->background.green/255.9, (double)figTemp->background.blue/255.9);
		fclose(filep);
		draw_background(figTemp, figTemp->filename);
		draw_grid(figTemp, figTemp->filename);
		while(flag==1){			//Square
			temp1 = fig->max.y;
			while(temp1 > fig->min.y){
				temp2 = fig->min.x;
				while(temp2 < fig->max.x){
					drawing_square(fig, temp2, temp1);
					temp2 = temp2 + fig->tile_size;
				}
				temp1 = temp1 - fig->tile_size;
			}
			flag=0;
		}
		while(flag==2){		//Hexagon
			temp1 = fig->max.y;
			counter=0;
			while(temp1 > fig->min.y-100){
				temp2 = fig->min.x;
				size_t size = strlen(fig->filename); 
				char *file = (char*)calloc(size, sizeof(char));
				double border, n, sqrt2 = sqrt(2), sq3 = sqrt(3)/2.0;
				//sq3=1;
				double angle, multiplier;
				strncpy(file, fig->filename, size);
				FILE *filep;
				filep = fopen(file,"a+");
				if(filep==NULL){
					printf("Couldn't open file\n");
					return;
				}
				if(is_even(((int)temp1))==0)	flag2=0;
				else	flag2=1;
			
				if(counter%4)	temp2 += fig->tile_size;
				while(temp2 < fig->max.x+100){
					fprintf(filep, "%.4f %.4f moveto\n", temp2, temp1 );
					fprintf(filep,"%.4f %.4f rlineto\n", (double)-fig->tile_size, (double)-fig->tile_size);	 temp = NULL;

					fprintf(filep, "%.4f %.4f moveto\n", temp2 , temp1 ); 
					fprintf(filep,"%.4f %.4f rlineto\n", (double)+fig->tile_size, (double)-fig->tile_size);

					fprintf(filep, "%.4f %.4f moveto\n", temp2 -fig->tile_size, temp1-fig->tile_size); 
					fprintf(filep,"%.4f %.4f rlineto\n", 0.0,  (double)- fig->tile_size*sq3);

					fprintf(filep, "%.4f %.4f moveto\n", temp2 + fig->tile_size, temp1-fig->tile_size); 
					fprintf(filep,"%.4f %.4f rlineto\n", 0.0, (double)- fig->tile_size*sq3);

					fprintf(filep, "%.4f %.4f moveto\n", temp2 + fig->tile_size, temp1 -fig->tile_size*sq3 - fig->tile_size*sq3);
					fprintf(filep,"%.4f %.4f rlineto\n", (double)-fig->tile_size, (double)-fig->tile_size);

					fprintf(filep, "%.4f %.4f moveto\n", temp2 -fig->tile_size, temp1 -fig->tile_size*sq3 - fig->tile_size*sq3);
					fprintf(filep,"%.4f %.4f rlineto\n", (double)+fig->tile_size, (double)-fig->tile_size);
					temp2 = temp2 + fig->tile_size*2;
				}	
				fclose(filep);
				counter+=2;
				temp1 = temp1 - fig->tile_size*2*sq3;
			}
			flag=0;
		}
		c.red = (fig->foreground.red*1.0)/255.9;
		c.green = (fig->foreground.green*1.0)/255.9;
		c.blue = (fig->foreground.blue*1.0)/255.9;
		fprintf(filep, "%lf %lf %lf setrgbcolor %% foreground color\n", c.red, c.green, c.blue);
		fprintf(filep, "stroke\n");
		break;
	}
	fclose(filep);
}