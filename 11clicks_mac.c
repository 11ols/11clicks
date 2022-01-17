/*
Max/Msp external to simulate mouse input
11oLsen.de, 2014,2015,2020
*/

#include "ext.h"		// standard Max include, always required
#include "ext_obex.h"	// required for new style Max object

#import <Carbon/Carbon.h>

////////////////////////// object struct
typedef struct _clicky 
{
	t_object					ob; // the object itself (must be first)
    t_object	*w_patcher;
    t_object	*w_patcherview;
} t_clicky;

///////////////////////// function prototypes
//// standard set
void *clicky_new(t_symbol *s, long argc, t_atom *argv);
void clicky_free(t_clicky *x);
void clicky_assist(t_clicky *x, void *b, long m, long a, char *s);

void clicky_doubleclick(t_clicky *x, t_symbol *s, long argc, t_atom *argv);
void clicky_do_doubleclick(t_clicky *x, t_symbol *s, long argc, t_atom *argv);

void clicky_set(t_clicky *x, t_symbol *s, long argc, t_atom *argv);
void clicky_do_set(t_clicky *x, t_symbol *s, long argc, t_atom *argv);

void clicky_move(t_clicky *x, t_symbol *s, long argc, t_atom *argv);
void clicky_do_move(t_clicky *x, t_symbol *s, long argc, t_atom *argv);

void clicky_wheel(t_clicky *x, t_symbol *s, long argc, t_atom *argv);
void clicky_do_wheel(t_clicky *x, t_symbol *s, long argc, t_atom *argv);

void clicky_hwheel(t_clicky *x, t_symbol *s, long argc, t_atom *argv);
void clicky_do_hwheel(t_clicky *x, t_symbol *s, long argc, t_atom *argv);

void clicky_leftclick(t_clicky *x, t_symbol *s, long argc, t_atom *argv);
void clicky_do_leftclick(t_clicky *x, t_symbol *s, long argc, t_atom *argv);

void clicky_rightclick(t_clicky *x, t_symbol *s, long argc, t_atom *argv);
void clicky_do_rightclick(t_clicky *x, t_symbol *s, long argc, t_atom *argv);

void clicky_cursor(t_clicky *x, t_symbol *s, long argc, t_atom *argv);
void clicky_do_cursor(t_clicky *x, t_symbol *s, long argc, t_atom *argv);

void clicky_init(t_clicky *x);

//////////////////////// global class pointer variable
void *clicky_class;
CGEventTapLocation tapLocation;
CGEventSourceRef sourceRef;


int C74_EXPORT main(void)
{
	t_class *c;
	
	c = class_new("11clicks", (method)clicky_new, (method)clicky_free, (long)sizeof(t_clicky), 0L /* leave NULL!! */, A_GIMME, 0);
	
    class_addmethod(c, (method)clicky_assist,			"assist",		A_CANT, 0);  
	class_addmethod(c, (method)clicky_doubleclick, "doubleclick",    A_GIMME, 0);
    
    class_addmethod(c, (method)clicky_set,        "set",                A_GIMME, 0);
    class_addmethod(c, (method)clicky_move,        "move",                A_GIMME, 0);
    class_addmethod(c, (method)clicky_wheel,    "wheel",            A_GIMME, 0);
    class_addmethod(c, (method)clicky_hwheel,    "hwheel",            A_GIMME, 0);
    class_addmethod(c, (method)clicky_cursor,    "cursor",            A_GIMME, 0);

    class_addmethod(c, (method)clicky_leftclick, "leftclick",    A_GIMME, 0);
    class_addmethod(c, (method)clicky_rightclick, "rightclick", A_GIMME, 0);

	class_register(CLASS_BOX, c); /* CLASS_NOBOX */
	clicky_class = c;

    tapLocation = kCGHIDEventTap; // used when specifying the tap location for CGEventPost
	sourceRef = CGEventSourceCreate(kCGEventSourceStateCombinedSessionState);
    //CGEventSourceSetPixelsPerLine(sourceRef, 1.);
    common_symbols_init();
    
	object_post(NULL, "11clicks 2022/01/17 11OLSEN.DE");
	
	return 0;
}

