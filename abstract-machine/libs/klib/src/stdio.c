#include "klib.h"
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

int printf(const char *fmt, ...) {
  assert(fmt);
  va_list va;
  int n = 0;
  va_start(va, fmt);
  while(*fmt != '\0')
  { 
	if (*fmt == '%') {
		fmt++;
		if (*fmt == 's') {
			char *s = va_arg(va, char*);
			while(*s != '\0')
			{
				_putc(*s);
				n++;
				s++;
			}
		}
		else if (*fmt == 'd') {
			int result = va_arg(va, int);
			char d[32];
			int temp = result;
			int i;
			for(i = 0; temp != 0; i++)
			{
				d[i] = temp % 10 + '0';
				temp /= 10;
			}
			i--;
			if(result == 0)
			{
				i++;
				d[i]='0';
			}
			while(i >= 0)
			{
				_putc(d[i]);
				i--;
				n++;
			}
		}
		else if (*fmt == 'x') {
			int result = va_arg(va, int);
			char d[32];
			int temp = result;
			int i;
			for(i = 0; temp != 0; i++)
			{
				d[i] = temp % 16 + '0';
				temp /= 16;
			}
			i--;
			while(i >= 0)
			{
				_putc(d[i]);
				i--;
				n++;
			}
		}
		else if(*fmt == '0') {
			fmt++;
			int len = *fmt-'0';
			fmt++;
			if(*fmt == 'd')
			{
				int result = va_arg(va, int);
				char d[32];
				int i;
				for(i = 0; result != 0; i++)
				{
					d[i] = result % 10 + '0';
					result /= 10;
				}
				len -= i;
				i--;
				while(len > 0)
				{
					_putc('0');
					len--;
					n++;
				}
				while(i >= 0)
				{
					_putc(d[i]);
					i--;
					n++;
				}
			}
			else if(*fmt == 'x')
			{
				int result = va_arg(va, int);
				char d[32];
				int i;
				for(i = 0; result != 0; i++)
				{
					d[i] = result % 16 + '0';
					if(d[i] > 57)
						d[i] += 39;
					result /= 16;
				}
				len -= i;
				i--;
				while(len > 0)
				{
					_putc('0');
					len--;
					n++;
				}
				while(i >= 0)
				{
					_putc(d[i]);
					i--;
					n++;
				}
			}
		}
	}
	else if(*fmt == '\\') {
		fmt++;
		if(*fmt == 'n')
			_putc('\n');
	}
	else {
		_putc(*fmt);
		n++;
	}
	fmt++;
  }
  va_end(va);
  return n;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  return 0;
}

int sprintf(char *out, const char *fmt, ...) {
  assert(fmt);
  va_list va;
  int n = 0;
  va_start(va, fmt);
  while(*fmt != '\0')
  { 
	if (*fmt == '%') {
		fmt++;
		if (*fmt == 's') {
			char *s = va_arg(va,char*);
			while(*s != '\0')
			{
				*out = *s;
				out++;
				n++;
				s++;
			}
		}
		else if (*fmt == 'd') {
			int result = va_arg(va, int);
			char d[32];
			int temp = result;
			int i;
			for(i = 0; temp != 0; i++)
			{
				d[i] = temp % 10 + '0';
				temp /= 10;
			}
			i--;
			while(i >= 0)
			{
				*out = d[i];
				i--;
				out++;
				n++;
			}
		}
	}
	else {
		*out = *fmt;
		out++;
		n++;
	}
	fmt++;
  }
  *out = '\0';
  va_end(va);
  return n;
}


int snprintf(char *out, size_t n, const char *fmt, ...) {
  assert(fmt);
  va_list va;
  int m = 0;
  va_start(va, fmt);
  while(*fmt != '\0')
  { 
	if (*fmt == '%') {
		fmt++;
		if (*fmt == 's') {
			char *s = va_arg(va,char*);
			while(*s != '\0')
			{
				*out = *s;
				out++;
				m++;
				s++;
				if(m == n)
				{
					*out = '\0';
					return strlen(fmt);
				}
			}
		}
		else if (*fmt == 'd') {
			int result = va_arg(va, int);
			char d[32];
			int temp = result;
			int i;
			for(i = 0; temp != 0; i++)
			{
				d[i] = temp % 10 + '0';
				temp /= 10;
			}
			i--;
			while(i >= 0)
			{
				*out = d[i];
				i--;
				out++;
				m++;
				if(m == n)
				{
					*out = '\0';
					return strlen(fmt);
				}
			}
		}
	}
	else {
		*out = *fmt;
		out++;
		m++;
		if(m == n)
		{
			*out = '\0';
			return strlen(fmt);
		}
	}
	fmt++;
  }
  *out = '\0';
  va_end(va);
  return n;
}

#endif