#ifndef _NGX_HTTP_NEWRELIC_MODULE_H_INCLUDED_
#define _NGX_HTTP_NEWRELIC_MODULE_H_INCLUDED_

#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_event.h>
#include <ngx_http.h>
#include <nginx.h>

typedef struct {
	ngx_flag_t enable;
	ngx_str_t license_key;
} ngx_http_newrelic_main_conf_t;

typedef struct {
	ngx_flag_t enable;
} ngx_http_newrelic_loc_conf_t;

extern ngx_module_t ngx_http_newrelic_module;

#endif /* _NGX_HTTP_NEWRELIC_MODULE_H_INCLUDED_ */