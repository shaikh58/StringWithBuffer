//
//  autotest.hpp
//  PA3
//
//  Created by rick gessner on 1/22/22.
//

#ifndef autotest_h
#define autotest_h
#include <map>
#include "Tracker.hpp"
#include "BufferManager.hpp"
#include "String.hpp"
#include <string>
#include <cstring>
#include "Timer.hpp"
#include "Testable.hpp"


bool assertTrue(const char* aMessage, bool aValue,
                std::ostream& anOutput = std::cout) {
    static const char* theMsgs[] = { "OK\n","FAIL\n" };
    anOutput << "\t" << aMessage << " " << theMsgs[!aValue];
    return aValue;
}

bool assertFalse(const char* aMessage, bool aValue,
                 std::ostream& anOutput = std::cout) {
    static const char* theMsgs[] = { "OK\n","FAIL\n" };
    anOutput << "\t" << aMessage << " " << theMsgs[aValue];
    return false == aValue;
}

std::string getWords(size_t aCount) {
    std::vector<std::string> kWords={
            "hello","world","goodbye","yellow","brick",
            "road","what","you","won't","do","there",
            "apricot","silly","songs","glass","keys",
            "apple","bananna","pear","grape","orange"};
    size_t theSize=kWords.size();
    const char *thePrefix="";
    std::string theResult;
    for(size_t i=0;i<aCount;i++) {
        theResult+=thePrefix;
        theResult+=kWords[rand() % theSize];
        thePrefix=" ";
    }
    return theResult;
}

class BufferManagerAutoTests : public ECE141::Testable {
    //If this won't compile, your Buffer Managerclass may not be ready.
    //If this crashes, check how your Buffer Managerwrites to a stream.

public:
    BufferManagerAutoTests(): ECE141::Testable(){
        count = vList.size();
    }
    using Callable = bool (BufferManagerAutoTests::*)(std::ostream &anOutput);
    std::map<std::string, Callable> vList{
            {"OCFTest",&BufferManagerAutoTests::doBufferManagerOCFTests},
            {"ExpandTest",&BufferManagerAutoTests::doBufferManagerExpandTests},
            {"CompactTest", &BufferManagerAutoTests::doBufferManagerCompactTests}

    };
    /*
     * The getTestName funciton used thorough the testable classes
     */
    OptString getTestName(size_t anIndex) const override{
        size_t thePos{0};
        for (auto const& thePair : vList){
            if(anIndex==thePos++){
                return thePair.first;
            }
        }
        return std::nullopt;
    };
    // calling operator overloaded
    bool operator()(const std::string &aName) override{

        return vList.count(aName) != 0 && (this->*vList[aName])(std::cout);
    }
    bool operator()(const std::string &aName, std::ostream &aStream) override{
        return vList.count(aName) != 0 && (this->*vList[aName])(aStream);
    }
private:
    bool doBufferManagerOCFTests(std::ostream &anOutput) {

        auto &theTracker=Tracker::instance();
        theTracker.enable(true).reset();

        {
            ECE141::BufferManager<char> theBuf1(100);
            ECE141::BufferManager<char> theBuf2(theBuf1);

            if(100>theBuf1.getCapacity()) {
                anOutput << "ctor copy failed\n";
                return false;
            }

            if(theBuf1.getCapacity()!=theBuf2.getCapacity()) {
                anOutput << "ctor copy failed\n";
                return false;
            }
        }

        theTracker.reportLeaks(anOutput);
        return true;
    }

    bool doBufferManagerExpandTests(std::ostream &anOutput) {

        auto& theTracker = Tracker::instance();
        theTracker.enable(true).reset();
        {
            ECE141::BufferManager<char> theBuf1;
            theBuf1.willExpand(100);
            if (100 > theBuf1.getCapacity()) {
                anOutput << "expand failed\n";
                return false;
            }
            theBuf1.willExpand(200);
            if (200 > theBuf1.getCapacity()) {
                anOutput << "expand failed\n";
                return false;
            }

        }
        theTracker.reportLeaks(anOutput);
        return true;
    }

    bool doBufferManagerCompactTests(std::ostream &anOutput) {
        auto& theTracker = Tracker::instance();
        theTracker.enable(true).reset();
        {
            ECE141::BufferManager<char> theBuf1(100);
            if(100>theBuf1.getCapacity()) {
                anOutput << "expand failed\n";
                return false;
            }
            theBuf1.willCompact(50);
            if(50>theBuf1.getCapacity()) {
                anOutput << "expand failed\n";
                return false;
            }
        }
        theTracker.reportLeaks(anOutput);
        return true;
    }
};


