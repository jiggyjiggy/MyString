#include <cassert>
#include <cstring>

#include <iostream>

#include <thread>
#include <vector>

#include "MyString.h"

using namespace std;

void test() {
    /* GetLength() Ex */
    MyString s1("Hello");
	assert(s1.GetLength() == 5);


    /* GetCString() Ex */
    MyString s2("Hello");
	assert(strcmp(s2.GetCString(), "Hello") == 0);


    /* Append() Ex */
    MyString s3("Hello");
    s3.Append(" World");
	assert(strcmp(s3.GetCString(), "Hello World") == 0);

    s3.Append("\n\tWorld\n");
	assert(strcmp(s3.GetCString(), "Hello World\n\tWorld\n") == 0);


    /* operator+ Ex */
    MyString s41("Hello");
    MyString s42(" World");
    MyString s43 = s41 + s42;
    assert(strcmp(s43.GetCString(), "Hello World") == 0);


    /* IndexOf() Ex */
    MyString s5("Hello");
	assert(s5.IndexOf("ell") == 1);
    assert(s5.IndexOf("l") == 2);
    assert(s5.IndexOf("This") == -1);
    assert(s5.IndexOf("") == 0);
    assert(s5.IndexOf("lo") == 3);

    MyString s52("");
	assert(s52.IndexOf("") == 0);
    assert(s52.IndexOf("A") == -1);

    MyString s53("Hellello");
    MyString s54("Heloo");
    assert(s53.IndexOf("ell") == 1);
    assert(s54.IndexOf("ell") == -1);


    /* LastIndexOf() Ex */
    MyString s6("Hello");
    assert(s6.LastIndexOf("ell") == 1);
    assert(s6.LastIndexOf("l") == 3);
    assert(s6.LastIndexOf("This") == -1);
    assert(s6.LastIndexOf("") == 5);
    assert(s6.LastIndexOf("He") == 0);

    MyString s62("");
	assert(s62.LastIndexOf("") == 0);
    assert(s62.LastIndexOf("A") == -1);

    MyString s63("Hellello");
    MyString s64("Heloo");
	assert(s63.LastIndexOf("ell") == 4);
    assert(s64.LastIndexOf("ell") == -1);


    /* Interleave() Ex */
    MyString s7("Hello");
    s7.Interleave(" World");
    assert(strcmp(s7.GetCString(), "H eWlolrold") == 0);

    MyString s72("A");
    s72.Interleave("BCDEF");
    assert(strcmp(s72.GetCString(), "ABCDEF") == 0);

    MyString s73("bcdef");
    s73.Interleave("a");
    assert(strcmp(s73.GetCString(), "bacdef") == 0);


    /* RemoveAt() Ex */
    MyString s8("Hello");
	assert(s8.RemoveAt(0) == true);
	assert(strcmp(s8.GetCString(), "ello") == 0);
	
	assert(s8.RemoveAt(2) == true);
	assert(strcmp(s8.GetCString(), "elo") == 0);

   	assert(s8.RemoveAt(4) == false);
	assert(strcmp(s8.GetCString(), "elo") == 0);

    MyString s82("");
    assert(s82.RemoveAt(0) == false);


    /* PadLeft() Ex */
    MyString s9("Hello");
    s9.PadLeft(2);  // "Hello"
	assert(strcmp(s9.GetCString(), "Hello") == 0);

    s9.PadLeft(8);  // "   Hello"
	assert(strcmp(s9.GetCString(), "   Hello") == 0);


    MyString s92("World");
    s92.PadLeft(3, '-');  // "World"
	assert(strcmp(s92.GetCString(), "World") == 0);

    s92.PadLeft(7, '-');  // "--World"
	assert(strcmp(s92.GetCString(), "--World") == 0);


    /* PadRight() Ex */
    MyString s10("Hello");
    s10.PadRight(2);  // "Hello"
    assert(strcmp(s10.GetCString(), "Hello") == 0);

    s10.PadRight(8);  // "Hello   "
    assert(strcmp(s10.GetCString(), "Hello   ") == 0);

    MyString s102("World");
    s102.PadRight(3, '-');  // "World"
    assert(strcmp(s102.GetCString(), "World") == 0);

    s102.PadRight(7, '-');  // "World--"
    assert(strcmp(s102.GetCString(), "World--") == 0);


    /* Reverse() Ex */
    MyString s11("Hello");
    s11.Reverse();  // olleH
	assert(strcmp(s11.GetCString(), "olleH") == 0);

    MyString s112("Banana");
    s112.Reverse();   // ananaB
	assert(strcmp(s112.GetCString(), "ananaB") == 0);

    MyString s113("A");
    s113.Reverse();   // A
    assert(strcmp(s113.GetCString(), "A") == 0);

    MyString s114("");
    s114.Reverse();   // 
	assert(strcmp(s114.GetCString(), "") == 0);


    /* operator== Ex */
    MyString s121("Hello");
    MyString s122("Hello");
    MyString s123("World");
    MyString s124("Hello ");
	MyString s125("HEllo 123K");
	MyString s126("HEllo 123K");
	MyString s127("");
	MyString s128("");

	assert((s121 == s122) == true);
    assert((s121 == s123) == false);
    assert((s121 == s124) == false);

	assert((s125 == s126) == true);

	assert((s127 == s128) == true);


    /* ToLower() Ex */
    MyString s131("HEllo 123K");
    MyString s132("A To z");
    s131.ToLower();   // "hello 123k"
    s132.ToLower();   // "a to z"
	assert(strcmp(s131.GetCString(), "hello 123k") == 0);
	assert(strcmp(s132.GetCString(), "a to z") == 0);


    /* ToUpper() Ex */
    MyString s141("HEllo 123K");
    MyString s142("A To z");
    s141.ToUpper();   // "HELLO 123K"
    s142.ToUpper();   // "A TO Z"
	assert(strcmp(s141.GetCString(), "HELLO 123K") == 0);
	assert(strcmp(s142.GetCString(), "A TO Z") == 0);
}


