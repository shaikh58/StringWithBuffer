//
//  main.cpp
//  Assignment2
//
//

#include <iostream>
#include <sstream>
#include <string>
#include "autotest.hpp"
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

//-----------------------------------

int main(int argc, const char * argv[]) {
    static const char* kMsgs[]={"FAIL","PASS"};
    if(argc>1) {
        std::string temp(argv[1]);
        std::stringstream theOutput;
        BufferManagerAutoTests bufferManagerTest;
        StringAutoTests stringTest;

        srand(static_cast<uint32_t>(time(NULL)));

        if("compile"==temp) {
            std::cout << temp << " test " << kMsgs[true] << "\n";
        }
        else if("BufferOCF" == temp) {
            auto result = bufferManagerTest("OCFTest",theOutput);
            std::cout<< temp << " test " << kMsgs[result] << "\n" << theOutput.str() <<"\n";
            return !result;
        }
        else if("OCF"==temp) {
            auto result = stringTest("OCFTest", theOutput);
            std::cout<< temp << " test " << kMsgs[result] << "\n" << theOutput.str() <<"\n";
            return !result;
        }
        else if("Expand"==temp) {
            auto result = bufferManagerTest("ExpandTest",theOutput);
            std::cout<< temp << " test " << kMsgs[result] << "\n" << theOutput.str() <<"\n";
            return !result;
        }
        else if("Compact"==temp) {
            auto result = bufferManagerTest("CompactTest",theOutput);
            std::cout<< temp << " test " << kMsgs[result] << "\n" << theOutput.str() <<"\n";
            return !result;
        }
        else if("Insert"==temp) {
            auto result = stringTest("InsertTest",theOutput);
            std::cout<< temp << " test " << kMsgs[result] << "\n" << theOutput.str() <<"\n";
            return !result;
        }
        else if("Append"==temp) {
            auto result = stringTest("AppendTest",theOutput);
            std::cout<< temp << " test " << kMsgs[result] << "\n" << theOutput.str() <<"\n";
            return !result;
        }
        else if("Replace"==temp) {
            auto result = stringTest("ReplaceTest",theOutput);
            std::cout<< temp << " test " << kMsgs[result] << "\n" << theOutput.str() <<"\n";
            return !result;
        }
        else if("Erase"==temp) {
            auto result = stringTest("EraseTest",theOutput);
            std::cout<< temp << " test " << kMsgs[result] << "\n" << theOutput.str() <<"\n";
            return !result;
        }
        else if("Search"==temp) {
            auto result = stringTest("SearchTest",theOutput);
            std::cout<< temp << " test " << kMsgs[result] << "\n" << theOutput.str() <<"\n";
            return !result;
        }
        else if("Compare"==temp) {
            auto result = stringTest("CompareTest",theOutput);
            std::cout<< temp << " test " << kMsgs[result] << "\n" << theOutput.str() <<"\n";
            return !result;
        }
        else if("Speed"==temp) {
            auto result = stringTest("SpeedTest",theOutput);
            std::cout<< temp << " test " << kMsgs[result] << "\n" << theOutput.str() <<"\n";
            return !result;
        }
        else if("All"==temp) {
            return stringTest.runTests() && bufferManagerTest.runTests();
        }
        else{
            std::cout<<"Unknown test " << temp << "\n";
            return 1;
        }
    }
    return 0;
}