class StringAutoTests : public ECE141::Testable {
    //If this won't compile, your Buffer Managerclass may not be ready.
    //If this crashes, check how your Buffer Managerwrites to a stream.
public:
    StringAutoTests() : ECE141::Testable(){
        count = vList.size();
    }
    using Callable = bool (StringAutoTests::*)(std::ostream &anOutput);
    std::map<std::string, Callable> vList{
            {"OCFTest",&StringAutoTests::doOCFTests},
            {"InsertTest",&StringAutoTests::doInsertTests},
            {"AppendTest",&StringAutoTests::doAppendTests},
            {"EraseTest",&StringAutoTests::doEraseTests},
            {"ReplaceTest",&StringAutoTests::doReplaceTests},
            {"SearchTest",&StringAutoTests::doSearchTests},
            {"CompareTest",&StringAutoTests::doCompareTests},
            {"SpeedTest",&StringAutoTests::doSpeedTest}
    };
    OptString getTestName(size_t anIndex) const override{
        size_t thePos{0};
        for (auto const& thePair : vList){
            if(anIndex==thePos++){
                return thePair.first;
            }
        }
        return std::nullopt;
    };
    bool operator()(const std::string &aName) override{
        return vList.count(aName) != 0 && (this->*vList[aName])(std::cout);
    }
    bool operator()(const std::string &aName, std::ostream &aStream) override{
        return vList.count(aName) != 0 && (this->*vList[aName])(aStream);
    }
private:
    bool doOCFTests(std::ostream &anOutput) {

        auto &theTracker=Tracker::instance();
        theTracker.enable(true).reset();

        {
            ECE141::BufferManager<char> theBuf1(100);
            ECE141::BufferManager<char> theBuf2(theBuf1);

            auto theTestStr1=getWords(1);
            ECE141::String theECEString1(theTestStr1.c_str());
            if (theTestStr1!=theECEString1.getBuffer()) {
                anOutput << "ctor failed\n";
                return false;
            }

            //string copy-ctor test...
            ECE141::String theECEString2(theECEString1);
            if (theTestStr1!=theECEString2.getBuffer()) {
                anOutput << "copy ctor() failed\n";
                return false;
            }

            auto theTestStr2=getWords(2);
            theECEString1 = theTestStr2.c_str();
            if (theTestStr2!=theECEString1.getBuffer()) {
                anOutput << "operator=char* failed\n";
                return false;
            }

            theECEString2 = theECEString1;
            auto theBuffer=theECEString1.getBuffer();
            auto theBuffer2=theECEString2.getBuffer();
            if (std::strcmp(theBuffer,theBuffer2)) {
                anOutput << "operator=string failed\n";
                return false;
            }

            theTestStr2=getWords(2);
            std::string theSTDString1(theTestStr2.c_str());
            ECE141::String theECEString5(theTestStr2.c_str());
            if (theECEString5[4]!=theSTDString1[4]) {
                anOutput << "operator[] failed\n";
                return false;
            }

            theTestStr2=getWords(1);
            ECE141::String theECEString6(theTestStr2.c_str());
            std::string theSTDString6(theTestStr2);

            ECE141::String theECEString7;
            std::string theSTDString7;

            theTestStr2=getWords(1);
            theSTDString7=theSTDString6+theTestStr2.c_str();
            theECEString7=theECEString6+theTestStr2.c_str();
            auto theBuf=theECEString7.getBuffer();
            if(theSTDString7!=theECEString7.getBuffer()) {
                anOutput << "operator+(const String& lhs, const char* rhs) failed\n";
                return false;
            }

            theTestStr2=getWords(1);
            ECE141::String theECEString8(theTestStr2.c_str());
            std::string theSTDString8(theTestStr2.c_str());
            theECEString7=theECEString6+theECEString8;
            theSTDString7=theSTDString6+theSTDString8;
            theBuf=theECEString7.getBuffer();
            if (strcmp(theSTDString7.c_str(),theBuf)) {
                anOutput << "operator+(const String& lhs, const String& rhs) failed\n";
                return false;
            }
        }

        theTracker.reportLeaks(anOutput);
        return true;
    }

