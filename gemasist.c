/*
 * GEMasist - a simple application for commanding cli tools via GEM
 *           or for generating config files.
 *           It utilises a simple XML file to set-up the dialogue box.
 *
 *           Uses the EzXML, Windom and DFRM libraries
 *
 * Authors : P Slegg
 * Date    : 2-Dec-2010
 *
 * 23-Jan-2011	P.Slegg			Added ability to create config files.
 *
 * Supports:
 *		text		TextLabel
 *		check		CheckBox
 *		button		Button
 *		radio		Radio Button Group
 *		file		File selector button

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
#include <string.h>
#include <windom.h>
#include <windom/dfrm.h>
#include <ezxml.h>
#include <mintbind.h>
#include <time.h>
#include <unistd.h>
#include "gemasist.h"

# define pexec(m,f,a,b)   Pexec (m, f, a, b)

struct a_buttonFunc
{
	int  attach;
	int  index;
	char *param;
};

static int MaxStringLen = 200;
static int MaxPathLen   = 256;

ezxml_t layout;
int 		radGrpCounter = 0
			 ,checkBoxSelected[100]
			 ,paramCounter
			 ,config = FALSE;

int			DEBUG = 1;																				/* 1 = debug on */

struct a_buttonFunc parameters[200];
char *params[200];
char *filename[256];


void wCheck(WINDOW *win, int index, int mode, char* appName)
{
	int	x;
	char *command;
	char *temp;
	char *quote = "\"";

	printf("wCheck: app name = %s\n", appName);

	if (config == TRUE)
		makeConfig(appName);
	else
	{
		debug_print("DEBUG: Execute application\n");

		/* Build up the command parameters */
		command = (char*)malloc(sizeof(char*) * MaxStringLen);
		strlcpy(command, " ", MaxStringLen);

		strlcat(command, appName, MaxStringLen);
		strlcat(command, " ", MaxStringLen);

		debug_print("Number of Radio Groups=%d\n", radGrpCounter);

		for (x=0; x < paramCounter; x++)
		{
			debug_print("Parameter[%d]\n", x);
			debug_print("  .attach = %d\n", parameters[x].attach);
			debug_print("  .param = %s\n" , parameters[x].param);
			debug_print("  .index = %d\n" , parameters[x].index);
			if (parameters[x].attach == 1)
			{
				strlcat(command, parameters[x].param, MaxStringLen);
				strlcat(command, " ", MaxStringLen);
			}  /* if */
		}  /* for */


    if (strlen(filename) > 0)
    {
			/* Wrap filename in quotes  */
			debug_print("DEBUG: Add filename to command\n");
			strlcat(command, quote, MaxStringLen);
			strlcat(command, filename, MaxStringLen);
			strlcat(command, quote, MaxStringLen);
		}


	  printf("Executing: [%s]\n", command);
		system(command);    // pexec (0, appName, command, NULL);
	}  /* if */

}  /* wCheck */


void getSysDate(char *dateS)
{
	struct tm *Sys_T = NULL;   /* date time */

	time_t Tval = 0;
	Tval = time(NULL);
	Sys_T = localtime(&Tval);

	int Day, Month, Year;

	Day   = Sys_T->tm_mday;
	Month = Sys_T->tm_mon + 1;
	Year  = 1900 + Sys_T->tm_year;

	sprintf(dateS, "%d/%d/%d", Day, Month, Year);
} /* getSysDate */


void makeConfig(char* appName)
{
	const char pathName[] = "afile.cnf";
	FILE *configFile;
	int  aesObject
	    ,x;
	char *command;
	char sysDate[12];

	debug_print("Make config file\n");

	getSysDate(sysDate);
	debug_print("Sys date is %s\n", sysDate);

	configFile = fopen(appName, "w");
	if (configFile == NULL)
	{
		debug_print("ERROR: Can't open the output config file!\n");
		/*aesObject = dfrm_new_label( dial, TYPE_LABEL, "Error: Cannot open the output config file!"); */
		/*dfrm_add(   dial, parentBox, aesObject, 0, -1, DIR_VERT);				/* -4=two char widths from left border; -1=align with demi-height char */
	}
	else
	{
		fprintf(configFile, "# %s\n# Created by GEMasist %s\n#\n", appName, sysDate);
		command = (char*)malloc(sizeof(char*) * MaxStringLen);
		strlcpy(command, " ", MaxStringLen);

		debug_print("Number of Radio Groups=%d\n", radGrpCounter);

		for (x=0; x < paramCounter; x++)
		{
			debug_print("Parameter[%d]\n", x);
			debug_print("  .attach = %d\n", parameters[x].attach);
			debug_print("  .param = %s\n" , parameters[x].param);
			debug_print("  .index = %d\n" , parameters[x].index);
			if (parameters[x].attach == 1)
			{
				strlcpy(command, parameters[x].param, MaxStringLen);
				strlcat(command, "\n", MaxStringLen);
				debug_print("Config line %d: %s\n", x, command);
				fprintf(configFile, command);
			}  /* if */
		}  /* for */
	}  /* if */

	fclose(configFile);
}

