//  BufferManager.hpp
//  Assignment2

#ifndef BufferManager_h
#define BufferManager_h
#include <iostream>
#include <memory>
#include <cstring>

namespace ECE141   {

    template <typename T>
    class BufferManager {
    private:
        size_t size; // size of the buffer in bytes
        size_t str_size = 0; // size of the string in the buffer; str_size is always <= size
        std::unique_ptr<T[]> ptr; // internal pointer to the beginning of the string
    public:
        BufferManager() : size{0}, ptr{nullptr} {}
        BufferManager(const BufferManager &aCopy) { this->operator=(aCopy); }
        explicit BufferManager(size_t aSize) : size{aSize + 2}, ptr{std::make_unique<T[]>(aSize + 2)} {}
        // for chars, each element = 1 byte so size = # elements including null terminator
        ~BufferManager() = default;

        BufferManager &operator=(const BufferManager &otherBuf) {
            this->size = otherBuf.size;
            this->ptr = std::make_unique<T[]>(otherBuf.size);
            std::memcpy((this->ptr).get(), otherBuf.ptr.get(), otherBuf.str_size);
            this->str_size = otherBuf.str_size;
            return *this;
        }

        size_t getCapacity() const { return this->size; }
        size_t getStrSize() const { return this->str_size; }
        void updateStrSize() { this->str_size = std::strlen((this->ptr).get());}
        T *getBuffer() const { return (this->ptr).get(); }

        // overloaded moveData operators that also update string size
        void moveData(char* dst, char* src, size_t bytes){
            std::memcpy(dst,src,bytes);
            this->updateStrSize();
        }
        void moveData(char* dst, const char* src_str){
            std::strcpy(dst, src_str);
            this->updateStrSize();
        }
        /*
         * Checks if existing buffer is large enough to accommodate aNewSize bytes; if yes, does nothing,
         * if no, creates a new empty buffer of size aNewSize switches the internal buffer pointer
         *
         * Parameters:
         *      aNewSize - number of bytes that string method passes in (incl. null terminator)
         *      anOffset - unused (retained for testing)
         * Outputs: Returns size_t number of bytes of the new buffer
         */
        size_t willExpand(size_t aNewSize, size_t /*anOffset*/ = 0) {
            if(this->size >= aNewSize){}
            else{
                size_t modified_size = aNewSize + 5;
                std::unique_ptr<T[]> tmp = std::make_unique<T[]>(modified_size);
                std::memcpy(tmp.get(), ptr.get(), size);
                std::swap(ptr, tmp); // switch the internal pointer; ptr's old data is automatically deallocated
                this->size = modified_size;
            }
            return this->size;
        }
        /*
         * Called after erase operations are done by string - creates a smaller buffer and moves the smaller string
         * into the new smaller buffer. Assumes that str_size has been updated at the end of any string operation
         */
        size_t willCompact(size_t aNewSize, size_t anOffset = 0) {
            if(this->size <= aNewSize){
                this->size = this->willExpand(aNewSize, anOffset);
            }
            else{
                std::unique_ptr<T[]> tmp = std::make_unique<T[]>(aNewSize);
                std::memcpy(tmp.get(), (this->ptr).get(), aNewSize);
                ptr = std::move(tmp);
                this->size = aNewSize;
            }
            return this->size;
        }
    };
} // namespace ECE141



#endif /* BufferManager_h */