void clicky_doubleclick(t_clicky *x, t_symbol *s, long argc, t_atom *argv)
{
    defer_low(x, (method)clicky_do_doubleclick, s, argc, argv);
}
void clicky_do_doubleclick(t_clicky *x, t_symbol *s, long argc, t_atom *argv)
{
    // get the current mouse location
	CGEventRef mouseEvent = CGEventCreate(NULL);
	CGPoint mouseLoc = CGEventGetLocation(mouseEvent);
	CFRelease(mouseEvent);
	
	// NOTE: the first mouse down and mouse up are not really needed to perform a double click
	// I only do that because sometimes you have to click once to bring whatever you want to double-click to the front
	
	// first click mouse
	CGEventRef clickMouse = CGEventCreateMouseEvent(sourceRef, kCGEventLeftMouseDown, mouseLoc, 0);
	CGEventSetIntegerValueField(clickMouse, kCGMouseEventClickState, 1);
	CGEventPost(tapLocation, clickMouse);
	CFRelease(clickMouse);
	
	// first mouse up
	CGEventRef releaseMouse = CGEventCreateMouseEvent(sourceRef, kCGEventLeftMouseUp, mouseLoc, 0);
	CGEventSetIntegerValueField(releaseMouse, kCGMouseEventClickState, 1);
	CGEventPost(tapLocation, releaseMouse);
	CFRelease(releaseMouse);
	
	// second click mouse
	CGEventRef clickMouse2 = CGEventCreateMouseEvent(sourceRef, kCGEventLeftMouseDown, mouseLoc, 0);
	CGEventSetIntegerValueField(clickMouse2, kCGMouseEventClickState, 2);
	CGEventPost(tapLocation, clickMouse2);
	CFRelease(clickMouse2);
	
	// second mouse up
	CGEventRef releaseMouse2 = CGEventCreateMouseEvent(sourceRef, kCGEventLeftMouseUp, mouseLoc, 0);
	CGEventSetIntegerValueField(releaseMouse2, kCGMouseEventClickState, 2);
	CGEventPost(tapLocation, releaseMouse2);
	CFRelease(releaseMouse2);
}



void clicky_leftclick(t_clicky *x, t_symbol *s, long argc, t_atom *argv)
{
    if (!argc) return;
    defer_low(x, (method)clicky_do_leftclick, s, argc, argv);
}
void clicky_do_leftclick(t_clicky *x, t_symbol *s, long argc, t_atom *argv)
{
	long n = atom_getlong(argv + 0);
	if (n) 
	{
		
        
            // get the current mouse location
            CGEventRef mouseEvent = CGEventCreate(NULL);
            CGPoint mouseLoc = CGEventGetLocation(mouseEvent);
          CFRelease(mouseEvent);
            
            // click mouse
            CGEventRef clickMouse = CGEventCreateMouseEvent(sourceRef, kCGEventLeftMouseDown, mouseLoc, 0);
            CGEventPost(tapLocation, clickMouse);
         CFRelease(clickMouse);// left-click at current mouse location but do not release the mouse
        
        
        
        
	} 
	else 
	{
		// get the current mouse location
        CGEventRef mouseEvent = CGEventCreate(NULL);
        CGPoint mouseLoc = CGEventGetLocation(mouseEvent);
     CFRelease(mouseEvent);
        
        // release mouse
        CGEventRef releaseMouse = CGEventCreateMouseEvent(sourceRef, kCGEventLeftMouseUp, mouseLoc, 0);
        CGEventPost(tapLocation, releaseMouse);
      CFRelease(releaseMouse);
	}
}





void clicky_rightclick(t_clicky *x, t_symbol *s, long argc, t_atom *argv)
{
    if (!argc) return;
    defer_low(x, (method)clicky_do_rightclick, s, argc, argv);
}
void clicky_do_rightclick(t_clicky *x, t_symbol *s, long argc, t_atom *argv)
{
    long n = atom_getlong(argv + 0);
	if (n) 
	{
        // get the current mouse location
        CGEventRef mouseEvent = CGEventCreate(NULL);
        CGPoint mouseLoc = CGEventGetLocation(mouseEvent);
      CFRelease(mouseEvent);
        
        // click mouse
        CGEventRef clickMouse = CGEventCreateMouseEvent(sourceRef, kCGEventRightMouseDown, mouseLoc, 0);
        CGEventPost(tapLocation, clickMouse);
     CFRelease(clickMouse);// click at current mouse location but do not release the mouse
	}
	else
	{
		// get the current mouse location
        CGEventRef mouseEvent = CGEventCreate(NULL);
        CGPoint mouseLoc = CGEventGetLocation(mouseEvent);
      CFRelease(mouseEvent);
        
        // release mouse
        CGEventRef releaseMouse = CGEventCreateMouseEvent(sourceRef, kCGEventRightMouseUp, mouseLoc, 0);
        CGEventPost(tapLocation, releaseMouse);
     CFRelease(releaseMouse);
	}
}