/*****************************************************************************/
void AddTextLabel( void *dial
									,int parent
									,char *obj_label)
{
  int aesObject;

	aesObject = dfrm_new_label( dial, TYPE_LABEL, obj_label);
	dfrm_add(   dial, parent, aesObject, 0, -1, DIR_VERT);				/* -4=two char widths from left border; -1=align with demi-height char */
//	dfrm_align( dial, aesObject, DIR_HORI, ALIGN_LEFT);
}  /* AddTextLabel */


void AddCheckBox( void *dial
								,int parent
								,ezxml_t object
								,char *obj_label)
{
	int	 check_box;
	ezxml_t Xml_option;
	char *obj_status;
	char *parameter[MaxStringLen];

	obj_status = ezxml_attr(object, "status");

//	printf("CHECKBOX label=%s status=%s\n"
//	      ,obj_label
//	      ,obj_status);

	check_box = dfrm_new_button( dial, TYPE_XSBUT, obj_label);

	/* Select the required box */
	if (strcmp(ezxml_attr(object,"status"), "on") == 0)
			parameters[paramCounter].attach = 1;

	dfrm_attach( dial, check_box, BIND_VAR, &parameters[paramCounter].attach);	/* attach the button to a variable  */
	dfrm_add( dial, parent, check_box, 0, -1, DIR_VERT);				/* 3. horizontaly aligned in invisible box */

	/* Get the "value" associated with the  check box */
	Xml_option = ezxml_child(object, "option");
	strlcpy (parameter, ezxml_attr(Xml_option, "value"), MaxStringLen);

	parameters[paramCounter].param = (char*)malloc(sizeof(char*) * MaxStringLen);
	strlcpy(parameters[paramCounter].param, parameter, MaxStringLen);
	parameters[paramCounter].index = check_box;

	debug_print("value=[%s]\n", parameters[paramCounter].param);
	paramCounter++;
}  /* AddCheckBox */


void AddRadioButtons( void   *dial
										,int     parent
										,ezxml_t object
										,char    *obj_label)
{
	int  radioGroup
			,radioButtn;
	ezxml_t  xmlRadButton;
	char *parameter[20];

	radioGroup = dfrm_new_tbox( dial, 0, 0, obj_label);			/* create a radio group */

	/* add the radio buttons to the group */
	for (xmlRadButton = ezxml_child(object, "button"); xmlRadButton; xmlRadButton = xmlRadButton->next)
	{
		radioButtn = dfrm_new_button( dial, TYPE_XRBUT, ezxml_attr(xmlRadButton, "label") );

		/* Get the "value" associated with the radio button */
		strlcpy (parameter, ezxml_attr(xmlRadButton, "value"), MaxStringLen);

		parameters[paramCounter].param = (char*)malloc(sizeof(char*) * MaxStringLen);
		strlcpy(parameters[paramCounter].param, parameter, MaxStringLen);
		parameters[paramCounter].index = radioButtn;

		debug_print("value=[%s]\n", parameters[paramCounter].param);

		/* Select the required button */
		if (strcmp(ezxml_attr(xmlRadButton,"status"), "on") == 0)
		{
			debug_print("  selected button ");
			parameters[paramCounter].attach = 1;
//			dfrm_setstate( dial, radioButtn, SELECTED, TRUE);
		}

		debug_print("radio_bttn=%d\n\n", radioButtn);
		dfrm_attach( dial, radioButtn, BIND_VAR, &parameters[paramCounter].attach);						/* attach the button to a variable  */
		dfrm_add( dial, radioGroup, radioButtn, 10, 10, DIR_VERT);
		
		paramCounter++;
	}  /* for */

	dfrm_add( dial, parent, radioGroup, 0, -2, DIR_VERT);		/* add group to the form orientated vertically */

	radGrpCounter++;

}  /* AddRadioButtons */


