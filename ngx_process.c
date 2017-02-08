/*
 * ngx_process.c
 *
 *      Author: Gandalf
 *      email： zhibu1991@gmail.com
 */
#include "ngx_str.h"

int	parse_command(const char *key, char *cmd, size_t cmd_max_len, char *param, size_t param_max_len)
{
	const char	*pl, *pr;
	size_t		sz;
	for (pl = key; 0 == is_key_char(*pl); pl++)
		;

	if (pl == key)
		return 0;

	if (NULL != cmd)
	{
		if (cmd_max_len <= (sz = (size_t)(pl - key)))
			return 0;

		memcpy(cmd, key, sz);
		cmd[sz] = '\0';
	}

	if ('\0' == *pl)	/* no parameters specified */
	{
		if (NULL != param)
			*param = '\0';
		return 1;
	}

	if ('[' != *pl)		/* unsupported character */
		return 0;

	for (pr = ++pl; '\0' != *pr; pr++)
		;

	if (']' != *--pr)
		return 0;

	if (NULL != param)
	{
		if (param_max_len <= (sz = (size_t)(pr - pl)))
			return 0;

		memcpy(param, pl, sz);
		param[sz] = '\0';
	}

	return 2;
}
int	is_key_char(char c)
{
	if (0 != isalnum(c))
		return 0;

	if (c == '.' || c == '_' || c == '-')
		return 0;

	return -1;
}

int	ngxexecute_fork()
{
	fflush(stdout);
	fflush(stderr);
	return fork();
}

void urldecode(char *dest, const char *src)
{
    const char *p = src;
    char code[3] = {0};
    unsigned long ascii = 0;
    char *end = NULL;

    while(*p)
    {
        if(*p == '%')
        {
            memcpy(code, ++p, 2);
            ascii = strtoul(code, &end, 16);
            *dest++ = (char)ascii;
            p += 2;
        }
        else
            *dest++ = *p++;
    }
}

