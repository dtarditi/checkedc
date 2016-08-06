// Feature tests of parsing new Checked C bounds declarations for
// function return values
//
// The following lines are for the LLVM test harness:
//
// RUN: %clang_cc1 -verify -fcheckedc-extension %s

// Parsing of function declarations
extern array_ptr<void> alloc(unsigned size) : byte_count(size);
extern array_ptr<int> f2(array_ptr<int> arr : count(5)) : count(3 + 2);
extern array_ptr<int> f3(int len, array_ptr<int> arr : count(len)) 
                      : count(len);
extern array_ptr<int> f4(array_ptr<int> arr : byte_count(20))
                      : byte_count(20);
extern array_ptr<int> f5(array_ptr<int> arr : byte_count(5 * sizeof(int)))
                      : byte_count(5 * sizeof(int));
extern array_ptr<int> f6(array_ptr<int> arr : bounds(arr, arr + 5))
                      : bounds(arr, arr + 5);
extern array_ptr<int> f7(int start,
                         array_ptr<int> arr : bounds(arr - start, arr - start + 5))
                      : bounds(arr - start, arr - start + 5);
extern array_ptr<char> f8() : bounds(none);
// count, bounds, and none are contextual keywords.  They are treated as keyword
// only when they immediately follow a ':';
extern array_ptr<char> f9(int count) : count(count);
extern array_ptr<char> f10(int none) : count(none);
extern array_ptr<int> f11(int bounds, array_ptr<int> arr : count(bounds))
                      : bounds(arr, arr + bounds);

// Parsing function definitions.
extern array_ptr<int> f1(array_ptr<int> arr : count(5)) : count(5) {
  return arr;
}

extern array_ptr<int> f2(array_ptr<int> arr : count(5)) : count(3 + 2) {
  return arr;
}

extern array_ptr<int> f3(int len,
                         array_ptr<int> arr : count(len))
                      : count(len) {
  return arr;
}

extern array_ptr<int> f4(array_ptr<int> arr : byte_count(20))
                      : byte_count(20) {
  return arr;
}

extern array_ptr<int> f5(array_ptr<int> arr : byte_count(5 * sizeof(int)))
                      : byte_count(5 * sizeof(int)) {
  return arr;
}

extern array_ptr<int> f6(array_ptr<int> arr : bounds(arr, arr + 5))
                      : bounds(arr, arr + 5) {
  return arr;
}

extern array_ptr<int> f7(int start,
                         array_ptr<int> arr : bounds(arr - start, arr - start + 5))
                      : bounds(arr - start, arr - start + 5) {
   return arr;
}

extern array_ptr<char> f8() : bounds(none) {
  return 0;
}

// 'count' is a contextual keyword.  It is only a keyword when it
// immediately follows the ':' in a bounds declaration.
extern array_ptr<char> f9(int count) : count(count) {
  return 0;
}

// 'none' is a contextual keyword.  It is only a keyword when it
// immediately follows the ':' in a bounds declaration.
extern array_ptr<char> f10(int none) : count(none) {
  return 0;
}

// 'bounds' is a contextual keyword.  It is only a keyword when it
// immediately follows the ':' in a bounds declaration.
extern array_ptr<int> f11(int bounds, array_ptr<int> arr : count(bounds))
                      : bounds(arr, arr + bounds) {
  return arr;
}

//
// Parsing of more complicated function declarations with bounds declarations
//

// Functions that return array_ptrs to ptrs

extern array_ptr<ptr<int>> f12(array_ptr<ptr<int>> arr : count(5)) : count(5) {
  return arr;
}

extern array_ptr<ptr<int>> f13(array_ptr<ptr<int>> arr : count(5))
                           : byte_count(5 * sizeof(ptr<int>)) {
  return arr;
}

extern array_ptr<ptr<int>> f14(array_ptr<ptr<int>> arr : count(5))
                           : bounds(arr, arr + 5) {
  return arr;
}

// Functions that return array_ptrs to arrays

extern array_ptr<int[10]> f15(array_ptr<int[10]> arr : count(5)) : count(3) {
  return arr;
}

extern array_ptr<int[10]> f16(array_ptr<int[10]> arr : count(5))
                          : byte_count(3 * sizeof(int[10])) {
  return arr;
}

extern array_ptr<int[10]> f17(array_ptr<int[10]> arr : count(5))
                          : bounds(arr, arr + 3) {
  return arr;
}

// Functions that return unchecked pointers to arrays, but that declare bounds
// on the unchecked pointers.  These will be used for interoperation.

extern int (*f18(int (*arr)[10] : count(5)) : count(3))[10] {
  return arr;
}

// Functions that return pointers to functions.  Note that array_ptrs to
// function types are not allowed, but array_ptr to pointers to functions type
// are allowed.