void AddButton(  void *dial
								,int parent
								,char *obj_label)
{
	int aesObject;

//	printf("BUTTON label=%s\n" ,obj_label );
	aesObject = dfrm_new_button( dial, TYPE_XDBUT, obj_label);
	dfrm_attach( dial, aesObject, BIND_FUNC, winfo);						/* attach the button to the function  */
	dfrm_add( dial, parent, aesObject, 0, -1, DIR_VERT);
}  /* AddButton */



void AddFselButton(  void *dial
										,int parent
										,char *obj_label)
{
	int aesObject;
	char *parameter[20];

//	printf("File selector BUTTON label=%s\n" ,obj_label );
	aesObject = dfrm_new_button( dial, TYPE_XDBUT, obj_label);
	dfrm_attach( dial, aesObject, BIND_FUNC, wFsel);						/* attach the button to the function  */
	dfrm_add( dial, parent, aesObject, 0, -1, DIR_VERT);
}  /* AddFselButton */



char* addXmlObjects(void* dial, int parent)
{/* add xml objects to form */
	int i=0;

	ezxml_t form
	       ,object;

	char *formType
			,*appName
	    ,*obj_type
	    ,*obj_label
	    ,*obj_status
	    ,*obj_config;

//	printf("addXmlObjects\n");

	for (form = ezxml_child(layout, "form"); form; form = form->next)
	{
		formType = ezxml_attr(form, "type");
		if (strcmp("config", formType) == 0)
		{
			printf("Type = config file\n");
			config = TRUE;
		}

		/* Add the name of the application to the form */
		appName = ezxml_attr(form, "path");
		//printf ("Run application %s\n", appName);
		AddTextLabel(dial, parent, appName);

		for (object = ezxml_child(form, "object"); object; object = object->next)
		{
			i++;
			obj_type   = ezxml_attr(object, "type");
			obj_label  = ezxml_attr(object, "label");
			debug_print("%d: type=%s label=%s status=%s \n"
			      ,i
			      ,obj_type
			      ,obj_label
			      ,ezxml_attr(object, "status") );

			if ( strcmp(obj_type,"text") == 0)												/* Text Label */
			{
				AddTextLabel(dial, parent, obj_label);
			}
			else if ( strcmp(obj_type,"check") == 0)									/* Checkbox */
			{
				AddCheckBox(dial, parent, object, obj_label);
			}
			else if ( strcmp(obj_type,"button") == 0)									/* Button */
			{
				AddButton(dial, parent, obj_label);
			}
			else if ( strcmp(obj_type,"radio") == 0)									/* Radio Button group */
			{
				AddRadioButtons(dial, parent, object, obj_label);
			}
			else if ( strcmp(obj_type,"file") == 0)										/* File selector button */
			{
				AddFselButton(dial, parent, obj_label);
//				AddTextLabel(dial, parent, obj_label);
			}


		}  /* for */

	  /* 4. add invisible box to ROOT object */
//    dfrm_add( dial, ROOT, parent, 0, 0, DIR_VERT);
//		dfrm_align( dial, parent, DIR_HORI, ALIGN_CENTER);

	}  /* for */

	return appName;
}  /* addXmlObjects */


void ApTerm(void)
{
//	printf("in ApTerm:1\n");
	/* catch the AP_TERM message (application termination) */
	while( wglb.first)
	{
	ApplWrite( _AESapid, WM_DESTROY, wglb.first->handle, 0, 0, 0, 0);
	EvntWindom( MU_MESAG);
  }
  ApplExit();
  exit( EXIT_SUCCESS);
}


void wclose( WINDOW *win, int index)
{
	ezxml_free(layout);
	BubbleFree();
	ObjcChange( OC_FORM, win, index, NORMAL, TRUE);
	ApplWrite( _AESapid, AP_TERM, 0, 0, 0, 0, 0);
}


void winfo( WINDOW *win, int index)
{
	FormAlert( 1, 
		"[1][Compiled using|WinDom v%x.%02x"
		"|DFRM v%x.%02x][OK]", 
		WinDom.patchlevel >> 8 ,
		WinDom.patchlevel & 0x00FF,
		__DFRM_MAJOR__,__DFRM_MINOR__);

	ObjcChange( OC_FORM, win, index, NORMAL, TRUE);
}


