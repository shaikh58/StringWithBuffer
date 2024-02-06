//
//  Tracker.hpp
//  memtest
//
//  Created by rick gessner on 1/22/22.
//

#ifndef Tracker_h
#define Tracker_h

#include <iostream>
#include <vector>
#include <filesystem>
#include <algorithm>

namespace fs = std::filesystem;

#define _TRACKER_ON  // Enable tracker

#ifdef _TRACKER_ON
#define GPS(aPtr) Tracker::instance().watch(aPtr, __LINE__, __FILE__)
#else
#define GPS(aPtr) (aPtr)
#endif

struct Tracker {

    static Tracker single;

    static Tracker& instance() {
        return single;
    }

    struct Memo {
        void*   ptr;
        size_t  size;
        size_t  line;
        size_t  filenum;
    };

    Tracker(bool aEnabled=false) : enabled(aEnabled), totalAllocated(0), totalDeallocated(0) {
        names.push_back("unknown");
    }

    bool isEnabled() const { return enabled; }

    Tracker& enable(bool aState) {
        enabled = aState;
        return *this;
    }

    Tracker& reset() {
        names.clear();
        list.clear();
        totalAllocated = 0;
        totalDeallocated = 0;
        return *this;
    }

    void* track(void* aPtr, size_t aSize) {
        if(enabled) {
            enabled = false;
            list.push_back(Memo{aPtr, aSize,  0, 0});
            totalAllocated += aSize;
            enabled = true;
        }
        return aPtr;
    }

    template<typename T>
    T* watch(T* aPtr, size_t aLine=0, const char* aFile=nullptr) {
        if(aLine) {
            bool wasEnabled=enabled;
            enabled=false;
            std::string theName=fs::path(aFile).filename().u8string();
            auto theIt = find(names.begin(), names.end(), theName);
            size_t theIndex=names.size();
            if (theIt != names.end()) {
                theIndex = theIt - names.begin();
            }
            else names.push_back(theName);
            enabled=wasEnabled;

            auto theEnd=list.rend();
            for (auto it = list.rbegin(); it!=theEnd; it++) {
                if(it->ptr==aPtr) {
                    it->line=aLine;
                    it->filenum=theIndex;
                    break;
                }
            }
        }
        return aPtr;
    }

    Tracker& untrack(void* aPtr) {
        auto theIter = std::find_if(
                list.begin(), list.end(), [aPtr](Memo const& aMemo){
                    return aMemo.ptr == aPtr;
                });
        if(theIter != list.end()) {list.erase(theIter);}
        return *this;
    }

    Tracker& reportLeaks(std::ostream &aStream) {
        for(auto &theMem: list) {
            aStream << theMem.ptr << " : "
                    << names[theMem.filenum] << "("
                    << theMem.line << ") Size: " << theMem.size << "\n";
        }
        return *this;
    }

    size_t getTotalAllocated() const { return totalAllocated; }
    size_t getTotalDeallocated() const { return totalDeallocated; }

protected:
    bool                      enabled;
    size_t                    totalAllocated;
    size_t                    totalDeallocated;
    std::vector<Memo>         list;
    std::vector<std::string>  names;
};

Tracker Tracker::single(false);


//-------------------------------------------

#ifdef _TRACKER_ON
void * operator new(size_t aSize) {
    auto thePtr=std::malloc(aSize);
    Tracker::instance().track(thePtr, aSize);
    return thePtr;
}

void * operator new[](size_t aSize) {
    return operator new(aSize);
}

void operator delete(void* aPtr) noexcept {
    Tracker::instance().untrack(aPtr);
    std::free(aPtr);
}

void operator delete[](void* aPtr) noexcept {
    operator delete(aPtr);
}

void operator delete(void* aPtr, std::size_t) noexcept {
    Tracker::instance().untrack(aPtr);
    std::free(aPtr);
}

void operator delete[](void* aPtr, std::size_t) noexcept {
    operator delete(aPtr);
}

#endif

#endif /* Tracker_h */