#include <set>

#include "rcVec.h"

template <typename T>
class vecSet {  // a set with key being rcVec type
private:
    rcVec<T> aRC; 
    typedef std::set<rcVec<T> > rcvSetType;
    std::pair<typename rcvSetType::iterator,bool> retPair; // not used
    rcvSetType rcvSet; // using operator< of rcVec<T>

public:
    void duplicatedMat    	(const T* x, const int* nrow, const int* ncol, int* const out, bool const byRow=true, bool const fromLast=false);
    void anyDuplicatedMat   (const T* x, const int* nrow, const int* ncol, int* const out, bool const byRow=true, bool const fromLast=false);
    void grpDuplicatedMat   (const T* x, const int* nrow, const int* ncol, int* const out, bool const byRow=true, bool const fromLast=false);
};

template <typename T>
void vecSet<T>::duplicatedMat (const T* x, const int* nrow, const int* ncol, int* const out, bool const byRow, bool const fromLast)
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
void vecSet<T>::anyDuplicatedMat (const T* x, const int* nrow, const int* ncol, int* const out, bool const byRow, bool const fromLast)
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

#include <map>

template <typename T>
class vecMap {  // a set with key being rcVec type
private:
    rcVec<T> aRC; 
    typedef std::map<rcVec<T>, int > rcvMapType;
    std::pair<typename rcvMapType::iterator,bool> retPair; 
    rcvMapType rcvMap; // using operator< of rcVec<T>

public:
    int grpDuplicatedMat   (const T* x, const int* nrow, const int* ncol, int* const out, bool const byRow=true, bool const fromLast=false);
};

template <typename T>
int vecMap<T>::grpDuplicatedMat (const T* x, const int* nrow, const int* ncol, int* const out, bool const byRow, bool const fromLast)
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