void clicky_set(t_clicky *x, t_symbol *s, long argc, t_atom *argv)
{
    if (argc<2) return;
    defer_low(x, (method)clicky_do_set, s, argc, argv);
}
void clicky_do_set(t_clicky *x, t_symbol *s, long argc, t_atom *argv)
{
	long h; long v;

	h = atom_getfloat(argv+0);
	v = atom_getfloat(argv+1);

        //CGWarpMouseCursorPosition(CGPointMake(h, v));
        CGEventRef moveMouse = CGEventCreateMouseEvent(sourceRef, kCGEventMouseMoved, CGPointMake(h, v), 0);
        CGEventPost(tapLocation, moveMouse);
        CFRelease(moveMouse);
        // mouse jumps directly from its current position to the new position and becomes visible, origin of CGPoint must be top-left
}



void clicky_move(t_clicky *x, t_symbol *s, long argc, t_atom *argv)
{
    if (argc<2) return;
    defer_low(x, (method)clicky_do_move, s, argc, argv);
}
void clicky_do_move(t_clicky *x, t_symbol *s, long argc, t_atom *argv)
{
    long h; long v;
    
	h = atom_getfloat(argv+0);
	v = atom_getfloat(argv+1);
    
    // get the current mouse location
	CGEventRef mouseEvent = CGEventCreate(NULL);
	CGPoint currentLoc = CGEventGetLocation(mouseEvent);
	CFRelease(mouseEvent);

    
    //CGWarpMouseCursorPosition(CGPointMake(h, v));
    CGEventRef moveMouse = CGEventCreateMouseEvent(sourceRef, kCGEventMouseMoved, CGPointMake((currentLoc.x+h), (currentLoc.y+v)), 0);
    CGEventPost(tapLocation, moveMouse);
    CFRelease(moveMouse);
}


void clicky_wheel(t_clicky *x, t_symbol *s, long argc, t_atom *argv)
{
    if (!argc) return;
    defer_low(x, (method)clicky_do_wheel, s, argc, argv);
}
void clicky_do_wheel(t_clicky *x, t_symbol *s, long argc, t_atom *argv)
{
    long n = atom_getlong(argv + 0);
    
    CGEventRef scrollWheel = CGEventCreateScrollWheelEvent(sourceRef, kCGScrollEventUnitPixel, 2, n, 0);
    CGEventPost(tapLocation, scrollWheel);
    CFRelease(scrollWheel);
}


void clicky_hwheel(t_clicky *x, t_symbol *s, long argc, t_atom *argv)
{
    if (!argc) return;
    defer_low(x, (method)clicky_do_hwheel, s, argc, argv);
}
void clicky_do_hwheel(t_clicky *x, t_symbol *s, long argc, t_atom *argv)
{
    long n = atom_getlong(argv + 0);
    
    CGEventRef scrollWheel = CGEventCreateScrollWheelEvent(sourceRef, kCGScrollEventUnitPixel, 2, 0, n);
    CGEventPost(tapLocation, scrollWheel);
    CFRelease(scrollWheel);
}


