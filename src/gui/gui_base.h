
// types:  ../gui/gui_types.h

// gio_src  obj-nr; first=window=0; next widget=1 ..


// all / Obj_Box Obj_Label
typedef struct { MemObj mem_obj;                 // memory-block: nr, offset
                 short gio_typ;                  // gui-widget-typ
                 short gio_src;                  // gui-widget-nr
                 void  *widget;                  // gtk-widget-pointer
                }                                        Obj_Unknown;

// TYP_GUI_Notebook
typedef struct { MemObj mem_obj;
                 short gio_typ;
                 short gio_src;
                 void  *widget;
                 void  (*uFunc)();
                }                                        Obj_gui1;


// Obj_Button Obj_Entry TYP_GUI_MenuEntry
typedef struct { MemObj mem_obj;
                 short gio_typ;
                 short gio_src;
                 void  *widget;
                 void  (*uFunc)();
                 void  *data;
                }                                        Obj_gui2;


typedef struct { MemObj mem_obj;
                 short gio_typ;
                 short gio_src;
                 void  *win;
                 void  (*uFunc)();
                }                                        Obj_Win;

typedef struct { MemObj mem_obj;
                 short gio_typ;
                 short gio_src;
                 void  *box;
                }                                        Obj_Box;

typedef struct { MemObj mem_obj;
                 short gio_typ;
                 short gio_src;
                 void  *button;
                 void  (*uFunc)();
                 void  *data;
                }                                        Obj_Button;

typedef struct { MemObj mem_obj;
                 short gio_typ;
                 short gio_src;
                 void  *label;
                }                                        Obj_Label;

// TYP_GUI_Slider
typedef struct { MemObj mem_obj;
                 short gio_typ;
                 short gio_src;
                 void  *slider;
                 void  (*uFunc)();
                }                                        Obj_Slider;

// TYP_GUI_Entry
typedef struct { MemObj mem_obj;
                 short gio_typ;
                 short gio_src;
                 void  *entry;
                 void  (*uFunc)();
                 void  *data;
                }                                        Obj_Entry;

// TYP_GUI_GLwin
typedef struct { MemObj mem_obj;
                 short gio_typ;
                 short gio_src;
                 void  *widget;
                 void  (*uFuDraw)();
                 void  (*uFuMove)();
                 void  (*uFuButt)();
                 int   (*uFuKey)();
                }                                        Obj_GLwin;



#define GUI_OBJ_NEW {0, 0}

/// set MemObj invalid
/// Usage:
///   in callBack from window-exit ((GUI_DATA_EVENT == TYP_EventExit)
///     window = GUI_OBJ_INVALID ();
///   test with if(GUI_OBJ_IS_VALID(&window)) window-is-valid
#define GUI_OBJ_INVALID() UME_obj_invalid_set (-4)

/// test if GUI-object is valid; returns: 1 is OK; 0 is invalid object.
/// MemObj *mo;  if(GUI_OBJ_IS_VALID(mo)) obj-is-valid .
///              if(!GUI_OBJ_IS_VALID(mo)) obj-is-not-valid .
#define GUI_OBJ_IS_VALID(mo1) ((mo1)->ioff >= 4)

/// compare GUI-objects; 0=identical; 1=different widgets.
///void   GUI_obj_cmp (MemObj *mo1, MemObj *mo2);
///       if(!GUI_obj_cmp (&mo1, &mo2)) objs-are-identical;
#define GUI_obj_cmp(mo1,mo2) memcmp(mo1,mo2,sizeof(MemObj))

// make MemObj from GUI-obj (Obj_Unknown)
#define MEMOBJ_GUIOBJ(GO) ((Obj_Unknown*)GO)->mem_obj

// copy pointer -> MemObj
// void *vp1; MemObj mo;
// GUI_MO_PTR (&mo, &vp1)
#define GUI_MO_PTR(mo,vp1) memcpy (mo, vp1, sizeof(void*))


  int    GUI_obj_init ();
  int    GUI_obj_save ();
  void   GUI_obj_clear ();
  MemObj GUI_obj_spc (void **oPos, int siz);
  MemObj GUI_obj_tmp (void **oPos, int siz);
  void*  GUI_obj_pos (MemObj *mo);
  Memspc* GUI_obj__  ();
  int    GUI_obj_src (MemObj *mo);
  int    GUI_obj_free (MemObj *gObj);

  void*  GUI_obj_parent__ (int *pTyp, MemObj *mObj);
  void*  GUI_obj_parentBox (void *o_par);
  void   GUI_obj_typ (int *pTyp, void **g_par, MemObj *o_par);

  void   GUI_obj_dump_mo (MemObj *mo);

  char GUI_opts_get1 (char **s1, char *opts);


// EOF