    bool doInsertTests(std::ostream &anOutput) {
        auto& theTracker = Tracker::instance();
        theTracker.enable(true).reset();
        {
            std::string     theSTDString1(getWords(2));
            ECE141::String  theECEString1(theSTDString1.c_str());

            std::string     temp(getWords(3));
            ECE141::String  theECEString2(temp.c_str());

            theECEString1.insert(5, theECEString2, 0, temp.size());
            theSTDString1.insert(5, temp, 0, temp.size());
            if (theSTDString1!=theECEString1.getBuffer()) {
                anOutput << "insert(size_t anIndex, const String &aStr, size_t aStrIndex, size_t aStrCount) failed\n";
                return false;
            }

            temp=getWords(2);
            theSTDString1.insert(8, temp.c_str(), 3, 6);
            theECEString1.insert(8, temp.c_str(), 3, 6);
            if (theSTDString1.compare(theECEString1.getBuffer()) != 0) {
                anOutput << "insert(size_t anIndex, const char* aBuffer, size_t aStrIndex, size_t aStrCount) failed\n";
                return false;
            }
            theSTDString1.insert(10, 1, '?');
            theECEString1.insert(10, '?'); //WE DONT PASS COUNT!
            if (theSTDString1.compare(theECEString1.getBuffer()) != 0) {
                anOutput << "insert(size_t anIndex, char ch) failed\n";
                return false;
            }
            for (int i = 0; i < 100; ++i) {
                theECEString1.insert(i%theECEString1.size(), temp.c_str(), 0, temp.size());
                theSTDString1.insert(i%theSTDString1.length(), temp.c_str(), 0, temp.size());
            }
            if (theSTDString1.compare(theECEString1.getBuffer()) != 0) {
                anOutput << "insert(size_t anIndex, const char* aBuffer, size_t aStrIndex, size_t aStrCount) failed 2\n";
                return false;
            }
        }
        theTracker.reportLeaks(anOutput);
        return true;
    }

    bool doAppendTests(std::ostream &anOutput) {
        auto& theTracker = Tracker::instance();
        theTracker.enable(true).reset();
        {
            std::string theSTDString1("hello world");
            ECE141::String theECEString1("hello world");
            std::string theSTDString2("hello there world");
            ECE141::String theECEString2("hello there world");
            theSTDString1 += theSTDString2;
            theECEString1 += theECEString2;
            if (theSTDString1!=theECEString1.getBuffer()) {
                anOutput << "operator+=(const String &aString) \failed\n";
                return false;
            }
            theSTDString1 += "testing";
            theECEString1 += "testing";
            if (theSTDString1!=theECEString1.getBuffer()) {
                anOutput << "operator+=(const char *aBuffer) \failed\n";
                return false;
            }
        }
        theTracker.reportLeaks(anOutput);
        return true;
    }

    bool doEraseTests(std::ostream &anOutput) {
        auto& theTracker = Tracker::instance();
        theTracker.enable(true).reset();
        {
            auto theWords=getWords(3);
            std::string    theSTDString1(theWords.c_str());
            ECE141::String theECEString1(theWords.c_str());

            theSTDString1.erase(3, 5);
            theECEString1.erase(3, 5);
            auto theBuffer=theECEString1.getBuffer();
            if (std::strcmp(theBuffer,theSTDString1.c_str())) {
                anOutput << "erase(size_t aPos, size_t aLength) failed\n";
                return false;
            }

            auto theWords2 = getWords(5);
            std::string    theSTDString2(theWords2.c_str());
            ECE141::String theECEString2(theWords2.c_str());

            theSTDString2.erase(0, 1);
            theECEString2.erase(0, 1);
            auto theBuffer2 = theECEString2.getBuffer();
            if (std::strcmp(theBuffer2, theSTDString2.c_str())) {
                anOutput << "erase(size_t aPos, size_t aLength) failed\n";
                return false;
            }

            auto theWords3 = getWords(8);
            std::string    theSTDString3(theWords3.c_str());
            ECE141::String theECEString3(theWords3.c_str());

            theSTDString3.erase(theSTDString3.length()-2, theSTDString3.length()-1);
            theECEString3.erase(theECEString3.size()-2, theECEString3.size()-1);
            auto theBuffer3 = theECEString3.getBuffer();
            if (std::strcmp(theBuffer3, theSTDString3.c_str())) {
                anOutput << "erase(size_t aPos, size_t aLength) failed\n";
                return false;
            }

            theWords3 = getWords(5);
            std::string    theSTDString4(theWords3.c_str());
            ECE141::String theECEString4(theWords3.c_str());

            theSTDString4.erase(12, 100);
            theECEString4.erase(12, 100); //truncate
            auto theBuffer4=theECEString4.getBuffer();
            if (std::strcmp(theBuffer4,theSTDString4.c_str())) {
                anOutput << "erase(12,100) (truncate) failed\n";
                return false;
            }
        }
        theTracker.reportLeaks(anOutput);

        return true;
    }

