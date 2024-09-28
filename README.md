# C++ 문자열 클래스(MyString) 구현
- 해당 클래스는 C 스타일 문자열(char*)을 감싼(wrap) 클래스로, C 스타일 문자열을 수정하는 다양한 유틸리티 메서드를 구현함
- thread-safety 적용
- 정적 라이브러리로써 제공할 클래스

## 핵심 설계 사항
1. 시그니처들을 보강하여 안전하고 효율적인 시그니처를 고려한 구현

2. 언어에서 제공하는 모든 라이브러리 사용하지 않음
    - cstring 및 cstring을 사용하는 라이브러리
    - string 
    - STL 컨테이너들 (vector, map, unordered_map, set, queue, stack 등)
  
3. 정적 라이브러리로 제공되려면, 성능 고려 필수
    - 반복문 속, 포인터 및 후위증가연산자(*ptr++)로 연산 최적화 고려한 메서드 구현
    - 동적 할당은 속도가 느리기에 동적 할당 횟수를 최소화하기 위한 capacity, size 전략 적용 (동적 배열, vector-like 정책)

4. thread-safety
    - mutex와 lock_guard 적용
        - 
  
# 정적 라이브러리 
## 빌드
```shell
clang++ -c MyString.cpp -o MyString.o
ar rcs libMyString.a MyString.o
```

## 사용
빌드한 라이브러리 파일 (libMyString.a) 과 헤더파일(MyString.h)을, 사용할 프로젝트에 세팅

``` shell
clang++ main.cpp -L. -lMyString -o a.out
```

---

## 지원 메서드, 연산자 및 설명

### `MyString(const char* s);`
- 생성자
### `MyString(const MyString& other);`
- 복사 생성자
- 깊은 복사
### `~MyString();`
- 소멸자

### `unsigned int GetLength() const;`
- 문자열의 길이를 반환한다
### `const char* const GetCString() const;`
- 개체 내부에 저장된 C 스타일 문자열을 반환한다
### `void Append(const char* s);`
- 현재 문자열에 매개변수로 들어온 C 스타일 문자열을 덧붙인다
### `const int IndexOf(const char* s) const;`
- 현재 문자열 안에서 매개변수로 들어온 C 스타일 문자열을 찾아 그 위치를 반환한다
- 현재 문자열 안에 찾는 문자열이 여럿 있다면 첫 번째 위치만 반환한다
- 문자열을 찾지 못한 경우에는 -1을 반환한다
### `const int LastIndexOf(const char* s) const;`
- 현재 문자열에서 매개 변수로 들어온 C 스타일 문자열을 찾아 그 위치를 반환한다
- 현재 문자열 안에 찾는 문자열이 여럿 있다면 마지막 위치만 반환한다
- 찾지 못한 경우에는 -1을 반환한다
### `void Interleave(const char* s);`
- 두 문자열을 한 글자씩 번갈아 가며 결합한다
- MyString 개체의 첫 번째 글자부터 시작한다
### `const bool RemoveAt(const unsigned int i);`
- 현재 문자열에서 글자를 하나 제거한다
- 매개변수로 들어온 색인 i가 지울 글자의 위치이다
- 이 메서드는 글자를 제거하면 true를, 그러지 못했으면 false를 반환한다
### `void PadLeft(const unsigned int totalLength);`, `void PadLeft(const unsigned int totalLength, const char c);`
- 문자열의 길이가 totalLength와 같아지도록 문자열의 왼쪽에 글자를 반복해서 덧붙인다.
- 덧붙일 글자는 스페이스 문자 또는 매개변수로 전달된 문자이다.
### `void PadRight(const unsigned int totalLength);`, `void PadRight(const unsigned int totalLength, const char c);`
- 문자열의 길이가 totalLength와 같아지도록 문자열의 오른쪽에 글자를 반복해서 덧붙인다.
- 덧붙일 글자는 스페이스 문자 또는 매개변수로 전달된 문자이다.
### `void Reverse();`
추가적인 메모리를 할당하지 않은 채 문자열을 좌우로 뒤집는다.
### `void ToLower();`
- ASCII 만 고려한다
- 문자열 안에 있는 모든 대문자를 소문자로 바꾼다.
- 추가 메모리를 사용하지 않는다
### `void ToUpper();`
- ASCII 만 고려한다
- 문자열 안에 있는 모든 소문자를 대문자로 바꾼다 
- 추가 메모리를 사용하지 않는다

### `MyString& operator=(const MyString& rhs);`
- 대입 연산자
- 깊은 복사
### `const bool operator==(const MyString& rhs) const;`
- 두 MyString 개체 안에 저장된 문자열이 같은지 검사한다
### `MyString operator+(const MyString& other) const;`
- 현재 문자열과 매개변수로 들어온 C 스타일 문자열을 합친 뒤 새로운 문자열 개체를 반환한다
