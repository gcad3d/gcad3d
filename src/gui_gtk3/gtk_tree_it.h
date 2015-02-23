// create Tree with icon + text.

// prereqisite:
// #include "../xa/xa_ico.h"              // ICO_PT,



#ifdef _LP64
  // Linux64
typedef struct {char iter[32];}        TreeNode;
  // 32=sizeof(GtkTreeIter)
#else
  // Linux32
typedef struct {char iter[16];}        TreeNode;
  // 16=sizeof(GtkTreeIter)
#endif




MemObj GUI_tree1__ (MemObj *o_par, void *selCB, char *opts);

  // int GUI_tree1_row_add (char *nodeChd,
  int GUI_tree1_row_add (TreeNode *nodeChd,
                         MemObj *mo,
                         TreeNode *nodePar,
                         int icoNr, char *txt, int mode);

  int GUI_tree1_row_set (MemObj *mo, TreeNode *row,
                          int ico, char *txt, int mode);

  int GUI_tree1_childs_set (MemObj *mo, TreeNode *it, int mode);
  int GUI_tree1_childs_remove (MemObj *mo, TreeNode *it);

  int GUI_tree1_iter_string (TreeNode *it,
                             char *txt, TreeNode *itPar, MemObj *mo);

  int GUI_tree1_expand__ (MemObj *mo, TreeNode *it);
  int GUI_tree1_unselect_all (MemObj *mo);

  int GUI_tree1_selRow  (MemObj *mo, TreeNode *it);
  int GUI_tree1_childNr (MemObj *mo, TreeNode *it);
  int GUI_tree1_lev     (MemObj *mo, TreeNode *it);
  int GUI_tree1_par     (TreeNode *itPar, MemObj *mo, TreeNode *itChild);
  int GUI_tree1_ndPos (char *ndPos, MemObj *mo, TreeNode *it);
  int GUI_tree1_cmp_row (MemObj *mo, TreeNode *it1, TreeNode *it2);

  int GUI_tree1_cbSel (void *selection, MemObj mo);
  int GUI_tree1_cbMouse (void *parent, void *ev, MemObj mo);

  int GUI_tree1_remove__ (MemObj *mo, TreeNode *it);
  int GUI_tree1_clear (MemObj *mo);

  int GUI_tree1_decode (MemObj *mo);


/// \code
/// Main & subModels:  Typ_SubModel ICO_natM;   natMdl.png
/// modelReferences:   Typ_Model    ICO_refM    part.png      
/// external Models:   Typ_Mock     ICO_extM;   extMdl.png
/// library-parts:                  ICO_libM    libPart.png
/// \endcode


// see Ico_icoNr_typ

// EOF
