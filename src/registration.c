#include <R.h>
#include <Rinternals.h>
#include <R_ext/Rdynload.h>



extern SEXP dupAtomMat(SEXP, SEXP, SEXP);
extern SEXP anyDupAtomMat(SEXP, SEXP, SEXP);
extern SEXP grpDupAtomMat(SEXP, SEXP, SEXP);

static R_CallMethodDef callMethods[]  = {
  {"dupAtomMat", (DL_FUNC) &dupAtomMat, 3},
  {"anyDupAtomMat", (DL_FUNC) &anyDupAtomMat, 3},
  {"grpDupAtomMat", (DL_FUNC) &grpDupAtomMat, 3},
  {NULL, NULL, 0}
};

void R_init_uniqueAtomMat(DllInfo *info)
{
   R_registerRoutines(info, NULL, callMethods, NULL, NULL);
}
