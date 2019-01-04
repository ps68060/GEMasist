/*
 * Authors : P Slegg
 * Date    : 2-Dec-2010
 *

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

#include <stdlib.h>
#include <stdio.h>
#include <windom.h>
#include <windom/dfrm.h>
#include <ezxml.h>
#include "DateUtils.h"
#include "DfrmGEMasist.h"
#include "gemasist.h"

static int MaxPathLen   = 256;

void winfo( WINDOW *win
					, int    index)
{
  char sysDate[12];

  getSysDate(sysDate);

	FormAlert( 1
		,"[1][GEMasist is Built using:||WinDom v%x.%02x"
		"|DFRM v%x.%02x||v%s][OK]"
		,WinDom.patchlevel >> 8 
		,WinDom.patchlevel & 0x00FF
		,__DFRM_MAJOR__,__DFRM_MINOR__
    ,sysDate
    );

	ObjcChange( OC_FORM, win, index, NORMAL, TRUE);
}  // wInfo


int wFsel(  WINDOW *win
          , int    index
          , char   *filename)
/** Purpose: When the fileselector is called, get the filename and return it.
 */
{
  static char path[256] = ""; // First usage : current directory

  if( FselInput(path, filename, "*.*", "View text file", NULL, NULL)) 
  {
    char fullname[256] = "";

    strlcpy( fullname, path, MaxPathLen);
    strlcat( fullname, "\\", MaxPathLen);
    strlcat( fullname, filename, MaxPathLen);
//    strlcpy( parameters[paramCounter].filename, fullname, MaxPathLen);
    strlcpy( filename, fullname, MaxPathLen);

//    snprintf(fullname, "%s\\%s", path, filename);
    printf("[%s\\%s]\n", path, filename);

    debug_print("DEBUG: filename = %s\n", filename);
    return 1;
  }
  else
    return 0;  // error
}  // wFsel

