// include with deactivated extern in "../xa/xa.c"
// includefile for memoryspace
// needs #include "../ut/ut_memTab.h"           // MemTab

extern char memspc011[1024];   // 1-K-Bloecke
extern char memspc012[1024];

extern char memspc251[25000];   // 25-K-Block-1

extern char memspc50[50000];   // 50-K-Bloecke
extern char memspc51[50000];
extern char memspc52[50000];
extern char memspc53[50000];
extern char memspc54[50000];
extern char memspc55[50000];

extern char memspc101[100000];
extern char memspc102[100000];
extern char memspc201[200000];
extern char memspc501[500000];


// maxSiz of single textline ..
#define mem_cbuf1_SIZ 200000

// zum scannen der Textzeilen
extern char mem_cbuf1[mem_cbuf1_SIZ];

// xa_cad_ui.c
extern char IE_buf[mem_cbuf1_SIZ];
extern char IE_outTxt[mem_cbuf1_SIZ];



// TODO: char memspcLocks[4] for memspc501 memspc201 memspc101 memspc102
// function MemSpcLock (ind, 1) = locked, 0 = free


//=================== EOF ==============================
