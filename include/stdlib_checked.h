//---------------------------------------------------------------------//
// Bounds-safe interfaces for functions in stdlib.h that               //
// take pointer arguments.                                             //
//                                                                     //
// These are listed in the same order that they occur in the C11       //
// specification.                                                      //
//                                                                     //
// TODO: revise string types after support for pointers to             //
// null-terminated arrays is added to C.                               //
/////////////////////////////////////////////////////////////////////////
#include <stdlib.h>

// TODO: strings
// double atof(const char *s);
// int atoi(const char *s);
// long int atol(const char *s);
// long long int atoll(const char *s);

double strtod(const char * restrict nptr,
              char ** restrict endptr : itype(restrict _Nullable _Ptr<char *>));
float strtof(const char * restrict nptr,
             char ** restrict endptr : itype(restrict _Nullable _Ptr<char *>));
long double strtold(const char * restrict nptr,
                    char ** restrict endptr : itype(restrict _Nullable _Ptr<char *>));

long int strtol(const char * restrict nptr,
                char ** restrict endptr : itype(restrict _Nullable _Ptr<char *>),
                int base);
long long int strtoll(const char * restrict nptr,
                      char ** restrict endptr : itype(restrict _Nullable _Ptr<char *>),
                      int base);
unsigned long int strtoul(const char * restrict nptr,
                          char ** restrict endptr :
                            itype(restrict _Nullable _Ptr<char *>),
                          int base);

unsigned long long int strtoull(const char * restrict nptr,
                                char ** restrict endptr:
                                   itype(restrict _Nullable _Ptr<char *>),
                                int base);

// TODO: express alignment constraints once where clauses have been added.
void *aligned_alloc(size_t alignment, size_t size) : _Nullable byte_count(size);
void *calloc(size_t nmemb, size_t size) : _Nullable byte_count(nmemb * size);
void free(void *ptr : itype(_Nullable _Ptr<void>));
void *malloc(size_t size) : _Nullable byte_count(size);
void *realloc(void *ptr  : itype(_Nullable _Ptr<void>), size_t size) : _Nullable byte_count(size);

// TODO: strings
// char *getenv(const char *n);

int atexit(void ((*func)(void)) : itype(_Ptr<void (void)>));
int atquick_exit(void ((*func)(void)) : itype(_Ptr<void (void)>));

// TODO: strings
// int system(const char *s);

// TODO: compar needs to have an itype that has bounds
// on parameters based on size.  Currently we are requiring that
// bounds in parameters lists be closed with respect to variables
// in the parameter list.
void *bsearch(const void *key : byte_count(size),
              const void *base : byte_count(nmemb * size),
              size_t nmemb, size_t size,
              int ((*compar)(const void *, const void *)) :
                itype(_Ptr<int(_Ptr<const void>, _Ptr<const void>)>)) :
                _Nullable byte_count(size);

// TODO: compar needs to have an itype that has bounds
// on parameters based on size.  Currently we are requiring that
// types be closed.
void qsort(void *base : byte_count(nmemb * size),
           size_t nmemb, size_t size,
           int ((*compar)(const void *, const void *)) :
             itype(_Ptr<int (_Ptr<const void>, _Ptr<const void>)>));

int mblen(const char *s : _Nullable count(n), size_t n);

int mbtowc(wchar_t * restrict output : itype(restrict _Nullable _Ptr<wchar_t>),
           const char * restrict input : _Nullable count(n),
           size_t n);

// MB_CUR_MAX is a macro that becomes a function call, so is banned
// in bounds annotations. 
// 
// int wctomb(char *s : count(MB_CUR_MAX), wchar_t wc);

size_t mbstowcs(wchar_t * restrict pwcs : _Nullable count(n),
                const char * restrict s,
                size_t n);

size_t wcstombs(char * restrict output : _Nullable count(n),
                const wchar_t * restrict pwcs,
                size_t n);
