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

4. thread-safety <br>
문자열이 여러 스레드에서 동시에 읽히기만 한다면 별도의 동기화가 필요 없다. 하지만, 문자열을 수정하는 작업이 발생한다면, 동기화를 통해 데이터 일관성을 보장해야한다.
    - `mutex`와 `lock_guard` 적용
        - `mutex` 적용
            - 공유 자원인 문자열을 관리하기 위해, 상호배제(Mutual Exlclusion) 보장이 필요. (위 경우 여러 스레드가 동시에 개체의 상태를 수정하면, 데이터 불일치가 발생한다)
            - 하나의 스레드만 해당 자원에 접근 가능하도록 제어해야했음
            - 따라서, binary semaphore 개념인 `mutex`를 적용
        - `lock_guard` 적용
            - 명시적으로 lock과 unlock을 사용하는 경우는, 실수할 경우가 종종 생김
            - `lock_guard` 는 C++에서 자주 사용하는 RAII(Resource Acquisition Is Initialization) 패턴을 구현한 도구
                - 해제를 직접하지 않더라도, 해제되기 때문에 안전 신경쓰는 비용을 줄일 수 있음
                    - 예외가 발생하는 경우에도 자동으로 해제되도록 보장해준다
            - 따라서, `lock_guard`를 적용
    - 추가적인 개선 방안
        - 읽기-쓰기 분리
            - 성능 향상: 읽기-쓰기 패턴이 빈번히 발생하는 경우, 읽기 작업과 쓰기 작업을 분리하여 성능을 향상시킬 수 있다.
                - 동시성 제어: 읽기 작업은 여러 스레드가 동시에 수행할 수 있도록 허용하고, 쓰기 작업은 단일 스레드만 수행하여 데이터 일관성을 보장한다.
                - 읽기 작업 빈도: string과 같은 데이터 구조에서는 읽기 작업이 많을 것으로 "예상"한다. 따라서, 읽기-쓰기 분리를 통해 성능 이점을 얻을 가능성이 크다.
            - 구현 예상 시나리오:
                - `shared_mutex` 적용 
                    - https://en.cppreference.com/w/cpp/thread/shared_mutex (C++17)
                    - `shared_mutex`: 공유(shared), 배타(exclusive)적 두 가지 수준의 액세스를 지원하는 클래스. 공유 뮤텍스는 공유 데이터가 여러 스레드에 의해 동시에 안전하게 읽힐 수 있는 경우에 특히 유용하지만, 스레드는 다른 스레드가 동시에 읽거나 쓰지 않을 때에만 동일한 데이터를 쓸 수 있다
                    - `shared_mutex` 예제 코드
                    ```cpp
                    class ThreadSafeCounter
                    {
                    public:
                        ThreadSafeCounter() = default;
                     
                        // Multiple threads/readers can read the counter's value at the same time.
                        unsigned int get() const
                        {
                            std::shared_lock lock(mutex_);
                            return value_;
                        }
                     
                        // Only one thread/writer can increment/write the counter's value.
                        void increment()
                        {
                            std::unique_lock lock(mutex_);
                            ++value_;
                        }
                     
                        // Only one thread/writer can reset/write the counter's value.
                        void reset()
                        {
                            std::unique_lock lock(mutex_);
                            value_ = 0;
                        }
                     
                    private:
                        mutable std::shared_mutex mutex_;
                        unsigned int value_{};
                    };
                    ``` 
---
  
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

# 지원 메서드, 연산자 및 설명

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