void append(MyString& myString, const char* s) {
    myString.Append(s);
}

void threadSafeAppendTest()
{
	MyString myString("Initial");
    const char* toAppend = "ABC";

    std::vector<std::thread> threads;


	int threadCount = 10;
    for (int i = 0; i < threadCount; ++i) {
        threads.emplace_back(append, std::ref(myString), toAppend);
    }

    for (auto& thread : threads) {
        thread.join();
    }

    char expectedResult[256] = "Initial";
    for (int i = 0; i < threadCount; ++i) {
        strcat(expectedResult, toAppend);
    }

    assert(strcmp(myString.GetCString(), expectedResult) == 0);
}

void interleave(MyString& myString, const char* s) {
    myString.Interleave(s);
}

void threadSafeInterleaveTest()
{
    MyString myString("Initial");
    const char* toInterleave = "12345678";

    std::vector<std::thread> threads;

    int threadCount = 10;
    for (int i = 0; i < threadCount; ++i) {
        threads.emplace_back(interleave, std::ref(myString), toInterleave);
    }

    for (auto& thread : threads) {
        thread.join();
    }

    char expectedResult[512] = "Initial";
    char temp[512] = "";

    for (int i = 0; i < threadCount; ++i) {
        const char* current = expectedResult;
        const char* append = toInterleave;
        int lenCurrent = strlen(current);
        int lenAppend = strlen(append);

        int k = 0;
        for (int j = 0; j < lenCurrent || j < lenAppend; ++j) {
            if (j < lenCurrent) 
			{
				temp[k++] = current[j];
			}
            if (j < lenAppend) 
			{
				temp[k++] = append[j];
			}
        }
        temp[k] = '\0'; 

        strcpy(expectedResult, temp);
    }

    assert(strcmp(myString.GetCString(), expectedResult) == 0);
}

void removeAt(MyString& myString, int index) {
    myString.RemoveAt(index);
}

void threadSafeRemoveAtTest() {
    MyString myString("ABCDEFGHIJK");

    std::vector<std::thread> threads;

	int threadCount = 10;
    for (int i = 0; i < threadCount; ++i) {
		threads.emplace_back(removeAt, std::ref(myString), 0); // 0번 인덱스를, 10번 삭제
    }

    for (auto& thread : threads) {
        thread.join();
    }
	
	const char* expectedResult = "K";
    assert(strcmp(myString.GetCString(), expectedResult) == 0);
}

void reverseString(MyString& myString)
{
	myString.Reverse();
}

void threadSafeReverseTest()
{
	MyString myString("ABCDEFGHIJK");

    std::vector<std::thread> threads;

	int threadCount = 9; // 홀수번 뒤집어야 문자열이 뒤집힘
    for (int i = 0; i < threadCount; ++i) {
        threads.emplace_back(reverseString, std::ref(myString));
    }

    for (auto& thread : threads) {
        thread.join();
    }

	const char* expectedResult = "KJIHGFEDCBA";
    assert(strcmp(myString.GetCString(), expectedResult) == 0);
}

void toLower(MyString& myString) {
    myString.ToLower();
}

void toUpper(MyString& myString) {
    myString.ToUpper();
}

void threadSafeLowerUpperTest() {
    MyString myString("AbCdEfGhIjK");

    std::vector<std::thread> threads;
    
	int threadCount = 10;
    for (int i = 0; i < threadCount; ++i) {
        threads.emplace_back(toLower, std::ref(myString));
		threads.emplace_back(toUpper, std::ref(myString));
    }
    
    for (auto& thread : threads) {
        thread.join();
    }

    const char* expectedResult1 = "ABCDEFGHIJK";
    const char* expectedResult2 = "abcdefghijk";
    assert(strcmp(myString.GetCString(), expectedResult1) == 0 || strcmp(myString.GetCString(), expectedResult2) == 0); // 둘중 하나의 결과로 나와야함
}

void runThreadSafeTests() {
   	threadSafeAppendTest();
	threadSafeInterleaveTest();
	threadSafeRemoveAtTest();
	threadSafeReverseTest();
	threadSafeLowerUpperTest();
}

int main(void)
{
	// 기본 기능 테스트
	test();

	// thread-safety 테스트
	for (int i = 0; i < 100; ++i) {
    	runThreadSafeTests();
	}
	
	return 0;
}