// Function that returns a ptr to a function that takes in a length and returns
// an array_ptr of that length.
extern ptr<array_ptr<int>(int len) : count(len)> f19(int len) {
  return 0;
}

// Like the prior function, but returns an unchecked pointer instead. The
// unchecked pointer points to a function that takes in a length and returns an
// array_ptr of that length.
//
// Parsing C declarators with * and () (for function declarators) intermixed
// can be confusing. To read this declaration, start from the innermost
// declarator and work your way out.  f19 is a function that takes an integer
// argument and returns a pointer.  Function declarator parens take precedence
//  over *.  The pointer points to function that takes an integer argument and
// returns an array_ptr<int>.
extern array_ptr<int> (*f20(int arg))(int len) : count(len) {
  return 0;
}

// Function that returns an array pointer to ptrs to functions that take in a
// length and return array_ptr<int>s of that length.
extern array_ptr<ptr<array_ptr<int>(int len) : count(len)>> f21(int arg)
                                                            : count(arg) {
  return 0;
}

// Use unchecked pointers instead. This is a function that returns a pointer to
// a pointer to a function that take in a length and return array_ptr<int>s of
// that length.
extern array_ptr<int> (**f22(int arg) : count(arg))(int len) : count(len) {
  return 0;
}

// Re-use len in the function type returned by the function and as an argument.
extern array_ptr<ptr<array_ptr<int>(int len) : count(len)>> f23(int len)
                                                            : count(len) {
  return 0;
}

// Pointers to functions that have bounds declarations on return values
extern void f24() {
  // Checked pointer to a function that returns an array_ptr to 5 integers.
  ptr<array_ptr<int>() : count(5)> p1 = 0;
  // Checked pointer to a function that returns an array_ptr to n integers,
  // where n is n argument.
  ptr<array_ptr<int>(int n) : count(n)> p2 = 0;
  // Use 'byte_count; instead of 'count'
  ptr<array_ptr<int>() : byte_count(5 * sizeof(int))> q1 = 0;
  ptr<int(int arg) : byte_count(5 * sizeof(int))> q2 = 0;
  ptr<int(int n, int arg) : byte_count(n * sizeof(int))> q3 = 0;
  // Use 'bounds' instead of 'count'.
  ptr<array_ptr<int>(array_ptr<int> arg : count(5)) : bounds(arg, arg + 5)>
    r1 = 0;
  ptr<int(array_ptr<int> arg : count(n), int n) : bounds(arg, arg + n)> r2 = 0;
  // Unchecked pointers to functions.
  int(*s1)(array_ptr<int> : count(5)) = 0;
  int(*s2)(array_ptr<int> arg : count(5)) = 0;
  int(*s3)(int n, array_ptr<int> arg : count(n)) = 0;
  int(*s4)(array_ptr<int> arg : count(n), int n) = 0;
  // Use 'byte_count' instead of 'count'.
  int(*t1)(array_ptr<int> : byte_count(5 * sizeof(int))) = 0;
  int(*t2)(array_ptr<int> arg : count(5 * sizeof(int))) = 0;
  int(*t3)(int n, array_ptr<int> arg : count(n * sizeof(int))) = 0;
  int(*t4)(array_ptr<int> arg : count(n * sizeof(int)), int n) = 0;
}

typedef int func1(int len) : count(len);
typedef int func2(array_ptr<int> arr : count(len), int len)
            : bounds(arr, arr + len);
static func1 *func1_ptr1;
static func2 *func2_ptr1;
func1 *func1_ptr2;

extern void f25() : 6 + 6 { // expected-error {{expected bounds expression}}
}

extern array_ptr<char> f26() : count(len) { // expected-error {{use of undeclared identifier 'len'}}
  return 0;
}

// Misspell bounds to cause a parsing error.
extern array_ptr<int> f27(int len,int arr : count(len)) : boounds(arr, arr + len)) { // expected-error {{expected bounds expression}}
  return 0;
}

// Misspell count to cause a parsing error.
extern array_ptr<int> f28(int len) : coount(len) { // expected-error {{expected bounds expression}}
  return 5;  // expected-error {{incompatible result type}}
}

// Omit an argument to bounds to cause a parsing error
extern array_ptr<int> f29(int len, int arr : count(len)) : bounds(arr)) { // expected-error {{expected ','}}
  return 0;
}

// Omit both arguments to bounds to cause a parsing error
extern array_ptr<int> f30(int len, int arr : count(len)) : bounds()) { // expected-error {{expected expression}}
  return 0;
}

// Omit the argument to count to cause a parsing error.
extern array_ptr<int> f31(int len) : count() { // expected-error {{expected expression}}
  return 5;  // expected-error {{incompatible result type}}
}
