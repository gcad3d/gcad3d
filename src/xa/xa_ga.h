


// test if surface has texture; 0=no, 1=yes.
// iAtt = GA_ObjTab[gaNr].iatt;
// if(GA_hasTexture(iAtt) == 1)  .. yes
// ((ColRGB*)&GA_ObjTab[gaNr].iatt)->vtex
int GA_hasTexture (int iAtt);
#define  GA_hasTexture(iAtt)\
  ((ColRGB*)&iAtt)->vtex


// get TextureReference-Index
// i1 = GA_getTexRefInd(iAtt);
// i1 = ((stru_c3c1*)&GA_ObjTab[gaNr].iatt)->b123;
int GA_getTexRefInd (int iAtt);
#define GA_getTexRefInd(iAtt)\
  ((stru_c3c1*)&iAtt)->b123
// see also GA_tex_ga2tr


// parent-type and child-type are equal !
// cInd    childIndex  (dbi)
// pInd    parentIndex (dbi)
// typ     groupCode !
typedef struct {long cInd, pInd; int typ;}                    Parent;



// EOF
