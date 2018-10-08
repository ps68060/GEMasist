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

#include <windom.h>
#include <windom/dfrm.h>
#include "DateUtils.h"

void winfo(		WINDOW *win
					,		int    index)
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


