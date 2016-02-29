#include <unordered_set>
#include <functional>

#include "rcVec.h"

/* Needs global randbit, lshift, rshift defined */

namespace std {
	
template <typename T>
struct hash <rcVec<T> > {
public: 
	size_t operator() (const rcVec<T> & x) const
	{	size_t ans = 0;
		int i ;
		for(i = x.len - 1; i >=0; i--)
			ans ^= ((hash<T>()(*(x.x + x.eltShift * i) )) ^ randbit.randbit) + (ans<<lshift) + (ans>>rshift) ;
		return ans;
	}
};

template <>
struct hash <CharSEXP > {
public: 
	size_t operator() (const CharSEXP & x) const
	{	
		return std::hash<char *>()(const_cast<char *>(CHAR(x.sexp)) );
	}
};


template <>
struct hash <Rcomplex > {
public: 
	size_t operator() (const Rcomplex & x) const
	{	
		size_t tmp;
		tmp = std::hash<double>()(x.r);
		return tmp ^ ( ( std::hash<double>()(x.i)  ^ randbit.randbit) + (tmp<<lshift) + (tmp>>rshift) ); 
	}
};


}

template <typename T>
class vecSetHash {  // a set with key being rcVec type
private:
    rcVec<T> aRC; 
    typedef std::unordered_set<rcVec<T> > rcvSetType;
    std::pair<typename rcvSetType::iterator,bool> retPair; // not used
    rcvSetType rcvSet; // using operator< of rcVec<T>

public:
    void duplicatedMat    	(const T* x, const int* nrow, const int* ncol, int* const out, bool const byRow=true, bool const fromLast=false);
    void anyDuplicatedMat   (const T* x, const int* nrow, const int* ncol, int* const out, bool const byRow=true, bool const fromLast=false);
    void grpDuplicatedMat   (const T* x, const int* nrow, const int* ncol, int* const out, bool const byRow=true, bool const fromLast=false);
};

template <typename T>
void vecSetHash<T>::duplicatedMat (const T* x, const int* nrow, const int* ncol, int* const out, bool const byRow, bool const fromLast)
{
    /* put a logical vector of duplicated rows of numeric matrix x into out */
    if(byRow){
        aRC.eltShift = aRC.nVec = (int)(*nrow);
        aRC.vecShift = 1;
        aRC.len = (int)(*ncol);
    }else{
        aRC.eltShift = 1;
        aRC.vecShift = aRC.len = (int)(*nrow);
        aRC.nVec = (int)(*ncol);
    }
    // set insert: if not previously inserted, the .second of returned pair is true; otherwise false. the .first is an iterator for the (previously) inserted element, which is not used. 
    if (fromLast) {
        aRC.x=const_cast<T*>(x) + ( byRow ? (*nrow)-1 : ((*ncol)-1)*(*nrow) ); 
        for(int i=aRC.nVec-1; i>=0; aRC.x -= aRC.vecShift)
            out[i--] = (int) !(rcvSet.insert( aRC ).second);
    }else {
        aRC.x=const_cast<T*>(x);
        for(int i=0; i<aRC.nVec; aRC.x += aRC.vecShift) 
            out[i++] = (int) !(rcvSet.insert( aRC ).second);
    }
    rcvSet.clear();
}


template <typename T>
void vecSetHash<T>::anyDuplicatedMat (const T* x, const int* nrow, const int* ncol, int* const out, bool const byRow, bool const fromLast)
{
    /* put a logical vector of duplicated rows of numeric matrix x into out */
    if(byRow){
        aRC.eltShift = aRC.nVec = (int)(*nrow);
        aRC.vecShift = 1;
        aRC.len = (int)(*ncol);
    }else{
        aRC.eltShift = 1;
        aRC.vecShift = aRC.len = (int)(*nrow);
        aRC.nVec = (int)(*ncol);
    }
    
    out[0] = 0; // result when no duplicates are found
    // set insert: if not previously inserted, the .second of returned pair is true; otherwise false. the .first is an iterator for the (previously) inserted element, which is not used. 
    if (fromLast) {
        aRC.x=const_cast<T*>(x) + ( byRow ? (*nrow)-1 : ((*ncol)-1)*(*nrow) ); 
        for(int i=aRC.nVec-1; i>=0; aRC.x -= aRC.vecShift, --i)
            if( !(rcvSet.insert( aRC ).second) ) {
                out[0] = i + 1;
                break;
            }
    }else {
        aRC.x=const_cast<T*>(x);
        for(int i=0; i<aRC.nVec; aRC.x += aRC.vecShift, ++i) 
            if( !(rcvSet.insert( aRC ).second) ){
                out[0] = i + 1;
                break;
            }
    }
    rcvSet.clear();
}

#include <unordered_map>

template <typename T>
class vecMapHash {  // a set with key being rcVec type
private:
    rcVec<T> aRC; 
    typedef std::unordered_map<rcVec<T>, int  > rcvMapType;
    std::pair<typename rcvMapType::iterator,bool> retPair; 
    rcvMapType rcvMap; // using operator< of rcVec<T>

public:
    int grpDuplicatedMat   (const T* x, const int* nrow, const int* ncol, int* const out, bool const byRow=true, bool const fromLast=false);
};

template <typename T>
int vecMapHash<T>::grpDuplicatedMat (const T* x, const int* nrow, const int* ncol, int* const out, bool const byRow, bool const fromLast)
{
    /* put a logical vector of duplicated rows of numeric matrix x into out */
    if(byRow){
        aRC.eltShift = aRC.nVec = (int)(*nrow);
        aRC.vecShift = 1;
        aRC.len = (int)(*ncol);
    }else{
        aRC.eltShift = 1;
        aRC.vecShift = aRC.len = (int)(*nrow);
        aRC.nVec = (int)(*ncol);
    }
    int grpId = 1;
    // map insert: if not previously inserted, the .second of returned pair is true; otherwise false. the .first is an iterator for the (previously) inserted element. 
    if (fromLast) {
        aRC.x=const_cast<T*>(x) + ( byRow ? (*nrow)-1 : ((*ncol)-1)*(*nrow) ); 
        for(int i=aRC.nVec-1; i>=0; aRC.x -= aRC.vecShift){
            retPair = rcvMap.insert( std::pair<rcVec<T>, int> (aRC, grpId) ); 
            out[i--] =  retPair.second ? grpId++ : retPair.first->second; // + (int)std::distance(retPair.first , rcvSet.begin());
        }
            
    }else {
        aRC.x=const_cast<T*>(x);
        for(int i=0; i<aRC.nVec; aRC.x += aRC.vecShift) {
            retPair = rcvMap.insert( std::pair<rcVec<T>, int> (aRC, grpId)  ); 
            out[i++] = retPair.second ? grpId++ : retPair.first->second; //+  (int)std::distance(retPair.first , rcvMap.begin());
           // Rprintf("i=%d\tgrpId=%d\tdistance=%d\n", i, grpId, (int)( std::distance(retPair.first , rcvMap.begin())) );
        }
    }
    rcvMap.clear();
    return grpId - 1;
}
