# C++ 문자열 클래스(MyString) 구현
1. 이 클래스는 C 스타일 문자열(char*)을 감싼(wrap) 클래스로 C 스타일 문자열을 수정하는 다양한 유틸리티 메서드를 구현함
2. thread-safety 적용
3. 정적 라이브러리

## 설계 요소
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
