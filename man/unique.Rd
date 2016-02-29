\name{unique.matrix}
\alias{unique.matrix}
\alias{duplicated.matrix}
\alias{anyDuplicated.matrix}

\title{Finding Unique or Duplicated Rows or Columns for Atomic Matrices}
\description{
These S3 methods are alternative (typically much faster) implementations of counterparts in the \code{base} package for atomic matrices.  

  \code{unique.matrix} returns a matrix with duplicated rows (or columns) removed. 
  
  \code{duplicated.matrix} returns a logical vector indicating which rows (or columns) are duplicated. 

  \code{anyDuplicated.matrix} returns an integer indicating the index of the first duplicate row (or column) if any, and \code{0L} otherwise.
}

\usage{
\method{unique}{matrix}(x, incomparables = FALSE, MARGIN = 1,
       fromLast = FALSE, signif=Inf, \dots)
\method{duplicated}{matrix}(x, incomparables = FALSE, MARGIN = 1,
           fromLast = FALSE, signif=Inf,\dots)
\method{anyDuplicated}{matrix}(x, incomparables = FALSE,
           MARGIN = 1, fromLast = FALSE, signif=Inf,\dots)
}


\arguments{
  \item{x}{an atomic matrix of mode \code{"numeric"}, \code{"integer"}, \code{"logical"}, \code{"complex"}, \code{"character"} or \code{"raw"}. When \code{x} is not atomic or when it is not a matrix, the \code{\link[base:unique]{base::unique.matrix}} in the \code{base} package will be called. }
  
  \item{incomparables}{a vector of values that cannot be compared, as in \code{\link[base:unique]{base::unique.matrix}}. Only when \code{incomparables=FALSE} will the code in \code{uniqueAtomMat} package be used; otherwise, the \code{base} version will be called. }
  
  \item{fromLast}{a logical scalar indicating if duplication should be considered
    from the last, as in \code{\link[base:unique]{base::unique.matrix}}. }
	
  \item{\dots}{arguments for particular methods.}
  
  \item{MARGIN}{a numeric scalar, the matrix margin to be held fixed, as in \code{\link{apply}}. For \code{unique.matrix}, only \code{MARGIN=1} and \code{MARGIN=2} are allowed; for \code{duplicated.matrix} and \code{anyDuplicated.matrix}, \code{MARGIN=0} is also allowed. For all other cases, the implementation in the \code{base} package will be called.}
  
  \item{signif}{a numerical scalar only applicable to numeric or complex \code{x}. If \code{signif=NULL}, then \code{x} will first be passed to \code{\link{signif}} function with the number of significant digits being the \code{C} constant \code{DBL_DIG}, as explained in \code{\link{as.character}}. If \code{signif=Inf} (which is the default value), then \code{x} is untouched before finding duplicates. If \code{signif} is any other number, it  specifies the required number of significant digits for \code{\link{signif}} function. }  
  
}

\details{
These S3 methods are alternative implementations of counterparts in the \code{base} package for atomic matrices (i.e., double, integer, logical, character, complex and raw) directly based on C++98 Standard Template Library (STL) \code{std::set}, or C++11 STL \code{std::unordered_set}. The implementation treats the whole row (or column) \emph{vector} as the key, without the intermediate steps of converting the mode to \code{character} nor collapsing them into a \emph{scalar} as done in \code{base}. On systems with empty \code{`R CMD config CXX1X`}, the C++98 STL \code{std::set} is used, which is typically implemented as a self-balancing tree (usually a red-black tree) that takes \eqn{O[n\log{(n)}]}{O[n log(n)]} to find all duplicates, where \code{n=dim(x)[MARGIN]}. On systems with non-empty \code{`R CMD config CXX1X`}, the C++11 STL \code{std::unordered_set} is used, with average \eqn{O(n)}{O(n)} performance and worst case \eqn{O(n^2)}{O(n^2)} performance.

  Missing values are regarded as equal, but \code{NaN} is not equal to
  \code{NA_real_}. 
  
 Further, in contrast to the \code{base} counterparts, characters are compared directly based on their internal representations; i.e., no encoding issues for characters. Complex values are compared by their real and imaginary parts separately. 
}

