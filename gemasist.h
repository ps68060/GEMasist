/*
This file is part of GEMasist.

    GEMasist is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    GEMasist is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with GEMasist.  If not, see <http://www.gnu.org/licenses/>.

*/

#define debug_print(fmt, ...) \
  do { if (DEBUG) fprintf(stderr, fmt, ##__VA_ARGS__); } while (0)


void ApTerm( void);

void winfo( WINDOW *win, int index);
void wclose( WINDOW *win, int index);
void wCheck( WINDOW *win, int index, int mode, char* appName);
void wFsel( WINDOW *win, int index);

void makeConfig(char* appName);

void AddTextLabel( void *dial, int parent, char *obj_label);
void AddCheckBox( void *dial, int parent, ezxml_t object, char *obj_label);
void AddRadioButtons( void   *dial, int     parent, ezxml_t object, char    *obj_label);
void AddButton(  void *dial, int parent, char *obj_label);

void RightButton(void);
