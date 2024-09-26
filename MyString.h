#ifndef MY_STRING_H
#define MY_STRING_H

#include <mutex>

class MyString
{
public:
	MyString(const char* s);
	MyString(const MyString& other);
	~MyString();

	unsigned int GetLength() const;
	const char* const GetCString() const;
	void Append(const char* s);
	const int IndexOf(const char* s) const;
	const int LastIndexOf(const char* s) const;
	void Interleave(const char* s);
	const bool RemoveAt(const unsigned int i);
	void PadLeft(const unsigned int totalLength);
	void PadLeft(const unsigned int totalLength, const char c);
	void PadRight(const unsigned int totalLength);
	void PadRight(const unsigned int totalLength, const char c);
	void Reverse();
	void ToLower();
	void ToUpper();

	MyString& operator=(const MyString& rhs);
	const bool operator==(const MyString& rhs) const;
	MyString operator+(const MyString& other) const;

private:
	static int strlen(const char* str);
	static void strcpy(char* dest, const char* src);

	void reserve(unsigned int capacity);

	char* mCString;
	unsigned int mLength;
	unsigned int mCapacity;

    mutable std::mutex mMutex;
};

#endif // MY_STRING_H
