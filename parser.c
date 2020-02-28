#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "ml6.h"
#include "display.h"
#include "draw.h"
#include "matrix.h"
#include "parser.h"

/*======== void parse_file () ==========
Inputs:   char * filename
          struct matrix * transform,
          struct matrix * pm,
          screen s
Returns:

Goes through the file named filename and performs all of the actions listed in that file.
The file follows the following format:
     Every command is a single character that takes up a line
     Any command that requires arguments must have those arguments in the second line.
     The commands are as follows:
         line: add a line to the edge matrix -
               takes 6 arguemnts (x0, y0, z0, x1, y1, z1)
         ident: set the transform matrix to the identity matrix -
         scale: create a scale matrix,
                then multiply the transform matrix by the scale matrix -
                takes 3 arguments (sx, sy, sz)
         translate: create a translation matrix,
                    then multiply the transform matrix by the translation matrix -
                    takes 3 arguments (tx, ty, tz)
         rotate: create a rotation matrix,
                 then multiply the transform matrix by the rotation matrix -
                 takes 2 arguments (axis, theta) axis should be x y or z
         apply: apply the current transformation matrix to the edge matrix
         display: clear the screen, then
                  draw the lines of the edge matrix to the screen
                  display the screen
         save: clear the screen, then
               draw the lines of the edge matrix to the screen
               save the screen to a file -
               takes 1 argument (file name)
         quit: end parsing

See the file script for an example of the file format

IMPORTANT MATH NOTE:
the trig functions int math.h use radian mesure, but us normal
humans use degrees, so the file will contain degrees for rotations,
be sure to conver those degrees to radians (M_PI is the constant
for PI)
====================*/
void parse_file ( char * filename,
                  struct matrix * transform,
                  struct matrix * edges,
                  screen s) {

  FILE *f;
  char line[256];
  clear_screen(s);

  if (!strcmp(filename, "stdin"))
    f = stdin;
  else
    f = fopen(filename, "r");

  while (fgets(line, 255, f)) {
    line[strlen(line)-1]='\0';
    printf(":%s:\n",line);

    if (!strcmp(line, "line")) {
      fgets(line, 255, f);

      double x0, y0, z0, x1, y1, z1;

      sscanf(line, "%lf %lf %lf %lf %lf %lf\n", &x0, &y0, &z0, &x1, &y1, &z1);

      add_edge(edges,
                x0, y0, z0,
                x1, y1, z1);

    } else if (!strcmp(line, "ident")) {

      ident(transform);

    } else if (!strcmp(line, "scale")) {
      fgets(line, 255, f);

      double sx, sy, sz;
      sscanf(line, "%lf %lf %lf\n", &sx, &sy, &sz);

      struct matrix* scale = make_scale(sx, sy, sz);

      matrix_mult(scale, transform);

    } else if (!strcmp(line, "move")) {
      fgets(line, 255, f);

      double tx, ty, tz;
      sscanf(line, "%lf %lf %lf\n", &tx, &ty, &tz);

      struct matrix* translate = make_translate(tx, ty, tz);

      matrix_mult(translate, transform);

    } else if (!strcmp(line, "rotate")) {
      fgets(line, 255, f);

      double axis, theta;
      sscanf(line, "%c %lf\n", axis, theta);

      struct matrix* rotate;

      switch (axis) {
        case 'x':
          rotate = make_rotX(theta * M_PI / 180);
          break;
        case 'y':
          rotate = make_rotY(theta * M_PI / 180);
          break;
        case 'z':
          rotate = make_rotZ(theta * M_PI / 180);
          break;
        default:
          printf("Input valid axis!\n");
      }

      matrix_mult(rotate, transform);

    } else if (!strcmp(line, "apply")) {
      matrix_mult(transform, edges);
    } else if (!strcmp(line, "display")) {

      clear_screen(s);

      draw_lines(edges, s, (color){0,255,0});

      display(s);
    } else if (!strcmp(line, "save")) {

      char filename[256];
      fscanf(f, "%s\n", filename);

      clear_screen(s);

      draw_lines(edges, s, (color){0,255,0});

      save_extension(s, filename);
    }
  }
}
