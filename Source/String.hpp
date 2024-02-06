//
//  String.hpp
//  Assignment3
//
//  Created by rick gessner on 1/29/20.
//  Copyright © 2020 rick gessner. All rights reserved.
//

#ifndef String_hpp
#define String_hpp

#include <iostream>
#include "BufferManager.hpp"
#include <cstring>

namespace ECE141 {

    template<typename T=char, size_t aPresize=32>
    class String {
    public:
        String(const char* aBuffer=nullptr) {
            if(aBuffer == nullptr){}
            else{ this->operator=(aBuffer); }
        }
        String(const String &aString){
            this->operator=(aString);
        }

        ~String() = default;
        String& operator=(const String &aCopy){
            buf.operator=(aCopy.buf);
            return *this;
        }
        String& operator=(const char* aChars){
            buf = ECE141::BufferManager<T>(std::strlen(aChars) + 1);
            buf.moveData(buf.getBuffer(), aChars);
            return *this;
        }

        T *getBuffer() const{return buf.getBuffer();}
        size_t size() const {return buf.getStrSize();}

        //mutation methods...
        T& operator[](size_t pos) const{
            return *(buf.getBuffer() + pos);
        }

        String  operator+(const String &aString){
            ECE141::String s_tmp(*this); // use the copy ctor to initialize tmp string with current string
            s_tmp.operator+=(aString); // simply call += to do in-place append with the new tmp string
            return s_tmp;
        }
        String  operator+(const char* aChars){
            ECE141::String s_tmp(*this);
            s_tmp.operator+=(aChars); // will call the char* operator+=
            return s_tmp;
        }

        String& operator+=(const String &aString){
            size_t otherStrSize = aString.buf.getStrSize() + 1;
            size_t str_size = buf.getStrSize() + 1; // get this str size
            // don't double count the null terminators
            buf.willExpand(str_size + otherStrSize);
//            std::cout << str_size << " " << aString.getBuffer() << " " << aString.buf.getStrSize() << " " << buf.getCapacity() << std::endl;
            buf.moveData(buf.getBuffer() + str_size - 1, aString.getBuffer(), aString.buf.getStrSize());
            return *this;
        }
        String& operator+=(const char* aChars){
            String s_tmp(aChars);
            this->operator+=(s_tmp);
            return *this;
        }

        String& insert(size_t anIndex, const String &aStr, size_t aStrIndex,
                       size_t aStrCount){
            size_t newSize = buf.getStrSize() + aStrCount; // insert only aStrCount portion of aStr
            buf.willExpand(newSize);
            // shift data along the new buffer to make space for the substring
            buf.moveData(buf.getBuffer() + anIndex + aStrCount, buf.getBuffer() + anIndex,
                         buf.getStrSize() - anIndex);
            // now insert the substring into the newly made space in the buffer
            buf.moveData(buf.getBuffer() + anIndex, aStr.buf.getBuffer() + aStrIndex, aStrCount);
            //todo: error handling if aStrCount goes beyond the end of aStr!
            return *this;
        }

        String& insert(size_t anIndex, const char* aStr, size_t aStrIndex, size_t aStrCount){
            String s_tmp(aStr);
            return this->insert(anIndex, s_tmp, aStrIndex, aStrCount);
        }

        String& insert(size_t anIndex, T aChar){
            char* tmp_char = &aChar;
            this->insert(anIndex, tmp_char, 0, sizeof(aChar));
            return *this;
        }

        String& replace(size_t anIndex, size_t aMaxCopyLen, const String &aString){
            this->erase(anIndex, aMaxCopyLen);
            this->insert(anIndex, aString, 0, aString.buf.getStrSize());
            return *this;
        }

        String& replace(size_t anIndex, size_t aMaxCopyLen, const char* aChars){
            String s_tmp(aChars);
            this->replace(anIndex, aMaxCopyLen, s_tmp);
            return *this;
        }

        String& erase(size_t anIndex, size_t aCount){
            size_t orig_size = buf.getStrSize();
            size_t modified_count = aCount;
            if(anIndex + aCount >= orig_size){
                modified_count = orig_size - anIndex;
            }
            buf.moveData(buf.getBuffer() + anIndex, buf.getBuffer() + anIndex + modified_count,
                         orig_size - anIndex - modified_count);
            buf.willCompact(orig_size - modified_count);
            return *this;
        }

        //Comparison methods...
        int compare( const String& aString ) const{
            return std::strcmp(this->getBuffer(), aString.getBuffer());
        }
        int compare( const char* aChars ) const{
            return std::strcmp(this->getBuffer(), aChars);
        }

        bool operator==(const String &aString) const {
            int are_equal = this->compare(aString);
            if(are_equal == 0){return true;}
            else{ return false; }
        }
        bool operator==(const char* aChars) const {
            int are_equal = this->compare(aChars);
            if(are_equal == 0){return true;}
            else{ return false; }
        }
        bool operator!=(const String &aString) const {return !(*this == aString);}
        bool operator!=(const char* aChars) const {return !(*this == aChars);}
        bool operator<(const String &aString) const {
            int comp_val = this->compare(aString);
            if(comp_val < 0){return true;}
            else{return false;}
        }
        bool operator<(const char* aChars) const {
            int comp_val = this->compare(aChars);
            if(comp_val < 0){return true;}
            else{return false;}
        }
        bool operator>(const String &aString) const {
            int comp_val = this->compare(aString);
            if(comp_val > 0){return true;}
            else{return false;}
        }
        bool operator>(const char* aChars) const {
            int comp_val = this->compare(aChars);
            if(comp_val > 0){return true;}
            else{return false;}
        }
        bool operator<=(const String &aString) const {return !(this->operator>(aString));}
        bool operator<=(const char* aChars) const {return !(this->operator>(aChars));}
        bool operator>=(const String &aString) const {return !(this->operator<(aString));}
        bool operator>=(const char* aChars) const {return !(this->operator<(aChars));}

        //Search...
        int find( const String &aString, size_t anIndex = 0 ){
            int match = 0;
            for(size_t i = anIndex; i<= buf.getStrSize() - aString.buf.getStrSize(); i++){
                for(size_t j=0; j<aString.buf.getStrSize(); j++){
                    if(this->operator[](i+j) == aString.operator[](j)){match = 0;}
                    else{
                        match = -1;
                        break;
                    }
                }
                if(match==0){return static_cast<int>(i);}
            }
            return -1;
        }

        int find( const char* aChars, size_t anIndex = 0 ){
            String s_tmp(aChars);
            return this->find(s_tmp, anIndex);
        }

        friend std::ostream& operator << (std::ostream &anOut, const String &aStr){
            anOut << aStr.getBuffer();
            return anOut;
        }
        friend std::istream& operator >> (std::istream &anOut,  String &aString){
            anOut >> aString.getBuffer();
            return anOut;
        }

    protected:
        BufferManager<char> buf;
    };

} // namespace ECE141

#endif /* String_hpp */