    bool doReplaceTests(std::ostream &anOutput) {

        auto& theTracker = Tracker::instance();
        theTracker.enable(true).reset();
        {
            auto temp=getWords(3);
            std::string theSTDString1(temp.c_str());
            ECE141::String theECEString1(temp.c_str());

            std::string theRep=getWords(2);
            std::string theSTDRep1(theRep.c_str());
            ECE141::String theECERep1(theRep.c_str());

            theSTDString1.replace(2,2, theSTDRep1);
            theECEString1.replace(2,2, theECERep1);
            if (!(theECEString1.getBuffer() == theSTDString1)) {
                anOutput << "replace(size_t pos, size_t len, const String& aString)\n";
                return false;
            }

            //replace all...
            theRep=getWords(2);
            theSTDString1=theRep.c_str();
            theECEString1=theRep.c_str();

            theRep=getWords(3);
            theSTDString1.replace(0, theSTDString1.size(), theSTDRep1);
            theECEString1.replace(0, theECEString1.size(), theECERep1);
            if (!(theECEString1.getBuffer()==theSTDString1)) {
                anOutput << "replace(0, size(), string)\n";
                return false;
            }

            auto theWords=getWords(2);
            std::string STDString1(theWords);
            ECE141::String ECEString5(theWords.c_str());

            theWords=getWords(3);
            std::string STDString2(theWords);
            ECE141::String ECEString6(theWords.c_str());
            STDString1.replace(5, 8, STDString2);
            ECEString5.replace(5, 8, ECEString6);
            if (!(ECEString5.getBuffer() == STDString1)) {
                anOutput << "replace(size_t pos, size_t len, const String& aString)\n";
                return false;
            }
            STDString1.replace(3, 2, STDString2);
            ECEString5.replace(3, 2, ECEString6);
            if (!(ECEString5.getBuffer() == STDString1)) {
                anOutput << "replace(size_t pos, size_t len, const String& aString)\n";
                return false;
            }
            STDString1.replace(5, 6, STDString2);
            ECEString5.replace(5, 6, ECEString6);
            if (!(ECEString5.getBuffer() == STDString1)) {
                anOutput << "replace(size_t pos, size_t len, const String& aString)\n";
                return false;
            }

            STDString1.replace(7, 8, STDString2);
            ECEString5.replace(7, 8, ECEString6);
            if (!(ECEString5.getBuffer() == STDString1)) {
                anOutput << "replace(size_t pos, size_t len, const String& aString)\n";
                return false;
            }

            theWords=getWords(1);
            STDString1.replace(3, 9, theWords.c_str());
            ECEString5.replace(3, 9, theWords.c_str());
            if (!(ECEString5.getBuffer() == STDString1)) {
                anOutput << "replace(size_t pos, size_t len, const String& aString)\n";
                return false;
            }

            theWords=getWords(1);
            STDString1.replace(5, 4, theWords.c_str());
            ECEString5.replace(5, 4, theWords.c_str());
            if (!(ECEString5.getBuffer() == STDString1)) {
                anOutput << "replace(size_t pos, size_t len, const String& aString)\n";
                return false;
            }

            theWords=getWords(1);
            STDString1.replace(0, 0, theWords.c_str());
            ECEString5.replace(0, 0, theWords.c_str());
            if (!(ECEString5.getBuffer() == STDString1)) {
                anOutput << "replace(size_t pos, size_t len, const String& aString)\n";
                return false;
            }
        }
        theTracker.reportLeaks(anOutput);

        return true;
    }

    bool doSearchTests(std::ostream &anOutput) {

        std::string STDString1("hello there world");
        ECE141::String ECEString1("hello there world");
        if (!(ECEString1.find("e", 5)==(int)STDString1.find("e", 5))) {
            anOutput << "find(const char *aBuffer, size_t anOffset=0)\n";
            return false;
        }
        if (!(ECEString1.find("ere")==(int)STDString1.find("ere"))) {
            anOutput << "find(const char *aBuffer, size_t anOffset=0)\n";
            return false;
        }
        std::string STDString2("world");
        ECE141::String ECEString2("world");
        ECEString1.find(ECEString2);
        if(!(ECEString1.find(ECEString2)==(int)STDString1.find(STDString2))) {
            anOutput << "3 find(const T &aTarget, size_t anOffset=0)\n";
            return false;
        }
        std::string STDString3("hello world");
        ECE141::String ECEString3("hello world");
        if (!(ECEString3[4]==STDString3[4])) {
            anOutput << "operator[]\n";
            return false;
        }
        return true;
    }

