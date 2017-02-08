/*
 * ngx_str.h
 *
 *      Author: Gandalf
 *      email： zhibu1991@gmail.com
 */

#ifndef NGX_STR_H_
#define NGX_STR_H_

#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>

int	ngxexecute_fork();
int	parse_command(const char *key, char *cmd, size_t cmd_max_len, char *param, size_t param_max_len);
int	is_key_char(char c);
void urldecode(char *dest, const char *src);

#endif /* NGX_STR_H_ */