\value{
  \code{unique.matrix} returns a matrix with duplicated rows (if \code{MARGIN=1}) or columns (if \code{MARGIN=2}) removed. 
  
  \code{duplicated.matrix} returns a logical vector indicating which rows (if \code{MARGIN=1}) or columns (if \code{MARGIN=2}) are duplicated. 

  \code{anyDuplicated.matrix} returns an integer indicating the index of the first (if \code{fromLast=FALSE}) or last (if \code{fromLast=TRUE})  duplicate  row (if \code{MARGIN=1}) or column (if \code{MARGIN=2}) if any, and \code{0L} otherwise.

}
\section{Warning}{
In contrast to the \code{base} counterparts, 
characters are compared directly based on their internal representations without considering encoding issues; for numeric and complex matrices, the default \code{signif} is \code{Inf}, i.e. comparing floating point values directly without rounding; and \link{long vectors} are not supported yet. 
}


\seealso{
  \code{\link[base:duplicated]{base::duplicated}}, \code{\link[base:unique]{base::unique}}, \code{\link{signif}}, \code{\link{grpDuplicated}}
}

\examples{
## prepare test data: 
set.seed(9992722L, kind="Mersenne-Twister")
x.double=model.matrix(~gl(5,8))[sample(40), ]

## typical uses
unique(x.double)
unique(x.double, fromLast=TRUE)
unique(t(x.double), MARGIN=2)
unique(t(x.double), MARGIN=2, fromLast=TRUE)
anyDuplicated(x.double)
anyDuplicated(x.double, fromLast = TRUE)


## additional atomic test data
x.integer=as.integer(x.double); attributes(x.integer)=attributes(x.double)
x.factor=as.factor(x.integer); dim(x.factor)=dim(x.integer); dimnames(x.factor)=dimnames(x.integer)
x.logical=as.logical(x.double); attributes(x.logical)=attributes(x.double)
x.character=as.character(x.double); attributes(x.character)=attributes(x.double)
x.complex=as.complex(x.double); attributes(x.complex)=attributes(x.double)
x.raw=as.raw(x.double); attributes(x.raw)=attributes(x.double)

## compare results with base:
stopifnot(identical(base::duplicated.matrix(x.double), 
                    uniqueAtomMat::duplicated.matrix(x.double)
))
stopifnot(identical(base::duplicated.matrix(x.integer, fromLast=TRUE), 
                    uniqueAtomMat::duplicated.matrix(x.integer, fromLast=TRUE)
))
stopifnot(identical(base::duplicated.matrix(t(x.logical), MARGIN=2L), 
                    uniqueAtomMat::duplicated.matrix(t(x.logical), MARGIN=2L) 
))
stopifnot(identical(base::duplicated.matrix(t(x.character), MARGIN=2L, fromLast=TRUE), 
                    uniqueAtomMat::duplicated.matrix(t(x.character), MARGIN=2L, fromLast=TRUE) 
))

stopifnot(identical(base::unique.matrix(x.complex), 
                    uniqueAtomMat::unique.matrix(x.complex) 
))
stopifnot(identical(base::unique.matrix(x.raw), 
                    uniqueAtomMat::unique.matrix(x.raw) 
))
stopifnot(identical(base::unique.matrix(x.factor), 
                    uniqueAtomMat::unique.matrix(x.factor) 
))
stopifnot(identical(base::duplicated.matrix(x.double, MARGIN=0), 
                    uniqueAtomMat::duplicated.matrix(x.double, MARGIN=0) 
))
stopifnot(identical(base::anyDuplicated.matrix(x.integer, MARGIN=0), 
                    uniqueAtomMat::anyDuplicated.matrix(x.integer, MARGIN=0) 
))


## benchmarking
if (require(microbenchmark)){
    print(microbenchmark(base::duplicated.matrix(x.double)))
    print(microbenchmark(uniqueAtomMat::duplicated.matrix(x.double)))

    print(microbenchmark(base::duplicated.matrix(x.character)))
    print(microbenchmark(uniqueAtomMat::duplicated.matrix(x.character)))
}else{
    print(system.time(replicate(5e3L, base::duplicated.matrix(x.double))))
    print(system.time(replicate(5e3L, uniqueAtomMat::duplicated.matrix(x.double))))

    print(system.time(replicate(5e3L, base::duplicated.matrix(x.character))))
    print(system.time(replicate(5e3L, uniqueAtomMat::duplicated.matrix(x.character))))
}


}
\keyword{manip}
\keyword{logic}
