#include "config.h"
#include <R.h>
#include <Rinternals.h>
#include <R_ext/Rdynload.h>


#ifdef HAVE_CXX1X
	#define HASHED_NAME(fun) fun ## Hash
	extern SEXP dupAtomMatHash(SEXP, SEXP, SEXP);
	extern SEXP anyDupAtomMatHash(SEXP, SEXP, SEXP);
	extern SEXP grpDupAtomMatHash(SEXP, SEXP, SEXP);
	extern int initHash(void);
#else
	#define HASHED_NAME(fun) fun
	extern SEXP dupAtomMat(SEXP, SEXP, SEXP);
	extern SEXP anyDupAtomMat(SEXP, SEXP, SEXP);
	extern SEXP grpDupAtomMat(SEXP, SEXP, SEXP);
#endif
extern SEXP dbl_dig();

static R_CallMethodDef callMethods[]  = {
  {"dupAtomMat", (DL_FUNC) &HASHED_NAME(dupAtomMat), 3},
  {"anyDupAtomMat", (DL_FUNC) &HASHED_NAME(anyDupAtomMat), 3},
  {"grpDupAtomMat", (DL_FUNC) &HASHED_NAME(grpDupAtomMat), 3},
  {"dbl_dig", (DL_FUNC) &dbl_dig, 0},
  {NULL, NULL, 0}
};
	

void R_init_uniqueAtomMat(DllInfo *info)
{
#ifdef HAVE_CXX1X
   if(!initHash())error("Hashing initialization error");
#endif   
   R_registerRoutines(info, NULL, callMethods, NULL, NULL);
}
