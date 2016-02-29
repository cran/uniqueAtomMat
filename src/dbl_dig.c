#include <R.h>
#include <Rinternals.h>
#include <float.h>   /* DBL_DIG */

SEXP dbl_dig()
{
    SEXP out = PROTECT(allocVector(INTSXP, 1));
    INTEGER(out)[0] = DBL_DIG;
    UNPROTECT(1);
    return out;
}
