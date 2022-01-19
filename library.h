#ifndef _LIBRARY_H_
#define _LIBRARY_H_
/*
	This is the header file for EPS square and hexagon patterns and motifs creating.
	Name-Surname: Suleyman Golbol
	Number: 1801042656
*/

/*
		------ PLEASE READ ------
	To make program working; 
	in commands file, it should be a space character after every information for parsing it.
	For ex:		 pattern_thickness:8, tile_size:80
	It should be : character before info content.
	For ex:		 pattern_thickness:8	
*/

typedef struct Color{
	double red;
	double green;
	double blue;
}Color;

typedef struct Point2D{
	double x,y;
	int size;
	struct Point2D* nextptr;
}Point2D;

//////////////PART2///////////////////////
typedef struct Components{
	double c1;
	double c2;
}Components;

typedef struct LineInfo{
	Color background;
	Color foreground;
	Color grid;
	Color col;//

	int tile_size;
	int pattern_thick;
	int show_grid; // 1 or 0
	int archetype; // 4 or 6

	Point2D max;
	Point2D min;
	Point2D *currentPoint;
	
	double gridthick, res;
	char *filename;

	Components angle_offset;
	Components canvas_size;

	struct LineInfo* nextptr;
}Figure;

					/*---From PART1------*/
/* Initializes the figure on canvas of a given dimension (width × height) */
Figure* start_figure(double width, double height, Figure *figure);
/* Set the colour for the drawings to happen next.  */
void set_color(Figure* fig, Color c);
/* Exports the current figure to an EPS file */
void export_eps(Figure * fig, char * file_name);
					/*------PART2------*/
/* Reading commands */
void read_file();
/* Reading default commands */
Figure* read_default(char* file_name);
/* Finding essential words archetype/file_name	*/
int essential_words(char *string);
/* Comparing names in the file */
int name_finder(char *piece);
/* Draws grid */
void draw_grid(Figure *info, char *file_name);
/* Sets background color */
void draw_background(Figure *fig, char *file_name);
/* Sets border length */
double broder_length(double angle);
/* Draws square pieces */
void coordinate_draw_top(Figure *fig, double x, double y, double multiplier, double angle);
void coordinate_draw_left(Figure *fig, double x, double y, double multiplier, double angle);
void coordinate_draw_bottom(Figure *fig, double x, double y, double multiplier, double angle);
void coordinate_draw_right(Figure *fig, double x, double y, double multiplier, double angle);
/* Drawing Square */
void drawing_square(Figure *fig, double x, double y);
#endif