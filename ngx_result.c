/*
 * ngx_result.c
 *
 *      Author: Gandalf
 *      email： zhibu1991@gmail.com
 */
#include "ngx_result.h"
#include "ngx_str.h"
int	ngxexecute_execute(const char *command, char **buffer, char *error, size_t max_error_len)
{
	size_t			buf_size = PIPE_BUFFER_SIZE, offset = 0;
	int			ret = FAIL;
	pid_t			pid;
	int			fd;

	*error = '\0';

	if (NULL != buffer)
	{
		*buffer = ngxexecute_realloc(*buffer, buf_size);
		**buffer = '\0';
	}
	alarm(3);
	if (-1 != (fd = ngxexecute_popen(&pid, command)))
	{
		int	rc = 0;
		char	tmp_buf[PIPE_BUFFER_SIZE];

		if (NULL != buffer)
		{
			while (0 < (rc = read(fd, tmp_buf, sizeof(tmp_buf) - 1)) &&
					MAX_EXECUTE_OUTPUT_LEN > offset + rc)
			{
				tmp_buf[rc] = '\0';
				ngxexecute_strcpy_alloc(buffer, &buf_size, &offset, tmp_buf);
			}
		}
		close(fd);
		if (-1 == rc || -1 == ngx_waitpid(pid))
				{
					 kill(-pid, SIGTERM);
				}
		else
					ret = SUCCEED;
	}


	return ret;
}

int	ngxexecute_popen(pid_t *pid, const char *command)
{
	int		fd[2];

	if (-1 == pipe(fd))
		return -1;

	if (-1 == (*pid = ngxexecute_fork()))
	{
		close(fd[0]);
		close(fd[1]);
		return -1;
	}

	if (0 != *pid)	/* parent process */
	{
		close(fd[1]);
		return fd[0];
	}

	/* child process */
	close(fd[0]);
	dup2(fd[1], STDOUT_FILENO);
	dup2(fd[1], STDERR_FILENO);
	close(fd[1]);

	/* set the child as the process group leader, otherwise orphans may be left after timeout */
	if (-1 == setpgid(0, 0))
	{
		exit(EXIT_SUCCESS);
	}

	execl("/bin/sh", "sh", "-c", command, NULL);

	exit(EXIT_SUCCESS);
}

void    *ngxexecute_realloc2(const char *filename, int line, void *old, size_t size)
{
	int	max_attempts;
	void	*ptr = NULL;

	for (
		max_attempts = 10, size = MAX(size, 1);
		0 < max_attempts && NULL == ptr;
		ptr = realloc(old, size), max_attempts--
	);

	if (NULL != ptr)
		return ptr;

	exit(EXIT_FAILURE);
}

void	ngxexecute_strcpy_alloc(char **str, size_t *alloc_len, size_t *offset, const char *src)
{
	ngxexecute_strncpy_alloc(str, alloc_len, offset, src, strlen(src));
}

void	ngxexecute_strncpy_alloc(char **str, size_t *alloc_len, size_t *offset, const char *src, size_t n)
{
	if (NULL == *str)
	{
		*alloc_len = n + 1;
		*offset = 0;
		*str = ngxexecute_malloc(*str, *alloc_len);
	}
	else if (*offset + n >= *alloc_len)
	{
		while (*offset + n >= *alloc_len)
			*alloc_len *= 2;
		*str = ngxexecute_realloc(*str, *alloc_len);
	}

	while (0 != n && '\0' != *src)
	{
		(*str)[(*offset)++] = *src++;
		n--;
	}

	(*str)[*offset] = '\0';
}

void    *ngxexecute_malloc2(const char *filename, int line, void *old, size_t size)
{
	int	max_attempts;
	void	*ptr = NULL;

	/* old pointer must be NULL */
	if (NULL != old)
	{
	}

	for (
		max_attempts = 10, size = MAX(size, 1);
		0 < max_attempts && NULL == ptr;
		ptr = malloc(size), max_attempts--
	);

	if (NULL != ptr)
		return ptr;

	exit(EXIT_FAILURE);
}


size_t	ngxexecute_strlcpy(char *dst, const char *src, size_t siz)
{
	const char	*s = src;

	if (0 != siz)
	{
		while (0 != --siz && '\0' != *s)
			*dst++ = *s++;

		*dst = '\0';
	}

	return s - src;	/* count does not include null */
}

char    *ngxexecute_strdup2(const char *filename, int line, char *old, const char *str)
{
	int	retry;
	char	*ptr = NULL;

	ngxexecute_free(old);

	for (retry = 10; 0 < retry && NULL == ptr; ptr = strdup(str), retry--)
		;

	if (NULL != ptr)
		return ptr;
	exit(EXIT_FAILURE);
}
int	ngx_waitpid(pid_t pid)
{
	int		rc, status;
	do
	{
#ifdef WCONTINUED
		static int	wcontinued = WCONTINUED;
retry:
		if (-1 == (rc = waitpid(pid, &status, WUNTRACED | wcontinued)))
		{
			if (EINVAL == errno && 0 != wcontinued)
			{
				wcontinued = 0;
				goto retry;
			}
#else
		if (-1 == (rc = waitpid(pid, &status, WUNTRACED)))
		{
#endif
			goto exit;
		}
	}
	while (!WIFEXITED(status) && !WIFSIGNALED(status));
exit:

	return rc;
}




