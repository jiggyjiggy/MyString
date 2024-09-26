#include <cassert>

#include "MyString.h"


MyString::MyString(const char* s)
	: mCString(nullptr)
	, mLength(0)
{
	mLength = strlen(s);

	mCString = new char[mLength + 1];
	strcpy(mCString, s);
	mCString[mLength] = '\0';
}

MyString::MyString(const MyString& other)
	: mCString(nullptr)
	, mLength(other.mLength)
{
	mCString = new char[mLength + 1];
	strcpy(mCString, other.mCString);
	mCString[mLength] = '\0';
}

MyString::~MyString()
{
	delete[] mCString;
}

int MyString::strlen(const char* str) 
{
	int len = 0;
	while (*str++ != '\0')
	{
		++len;
	}

	return len;
}

unsigned int MyString::GetLength() const
{
	return mLength;
}

const char* const MyString::GetCString() const
{
	return mCString;
}

void MyString::Append(const char* s)
{
	if (*s == '\0') 
	{
		return;
	}

	int len = strlen(s);
	int newLength = mLength + len;

	{
		char* newCString = new char[newLength + 1];
		strcpy(newCString, mCString);
		strcpy(newCString + mLength, s);
		newCString[newLength] = '\0';
		
		delete[] mCString;
		mCString = newCString;
	}

	{
		mLength = newLength;
	}
}

const  int MyString::IndexOf(const char* s) const
{
	if (*s == '\0') 
	{
		return 0;
	}

	const char* copiedCString = mCString;
	while (*copiedCString != '\0') 
	{
		const char* tempCopiedCString = copiedCString;
		const char* copiedS = s;

		while (*tempCopiedCString != '\0' && *copiedS != '\0' && *tempCopiedCString == *copiedS) 
		{
			++tempCopiedCString;
			++copiedS;
		}
		if (*copiedS == '\0') 
		{
			return copiedCString - mCString;
		}

		++copiedCString;
	}

	return -1;
}

const int MyString::LastIndexOf(const char* s) const
{
	if (*s == '\0') 
	{
		return mLength;
	}

	int len = strlen(s);
	const char* copiedCString = mCString + mLength - 1;
	while (copiedCString >= mCString) 
	{
		const char* tempCopiedCString = copiedCString;
		const char* copiedS = s + len - 1;

		while (tempCopiedCString >= mCString && copiedS >= s && *tempCopiedCString == *copiedS) 
		{
			--tempCopiedCString;
			--copiedS;
		}
		if (copiedS < s) 
		{
			return tempCopiedCString - mCString + 1;
		}

		--copiedCString;
	}

	return -1;
}

void MyString::Interleave(const char* s)
{
	if (*s == '\0') 
	{
		return;
	}

	int len = strlen(s);
	int newLength = mLength + strlen(s);
	char* newCString = new char[newLength + 1];
	
	{
		mLength = newLength;
	}

	{
		const char* copiedCString = mCString;
		const char* copiedS = s;
		char* copiedNewCString = newCString;
		while (copiedNewCString <= newCString + newLength - 1) 
		{
			if (*copiedCString != '\0') 
			{
				*copiedNewCString++ = *copiedCString++;
			}
			if (*copiedS != '\0') 
			{
				*copiedNewCString++ = *copiedS++;
			}
		}
		*copiedNewCString = '\0';

		delete[] mCString;
		mCString = newCString;
	}
}

const bool MyString::RemoveAt(const unsigned int i)
{
	if (mLength == 0 || i > mLength - 1) 
	{
		return false;
	}

	{
		char* newCString = new char[mLength];
		char* copiedNewCString = newCString;
		const char* copiedCString = mCString;
		while (*copiedCString != '\0') 
		{
			if (copiedCString == mCString + i) 
			{
				++copiedCString;
				continue;
			}

			*copiedNewCString++ = *copiedCString++;
		}
		*copiedNewCString = '\0';

		delete[] mCString;
		mCString = newCString;
	}

	{
		mLength = mLength - 1;
	}

	return true;
}

void MyString::PadLeft(const unsigned int totalLength)
{
	PadLeft(totalLength, ' ');
}

void MyString::PadLeft(const unsigned int totalLength, const char c)
{
	if (totalLength <= mLength) 
	{
		return;
	}
	
	{
		char* newCString = new char[totalLength + 1];
		char* copiedNewCString = newCString;
		int paddingCount = totalLength - mLength;
		
		for (int index = 0; index < paddingCount; ++index)
		{
			*copiedNewCString++ = c;
		}

		strcpy(copiedNewCString, mCString);
		
		newCString[totalLength] = '\0';

		delete[] mCString;
		mCString = newCString;
	}

	{
		mLength = totalLength;
	}
}

void MyString::PadRight(const unsigned int totalLength)
{
	PadRight(totalLength, ' ');
}

void MyString::PadRight(const unsigned int totalLength, const char c)
{
	if (totalLength <= mLength) 
	{
		return;
	}

	{
		char* newCString = new char[totalLength + 1];
		
		char* copiedNewCString = newCString;

		strcpy(copiedNewCString, mCString);
		copiedNewCString += mLength;

		for (unsigned int index = mLength; index < totalLength; ++index) 
		{
			*copiedNewCString++ = c;
		}

		*copiedNewCString = '\0';
		
		delete[] mCString;
		mCString = newCString;
	}

	{
		mLength = totalLength;
	}
}

void MyString::strcpy(char* dest, const char* src) 
{
	while (*src != '\0') 
	{
		*dest++ = *src++;
	}
}

void MyString::Reverse()
{
	char* front = mCString;
	char* end = mCString + mLength - 1;

	while (front < end) 
	{
		if (*front != *end) 
		{
			char temp = *front;
			*front = *end;
			*end = temp;
		}

		++front;
		--end;
	}
}

void MyString::ToLower()
{
	char* copiedCString = mCString;
	while (*copiedCString != '\0') 
	{
		if ('A' <= *copiedCString && *copiedCString <= 'Z') 
		{
			*copiedCString ^= 0x20;
		}

		++copiedCString;
	}
}

void MyString::ToUpper()
{
	char* copiedCString = mCString;
	while (*copiedCString != '\0') 
	{
		if ('a' <= *copiedCString && *copiedCString <= 'z') 
		{
			*copiedCString ^= 0x20;
		}

		++copiedCString;
	}
}

MyString MyString::operator+(const MyString& other) const
{
	MyString str = MyString(mCString);
	str.Append(other.mCString);
	return str;
}

MyString& MyString::operator=(const MyString& rhs)
{
	if (this == &rhs)
	{
		return *this;
	}

	{
		mLength = rhs.mLength;
	}

	{
		delete[] mCString;

		mCString = new char[mLength + 1];
		strcpy(mCString, rhs.mCString);
		mCString[mLength] = '\0';
	}

	return *this;
}

const bool MyString::operator==(const MyString& rhs) const
{
	if (mLength != rhs.mLength) 
	{
		return false;
	}

	const char* copiedCString = mCString;
	const char* copiedRhsCString = rhs.mCString;
	for (unsigned int i = 0; i < mLength; ++i) 
	{
		if (*copiedCString++ != *copiedRhsCString++)
		{
			return false;
		}
	}
	return true;
}