void wFsel( WINDOW *win, int index)
/** Purpose: When the fileselector is called, get the filename and return it.
 */
{
	static char path[255] = ""; // First usage : current directory

	if( FselInput(path, filename, "*.*", "View text file", NULL, NULL)) 
	{
		char fullname[MaxPathLen];

		strlcpy( fullname, path, MaxPathLen);
		strlcat( fullname, "\\", MaxPathLen);
		strlcat( fullname, filename, MaxPathLen);
		strlcpy( filename, fullname, MaxPathLen);

		debug_print("DEBUG: filename = %s\n", filename);
		return 1;
	}
	else
		return 0;  // error
}  // wFsel


void RightButton(void)
{
	if (evnt.mbut & 0x2)
		BubbleEvnt();
}


void main(int argc, char *argv[])
{
	WINDOW *win;
	void   *dial;
	char   *appName;
	int  aesObject
			,parentBox
			,buttonbox
			,mainBox;

	int opt;
	enum {NONDEBUG_MODE, DEBUG_MODE} mode = NONDEBUG_MODE;

	ApplInit();																							/* WinDOm initialisation */

	dial = dfrm_create( 20, TYPE_NORMAL);										/* Create the dialogue pointer */

	/* 1. Create an Invisible BOX for the xml objects */
	parentBox = dfrm_new_box( dial, 0, 0, 0, 0);

	debug_print("DEBUG: argc = %d\n", argc);

	while ((opt = getopt(argc, argv, "d")) != -1)
	{
		switch (opt)
		{
			case 'd' : mode = DEBUG_MODE; break;
			default  : mode = NONDEBUG_MODE;
		}
	}  // while

	debug_print("DEBUG: optind = %d\n", optind);

	DEBUG = mode;
	debug_print("DEBUG has been turned ON\n");

	if (optind >= argc)
	{
		debug_print("DEBUG: Supply a .xml file\n");
		aesObject = dfrm_new_label( dial, TYPE_LABEL, "Error: GEMasist relies on a .xml file");
		dfrm_add(   dial, parentBox, aesObject, 0, -1, DIR_VERT);				/* -4=two char widths from left border; -1=align with demi-height char */
	}
	if (argc - optind == 1)
	{
		layout = ezxml_parse_file(argv[argc-1]);								/* parse the xml file */
	  appName = addXmlObjects(dial, parentBox);								/* add xml objects to form */
	  printf("app name = %s\n", appName);
	}

	/* Create a second Invisible BOX for the control buttons */
	buttonbox = dfrm_new_box( dial, 0, 0, 0, 0);

	/* 2. create button 1 */
	aesObject = dfrm_new_button( dial, TYPE_XDBUT, "[Apply");
	dfrm_attach( dial, aesObject, BIND_FUNC, wCheck, appName);	/* attach the button to the wCheck function */
	dfrm_add( dial, buttonbox, aesObject, 0, -1, DIR_HORI);			/* horizontaly aligned in invisible box */

	/* create button 2 */
	aesObject = dfrm_new_button( dial, TYPE_XEBUT, "[Exit");
	dfrm_attach( dial, aesObject, BIND_FUNC, wclose);						/* attach the button to the function  */
	dfrm_add( dial, buttonbox, aesObject, -4, -1, DIR_HORI);

	BubbleAttach (dial, aesObject, "bubble help success");

	/* 4. add invisible box to the ROOT object, one above the other */
	mainBox = dfrm_new_box( dial, 0, 0, 0, 0);

	dfrm_add( dial, mainBox, parentBox, -4, 0, DIR_VERT);
	dfrm_add( dial, mainBox, buttonbox, -4, 0, DIR_VERT);

	dfrm_add( dial, ROOT, mainBox, -4, 0, DIR_VERT);

	dfrm_align( dial, mainBox, DIR_VERT, ALIGN_LEFT);
	dfrm_align( dial, mainBox, DIR_VERT, ALIGN_CENTER);

	dfrm_repack( dial);
	win = dfrm_form( dial, WAT_FORM, "GEMasist", TRUE);		/* place the dialogue into a window */
	EvntAttach( NULL, AP_TERM, ApTerm);
	EvntAttach( NULL, WM_XBUTTON, RightButton);

	for( ;;) EvntWindom( MU_MESAG|MU_BUTTON);													/* Handle GEM events */
//	while (wglb.first) EvntWindom(MU_MESAG);
}