    bool doCompareTests(std::ostream &anOutput) {
        auto theWords=getWords(2);
        const char* theBuf=theWords.c_str();
        ECE141::String theString1(theBuf);
        std::string theSTLString1(theBuf);

        //first -- test operator== (comparison)...
        if (!(theSTLString1==theBuf && theString1==theBuf)) {
            anOutput << "operator==(const char *aBuffer) fail\n";
            return false;
        }

        ECE141::String theString2(theBuf);
        std::string theSTLString2(theBuf);
        if (!(theString1==theString2 && theSTLString1==theSTLString2)) {
            anOutput << "operator==(const T &aString)\n";
            return false;
        }

        if (theString1!=theBuf && theSTLString1!=theBuf) {
            anOutput << "operator!=(const char *aBuffer) fail\n";
            return false;
        }

        ECE141::String theString3(theBuf);
        std::string theSTLString3(theBuf);
        if (theString1!=theString3 && theSTLString1!=theSTLString3) {
            anOutput << "operator!=(const T &aString) fail\n";
            return false;
        }

        const char* temp="zoologists rock";
        theWords=getWords(2);
        theBuf=theWords.c_str();
        ECE141::String theString4(theBuf);
        std::string theSTLString4(theBuf);
        if (!(theSTLString4<temp && theString4<temp)) {
            anOutput << "operator<(const T* aBuffer) fail\n";
            return false;
        }

        theSTLString1=temp;
        theString1=temp;
        if (!(theString4<theString1 && theSTLString4<theSTLString1)) {
            anOutput << "operator<(const T* aBuffer) fail\n";
            return false;
        }

        if (!(theSTLString4<=temp && theString4<=temp)) {
            anOutput << "operator<=(const char *aBuffer)\n";
            return false;
        }

        if (!(theString4<=theString1 && theSTLString4<=theSTLString1)) {
            anOutput << "operator<=(const T &aString)\n";
            return false;
        }

        if (!(theString4<=theString4 && theSTLString4<=theSTLString4)) {
            anOutput << "operator<=(const T &aString)\n";
            return false;
        }

        std::string str1("able and ready");
        theWords=getWords(2);
        theBuf=theWords.c_str();
        std::string theSTLString5(theBuf);
        ECE141::String theString5(theBuf);
        if (!(theString5>str1.c_str() && theSTLString5>str1.c_str())) {
            anOutput << "operator>(const char *aBuffer)\n";
            return false;
        }

        theString1=str1.c_str();
        if (!(theSTLString5>str1 && theString5>theString1)) {
            anOutput << "operator>(const T &aString)\n";
            return false;
        }

        if (!(theSTLString5>=theSTLString5 && theString5>=theString5)) {
            anOutput << "operator>=(const T &aString)\n";
            return false;
        }

        if (!(theString5>=theBuf && theSTLString5>=theBuf)) {
            anOutput << "operator>=(const char *aBuffer)\n";
            return false;
        }
        return true;
    }

    bool doSpeedTest(std::ostream& anOutput) {
        std::vector<int> stringSizes = {10, 100, 1000, 10000};
        bool testPassed = true;

        Tracker::instance().enable(true);

        for (auto size : stringSizes) {
            std::string stdString(size, 'x');
            ECE141::String ece141String(stdString.c_str());

            // Speed and Memory test for ECE141::String
            Tracker::instance().reset();
            ECE141::Timer ece141Timer;
            for (int i = 0; i < 10000; i++) {
                ECE141::String temp = ece141String;
                temp += "suffix";
            }
            double ece141Time = ece141Timer.elapsed();
            size_t ece141Memory = Tracker::instance().getTotalAllocated();

            // Speed and Memory test for std::string
            Tracker::instance().reset();
            ECE141::Timer stdTimer;
            for (int i = 0; i < 10000; i++) {
                std::string temp = stdString;
                temp += "suffix";
            }
            double stdTime = stdTimer.elapsed();
            size_t stdMemory = Tracker::instance().getTotalAllocated();

            anOutput << "String Size: " << size << "\n";
            anOutput << "ECE141::String - Time: " << ece141Time << "s, Memory: " << ece141Memory << " bytes\n";
            anOutput << "std::string - Time: " << stdTime << "s, Memory: " << stdMemory << " bytes\n";

            // Fail if ECE141::String is more than twice as slow or uses more than 3x as much memory
            if (ece141Time > stdTime * 2 || ece141Memory > stdMemory * 3) {
                anOutput << "ECE141::String failed the test for size " << size << "\n";
                testPassed = false;
            }
        }

        return testPassed;
    }


};


#endif /* autotest_h */