////////////// CURSOR ///////////////////////////////////////////////
void clicky_cursor(t_clicky *x, t_symbol *s, long argc, t_atom *argv)
{
    defer_low(x, (method)clicky_do_cursor, s, argc, argv);
}
void clicky_do_cursor(t_clicky *x, t_symbol *s, long argc, t_atom *argv)
{
	
    /*typedef enum _jmouse_cursortype {
     JMOUSE_CURSOR_NONE, 						///< None
     JMOUSE_CURSOR_ARROW, 						///< Arrow
     JMOUSE_CURSOR_WAIT, 						///< Wait
     JMOUSE_CURSOR_IBEAM, 						///< I-Beam
     JMOUSE_CURSOR_CROSSHAIR, 					///< Crosshair
     JMOUSE_CURSOR_COPYING,						///< Copying
     JMOUSE_CURSOR_POINTINGHAND,					///< Pointing Hand
     JMOUSE_CURSOR_DRAGGINGHAND,					///< Dragging Hand
     JMOUSE_CURSOR_RESIZE_LEFTRIGHT,				///< Left-Right
     JMOUSE_CURSOR_RESIZE_UPDOWN,				///< Up-Down
     JMOUSE_CURSOR_RESIZE_FOURWAY,				///< Four Way
     JMOUSE_CURSOR_RESIZE_TOPEDGE,				///< Top Edge
     JMOUSE_CURSOR_RESIZE_BOTTOMEDGE,			///< Bottom Edge
     JMOUSE_CURSOR_RESIZE_LEFTEDGE,				///< Left Edge
     JMOUSE_CURSOR_RESIZE_RIGHTEDGE,				///< Right Edge
     JMOUSE_CURSOR_RESIZE_TOPLEFTCORNER,			///< Top-Left Corner
     JMOUSE_CURSOR_RESIZE_TOPRIGHTCORNER,		///< Top-Right Corner
     JMOUSE_CURSOR_RESIZE_BOTTOMLEFTCORNER,		///< Bottom-Left Corner
     JMOUSE_CURSOR_RESIZE_BOTTOMRIGHTCORNER		///< Bottom-Right Corner
     } t_jmouse_cursortype;*/
    
    
	t_object *jb;
	t_object *box;
	//t_rect jr;
	t_symbol *scriptingname;
	t_symbol *vname;
	long n;
	long i;
	
	jb = jpatcher_get_firstobject(x->w_patcher); // get the first BOX in the object list
    box = jpatcher_get_box(x->w_patcherview); // get box of firstview
	
	if  (!argc || !argv){object_post((t_object *)x, "missing arguments"); return;}
	
	if  (argc == 1) {               // apply to patcherview if only 1 arg
		n = atom_getlong(argv+0);
		jmouse_setcursor (x->w_patcherview, box, n);
        
		object_method(x->w_patcher, _sym_refresh); //
		return;
	}
    
	//now the case of more than 1 arg
	if ((argv + 0)->a_type == A_LONG) {n = atom_getlong(argv+0);}
	else {object_post((t_object *)x, "first arg needs to be the cursor number 0-18");return;}
	
	if (atom_getsym(argv+1) == gensym("all")) // if second arg is "all", the defined cursor is applied to all objects in the patcher
	{
		while(jb) {
			jmouse_setcursor (x->w_patcherview, jb, n);
			jb = jbox_get_nextobject(jb);
		}
		object_method(x->w_patcher, _sym_refresh); return;
	}
    
	for (i = 1; i < argc; i++) {			//for every argument starting with the second
		if ((argv + i)->a_type == A_SYM)		// if arg is a symbol
		{
			vname = atom_getsym(argv+i);
			jb = jpatcher_get_firstobject(x->w_patcher);
			
            
			while(jb) {            // iterate to all objects in the patcher and apply cursor if name matches
                
				scriptingname = jbox_get_varname(jb);              // scripting name
				if (scriptingname  == vname)
				{
					jmouse_setcursor (x->w_patcherview, jb, n);
                    
				}
                
				jb = jbox_get_nextobject(jb);
                
			}// end while
            
            
		} else  {object_post((t_object *)x, "arg %ld is not a scripting-name", i+1);}
	} // end for
	// maybe upwards? jpatcher_get_parentpatcher(<#t_object * p#>)
    
	object_method(x->w_patcher, _sym_refresh);
}




void clicky_assist(t_clicky *x, void *b, long m, long a, char *s)
{
    ;
}


void clicky_free(t_clicky *x)
{
	;
}


void *clicky_new(t_symbol *s, long argc, t_atom *argv)
{
	t_clicky *x = NULL;
    
	if ((x = (t_clicky *)object_alloc(clicky_class))) 
	{
		object_obex_lookup(x, _sym_pound_P, &x->w_patcher);   // assign w_patcher
		
        defer_low(x, (method)clicky_init, NULL, 0, NULL);
	}
	return (x);
}


void clicky_init(t_clicky *x)
{
    x->w_patcherview = object_attr_getobj(x->w_patcher, _sym_firstview); //get firstview object of w_patcher
}
