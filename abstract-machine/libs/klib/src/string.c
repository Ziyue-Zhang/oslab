#include "klib.h"

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
  assert(s);
  for(size_t i = 0; ; i++){
	  if(s[i] == '\0')
		  return i;
	  //printf("fuck!\n");
  }
}

char *strcpy(char* dst,const char* src) {
  assert(dst && src);
  for (int i = 0; ; i++) {
	 dst[i] = src[i];
	 if(src[i] == '\0')
		 return dst;
  }
}

char* strncpy(char* dst, const char* src, size_t n) {
  assert(dst && src);
  size_t i = 0;
  for (; i < n; i++)
	  dst[i] = src[i];
  dst[i] = '\0';
  return dst;
}

char* strcat(char* dst, const char* src) {
  int i = 0;
  while(dst[i] != '\0')
	  i++;
  int j = 0;
  while(src[j] != '\0'){
	  dst[i+j] = src[j];
	  j++;
  }
  dst[i+j] = '\0';
  return dst;
}

int strcmp(const char* s1, const char* s2) {
  assert(s1 && s2);
  while (*s1 == *s2 && *s1)
  {
      s1++;
      s2++;
	  //printf("fuck\n");
  } 
  int ret = *(unsigned char*)s1 - *(unsigned char*)s2;
  if(ret < 0)
    return -1;
  else if(ret > 0) 
    return 1;
  return 0;
}

int strncmp(const char* s1, const char* s2, size_t n) {
  assert(s1 && s2);
  while (n > 0 && *s1) {
	  if (*s1 != *s2) {
		  int ret = *(unsigned char*)s1 - *(unsigned char*)s2;
		  if(ret > 0)
			  return 1;
		  else
			  return -1;
	  }
	  s1++;
	  s2++;
	  n--;
  }
  return 0;
}

void* memset(void* v,int c,size_t n) {
  char* temp = v;
  while(n--) {
	 *temp = (char)c;
	 temp++;
  }
  return v;
}

void* memcpy(void* out, const void* in, size_t n) {
  char* temp1 = out;
  const char *temp2 = in;
  while (n--) {
    *temp1 = *temp2;
	temp1++;
	temp2++;
  }
  return out;
}

int memcmp(const void* s1, const void* s2, size_t n){
	assert(s1 && s2);
	while (n-- && s1) {	
		if (*(char*)s1 != *(char*)s2) {
		  int ret = *(unsigned char*)s1 - *(unsigned char*)s2;
		  if(ret > 0)
			  return 1;
		  else
			  return -1;
		}
		s1 = (char*)s1 + 1;
		s2 = (char*)s2 + 1;	
	}
	return 0;
}

void* memmove(void* dst, const void* src, size_t count){
  void* ret =dst;
  if(dst<=src||(char*)dst>=((char*)src+count)){
    while(count--){
      *(char*)dst=*(char*)src;
      dst=(char*)dst+1;
      src=(char*)src+1;
    }
  }
  else{
    dst=(char*)dst+count-1;
    src=(char*)src+count+-1;
    while(count--){
      *(char*)dst=*(char*)src;
      dst=(char*)dst-1;
      src=(char*)src-1;
    }
  }
  return ret;
}
#endif
