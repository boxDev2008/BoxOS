#include <string.h>

uint32_t digit_count(int num, int base)
{
    uint32_t count = 0;
    if(num == 0)
        return 1;
    while(num > 0){
        count++;
        num = num/base;
    }
    return count;
}

uint32_t strsplit(char* str, char delim)
{
	uint32_t n = 0;
	uint32_t i = 0;
	while(str[i])
	{
		if(str[i] == delim)
		{
			//printf(".");
			str[i] = 0;
			n++;
		}
		i++;
	}
	n++;
	return n;
}

void *memset(void *dst, char c, uint32_t n)
{
    char *temp = dst;
    for (; n != 0; n--) *temp++ = c;
    return dst;
}

void *memcpy(void *dst, const void *src, uint32_t n)
{
    char *ret = dst;
    char *p = dst;
    const char *q = src;
    while (n--)
        *p++ = *q++;
    return ret;
}

bool strncmp(char *a, char *b, uint32_t n)
{
    uint32_t i;
    for (i = 0; i < n; ++i)
        if (a[i] != b[i])
        {
            return true;
        }
    return false;
}

int memcmp(uint8_t *s1, uint8_t *s2, uint32_t n)
{
    while (n--)
    {
        if (*s1 != *s2)
            return 0;
        s1++;
        s2++;
    }
    return 1;
}

void *memmove(void *dest, void *src, size_t n)
{
    char *csrc = (char *)src;
    char *cdest = (char *)dest;

    char temp[n];

    for (int i = 0; i < n; i++)
        temp[i] = csrc[i];
    
    for (int i = 0; i < n; i++)
        cdest[i] = temp[i];
    
    return cdest;
}

int strlen(const char *s)
{
    int len = 0;
    while (*s++)
        len++;
    return len;
}

int strcmp(const char *s1, char *s2)
{
    int i = 0;

    while ((s1[i] == s2[i])) {
        if (s2[i++] == 0)
            return 0;
    }
    return 1;
}

int strcpy(char *dst, const char *src)
{
    int i = 0;
    while ((*dst++ = *src++) != 0)
        i++;
    return i;
}

void strcat(char *dest, const char *src)
{
    char *end = (char *)dest + strlen(dest);
    memcpy((void *)end, (void *)src, strlen(src));
    end = end + strlen(src);
    *end = '\0';
}

void strncat(char *dest, const char *src, int n)
{
    char *end = (char *)dest + strlen(dest);
    memcpy((void *)end, (void *)src, n);
    end = end + n;
    *end = '\0';
}

void itoa(char *buf, int base, int d)
{
    char *p = buf;
    char *p1, *p2;
    unsigned long ud = d;
    int divisor = 10;

    /* If %d is specified and D is minus, put ‘-’ in the head. */
    if (base == 'd' && d < 0)
    {
        *p++ = '-';
        buf++;
        ud = -d;
    }
    else if (base == 'x')
        divisor = 16;

    /* Divide UD by DIVISOR until UD == 0. */
    do
    {
        int remainder = ud % divisor;
        *p++ = (remainder < 10) ? remainder + '0' : remainder + 'a' - 10;
    }
    while (ud /= divisor);

    /* Terminate BUF. */
    *p = 0;

    /* Reverse BUF. */
    p1 = buf;
    p2 = p - 1;
    while (p1 < p2)
    {
        char tmp = *p1;
        *p1 = *p2;
        *p2 = tmp;
        p1++;
        p2--;
    }
}

bool strprefix(const char *prefix, const char *str)
{
	return strncmp(prefix, str, strlen(prefix)) == 0;
}

uint8_t strbackspace(char *s, char c)
{
	size_t i = strlen(s);
	while (--i)
	{
		if (s[i] == c)
		{
			s[i + 1] = 0;
			return 1;
		}
	}
	return 0;
}

void strreverse(char *s)
{
	int i, j;
	uint8_t c;

	for (i = 0, j = strlen(s) - 1; i < j; i++, j--)
	{
		c    = s[i];
		s[i] = s[j];
		s[j] = c;
	}
}


char *strtok(char *str, const char *delim)
{
    static char *temp_ptr = NULL;
    char *final_ptr = NULL;

    static int flag = 0;
    int i, j;

    if (delim == NULL)
        return NULL;

    if (flag == 1)
        return NULL;

    if (str != NULL)
        temp_ptr = str; 

    final_ptr = temp_ptr;

    for (i = 0; i <= strlen(temp_ptr); i++)
    {
        for (j = 0; j < strlen(delim); j++)
        {

            if (temp_ptr[i] == '\0')
            {
                flag = 1;
                return final_ptr;
            }

            if ((temp_ptr[i] == delim[j]))
            {
                temp_ptr[i] = '\0';
                temp_ptr += i+1;
                return final_ptr;
            }
        }
    }

    return NULL;
}
