/*
 * ngx_result.h
 *
 *      Author: Gandalf
 *      email： zhibu1991@gmail.com
 */

#ifndef NGX_RESULT_H_
#define NGX_RESULT_H_
#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>

#ifndef MIN
#define MIN(a,b) (((a)<(b))?(a):(b))
#endif /* MIN */
#ifndef MAX
#define MAX(a,b) (((a)>(b))?(a):(b))
#endif  /* MAX */

#define MAX_STRING_LEN		2048
#define MAX_EXECUTE_OUTPUT_LEN	(512 * 1024)
#define	SUCCEED		0
#define	FAIL		-1
#define PIPE_BUFFER_SIZE	4096
#define ngxexecute_free(ptr)		\
				\
do				\
{				\
	if (ptr)		\
	{			\
		free(ptr);	\
		ptr = NULL;	\
	}			\
}				\
while (0)

#define ngxexecute_realloc(src, size)		ngxexecute_realloc2(__FILE__, __LINE__, src, size)
void    *ngxexecute_realloc2(const char *filename, int line, void *old, size_t size);
#define ngxexecute_malloc(old, size)		ngxexecute_malloc2(__FILE__, __LINE__, old, size)
void    *ngxexecute_malloc2(const char *filename, int line, void *old, size_t size);
#define ngxexecute_strdup(old, str)		ngxexecute_strdup2(__FILE__, __LINE__, old, str)
char    *ngxexecute_strdup2(const char *filename, int line, char *old, const char *str);
int	ngxexecute_execute(const char *command, char **buffer, char *error, size_t max_error_len);
int	ngxexecute_popen(pid_t *pid, const char *command);
void    *ngxexecute_realloc2(const char *filename, int line, void *old, size_t size);
void	ngxexecute_strcpy_alloc(char **str, size_t *alloc_len, size_t *offset, const char *src);
void	ngxexecute_strncpy_alloc(char **str, size_t *alloc_len, size_t *offset, const char *src, size_t n);
void    *ngxexecute_malloc2(const char *filename, int line, void *old, size_t size);
size_t	ngxexecute_strlcpy(char *dst, const char *src, size_t siz);
char    *ngxexecute_strdup2(const char *filename, int line, char *old, const char *str);
int	ngx_waitpid(pid_t pid);
#endif /* NGX_RESULT_H_ */
