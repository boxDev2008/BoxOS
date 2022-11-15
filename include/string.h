#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

uint32_t digit_count(int num, int base);

void kfree(void* memory);

void *memset(void *dst, char c, uint32_t n);

void *memcpy(void *dst, const void *src, uint32_t n);

int memcmp(uint8_t *s1, uint8_t *s2, uint32_t n);

void *memmove(void *dest, void *src, size_t n);

bool strncmp(char *a, char *b, uint32_t n); 

int strlen(const char *s);

int strcmp(const char *s1, char *s2);

int strcpy(char *dst, const char *src);

void strcat(char *dest, const char *src);

void strncat(char *dest, const char *src, int n);

uint32_t strsplit(char* str, char delim);

int isspace(char c);

int isalpha(char c);
char upper(char c);
char lower(char c);

void itoa(char *buf, int base, int d);

bool strprefix(const char *prefix, const char *str);

uint8_t strbackspace(char *s, char c);

void strreverse(char *s);