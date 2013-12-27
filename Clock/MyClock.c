#define BORDER_WIDTH 2 /**< Set border width.. **/
#include "../config.h"
#include "../Clock/Clock.h"
#include "../Menu/Menu.h"
#include <X11/Xatom.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <X11/Xutil.h>
#include <time.h>
/********************************************************************************/
/* show a clock                                                                 */
/* Copyright (C) 2012  Ed Kapitein                                              */
/*                                                                              */
/* This program is free software: you can redistribute it and/or modify         */
/* it under the terms of the GNU General Public License as published by         */
/* the Free Software Foundation, either version 3 of the License, or            */
/* (at your option) any later version.                                          */
/*                                                                              */
/* This program is distributed in the hope that it will be useful,              */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of               */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                */
/* GNU General Public License for more details.                                 */
/*                                                                              */
/* You should have received a copy of the GNU General Public License            */
/* along with this program.  If not, see <http://www.gnu.org/licenses/>.        */
/********************************************************************************/

/**
 * \brief Create a new window\n
 * @return a ClockWindow on which we draw.
**/
Window CreateClockWindow(GC *ClockWindowGC)
 {
 char			*FontString;
 char			*WindowName="Clock";
 Window			ClockWindow;
 XColor			color, dummy;
 XEvent			main_xevent;
 XFontStruct		*fontinfo;
 XGCValues		ClockWindowGCValues;
 XSetWindowAttributes	my_windowAttributes;
 XSizeHints		theSizeHints;
 XTextProperty		my_xtextproperty;
 XWMHints		theWMHints;
 unsigned long		my_windowMask;
 int			x;
 int			y;


 x=0;
 y=0;

 MyDisplay = XOpenDisplay(NULL);
 if(MyDisplay==NULL)
  {
  fprintf(stderr,"Opening display failed, check DISPLAY variable\n");
  exit(1);
  }
 
 /*Setting the attributes*/
 MyScreen = DefaultScreen( MyDisplay );
 MyDepth = DefaultDepth( MyDisplay, MyScreen );
 theBlackPixel = BlackPixel( MyDisplay, MyScreen );
 theWhitePixel = WhitePixel( MyDisplay, MyScreen );

 my_windowAttributes.border_pixel	= theBlackPixel;

 XAllocNamedColor( MyDisplay, DefaultColormap( MyDisplay, MyScreen ), "Black", &color, &dummy );
 my_windowAttributes.background_pixel	= color.pixel;

 my_windowAttributes.override_redirect	= False;
 my_windowMask = CWBackPixel|CWBorderPixel|CWOverrideRedirect;
 ClockWindow = XCreateWindow(MyDisplay, RootWindow( MyDisplay, MyScreen ), x, y, Width, Height, BORDER_WIDTH, MyDepth, InputOutput, CopyFromParent, my_windowMask, &my_windowAttributes ); 

 theWMHints.initial_state = NormalState;
 theWMHints.flags = StateHint;
 
 XSetWMHints( MyDisplay, ClockWindow, &theWMHints );
 
 theSizeHints.flags = PPosition | PSize;
 theSizeHints.x = x;
 theSizeHints.y = y;
 theSizeHints.width = Width;
 theSizeHints.height = Height;
 
 XSetNormalHints( MyDisplay, ClockWindow, &theSizeHints );

 XStoreName(MyDisplay, ClockWindow, WindowName );
 XGetTextProperty( MyDisplay, ClockWindow, &my_xtextproperty, XA_WM_NAME );
 my_xtextproperty.value=(unsigned char *) WindowName;
 XSetTextProperty( MyDisplay, ClockWindow, &my_xtextproperty, XA_WM_ICON_NAME );

 FontString = "-*-*-*-*-*-*-*-[0 ~120 120 0]-*-*-*-*-*-*";
 FontString = "-*-Helvetica-*-*-*-*-*-[0 ~220 220 0]-*-*-*-*-*-*";

 MyScreen = DefaultScreen( MyDisplay );
 fontinfo = XLoadQueryFont( MyDisplay, FontString );
 ClockWindowGCValues.font = fontinfo->fid;

 XAllocNamedColor( MyDisplay, DefaultColormap( MyDisplay, MyScreen ), "Red", &color, &dummy );
 ClockWindowGCValues.foreground = color.pixel;

 *ClockWindowGC = XCreateGC( MyDisplay, ClockWindow, GCFont+GCForeground, &ClockWindowGCValues );
 if(*ClockWindowGC == 0)
  {
  XDestroyWindow( MyDisplay, ClockWindow );
  return( (Window) 0);
  }

 XSelectInput( MyDisplay, ClockWindow, StructureNotifyMask );

 XMapWindow( MyDisplay, ClockWindow );

 while(1)
  {
  XWindowEvent( MyDisplay, ClockWindow, StructureNotifyMask, &main_xevent );
  if ( main_xevent.type == MapNotify )
   {
   break;
   }
  }

 XFlush( MyDisplay );

 return ClockWindow;
 }


int main()
 {
 char	*Label[5];
 char	TmpString[10];
 int	X=240;
 int	Y=5;
 int	h1=2;
 int	h2=3;
 int	m1=5;
 int	m2=9;
 int	LapCounter=0;
 Window	PhrankyClockWindow;
 GC	PhrankyClockWindowGC;
 time_t timer;
 struct tm *timerstruct;

 Label[0]="2";
 Label[1]="3";
 Label[2]=":";
 Label[3]="5";
 Label[4]="9";

 PhrankyClockWindow=CreateClockWindow(&PhrankyClockWindowGC);
 
 while(1)
  {
  Y=5;
  timer = time(NULL);
  timerstruct = localtime(&timer);
  h1=( timerstruct->tm_hour / 10 );
  h2=( timerstruct->tm_hour - 10 * h1 );
  m1=( timerstruct->tm_min / 10 );
  m2=( timerstruct->tm_min - 10 * m1 );
 
  sprintf( TmpString,"%i",h1);
  Label[0]=strdup(TmpString);
 
  sprintf( TmpString,"%i",h2);
  Label[1]=strdup(TmpString);
  
  LapCounter = !LapCounter;
  if( LapCounter )
   {
   Label[2]=":";
   }
  else
   {
   Label[2]="";
   }
 
  sprintf( TmpString,"%i",m1);
  Label[3]=strdup(TmpString);
 
  sprintf( TmpString,"%i",m2);
  Label[4]=strdup(TmpString);
 
  XClearWindow(MyDisplay, PhrankyClockWindow);
  XDrawString( MyDisplay, PhrankyClockWindow, PhrankyClockWindowGC, X,  Y, Label[0], strlen( Label[0] ) );
  Y=Y+130;
  XDrawString( MyDisplay, PhrankyClockWindow, PhrankyClockWindowGC, X,  Y, Label[1], strlen( Label[1] ) );
  Y=Y+120;
  XDrawString( MyDisplay, PhrankyClockWindow, PhrankyClockWindowGC, X,  Y, Label[2], strlen( Label[2] ) );
  Y=Y+80;
  XDrawString( MyDisplay, PhrankyClockWindow, PhrankyClockWindowGC, X,  Y, Label[3], strlen( Label[3] ) );
  Y=Y+130;
  XDrawString( MyDisplay, PhrankyClockWindow, PhrankyClockWindowGC, X,  Y, Label[4], strlen( Label[4] ) );
  Y=Y+130;
 
  XFlush( MyDisplay );
  sleep(1);
  }
 return(0);
 }

