#include "ngx_http_newrelic_module.h"

#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_event.h>
#include <ngx_http.h>
#include <nginx.h>

#include <newrelic_collector_client.h>
#include <newrelic_common.h>
#include <newrelic_transaction.h>

typedef struct {
	long transaction_id;
	long parent_id;
	long segment_id;
} ngx_http_newrelic_shm_worker_t;

typedef struct {
	ngx_http_newrelic_shm_worker_t workers[NGX_MAX_PROCESSES];
} ngx_http_newrelic_shm_data_t;

static char *newrelic_license;
static ngx_shm_zone_t *ngx_http_newrelic_shm_zone;

static void *ngx_http_newrelic_create_main_conf(ngx_conf_t *cf);
static char *ngx_http_newrelic_init_main_conf(ngx_conf_t *cf, void *conf);
static void *ngx_http_newrelic_create_loc_conf(ngx_conf_t *cf);
static char *ngx_http_newrelic_merge_loc_conf(ngx_conf_t *cf, void *parent, void *child);
static ngx_int_t ngx_http_newrelic_init(ngx_conf_t *cf);
static ngx_int_t ngx_http_newrelic_process_init(ngx_cycle_t *cycle);
static void ngx_http_newrelic_process_exit(ngx_cycle_t *cycle);
static ngx_int_t ngx_http_newrelic_begin_request_handler(ngx_http_request_t *r);
static ngx_int_t ngx_http_newrelic_end_request_handler(ngx_http_request_t *r);

static ngx_command_t ngx_http_newrelic_commands[] = {
	{
		ngx_string("newrelic"),
		NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_HTTP_LIF_CONF|NGX_CONF_FLAG,
		ngx_conf_set_flag_slot,
		NGX_HTTP_MAIN_CONF_OFFSET,
		offsetof(ngx_http_newrelic_main_conf_t, enable),
		NULL
	},
	{
		ngx_string("newrelic_license_key"),
		NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
		ngx_conf_set_str_slot,
		NGX_HTTP_MAIN_CONF_OFFSET,
		offsetof(ngx_http_newrelic_main_conf_t, license_key),
		NULL
	},
	ngx_null_command
};

static ngx_http_module_t ngx_http_newreilc_module_ctx = {
		NULL,                                       /* preconfiguration */
		ngx_http_newrelic_init,                     /* postconfiguration */
		ngx_http_newrelic_create_main_conf,         /* create main configuration */
		ngx_http_newrelic_init_main_conf,           /* init main configuration */
		NULL,                                       /* create server configuration */
		NULL,                                       /* merge server configuration */
		ngx_http_newrelic_create_loc_conf,          /* create location configuration */
		ngx_http_newrelic_merge_loc_conf            /* merge location configuration */
};

ngx_module_t ngx_http_newrelic_module = {
		NGX_MODULE_V1,
		&ngx_http_newreilc_module_ctx,              /* module context */
		ngx_http_newrelic_commands,                 /* module directives */
		NGX_HTTP_MODULE,                            /* module type */
		NULL,                                       /* init master */
		NULL,                                       /* init module */
		ngx_http_newrelic_process_init,             /* init process */
		NULL,                                       /* init thread */
		NULL,                                       /* exit thread */
		ngx_http_newrelic_process_exit,             /* exit process */
		NULL,                                       /* exit master */
		NGX_MODULE_V1_PADDING
};

static ngx_http_newrelic_shm_data_t *ngx_http_newrelic_get_shm_data() {
	if (ngx_http_newrelic_shm_zone == NULL) {
		return NULL;
	}

	return (ngx_http_newrelic_shm_data_t *)ngx_http_newrelic_shm_zone->data;
}

static ngx_int_t ngx_http_newrelic_update_shm_slot(long transaction_id, long parent_id, long segment_id) {
	ngx_http_newrelic_shm_data_t *shm_data;

	if ((shm_data = ngx_http_newrelic_get_shm_data()) == NULL) {
		return NGX_ERROR;
	}

	shm_data->workers[ngx_process_slot].transaction_id = transaction_id;
	shm_data->workers[ngx_process_slot].parent_id = parent_id;
	shm_data->workers[ngx_process_slot].segment_id = segment_id;

	return NGX_OK;
}

static ngx_int_t ngx_http_newrelic_init_shm(ngx_shm_zone_t *shm_zone, void *data) { 
	ngx_slab_pool_t *shm_pool;
	ngx_http_newrelic_shm_data_t *shm_data;

	if (data) {
		shm_zone->data = data;
		return NGX_OK;
	}

	shm_pool = (ngx_slab_pool_t *)shm_zone->shm.addr;
	shm_data = shm_zone->data = ngx_slab_alloc(shm_pool, sizeof(ngx_http_newrelic_shm_data_t));

	// //initialise shm_zone->data
	// shm_zone->data = something_interesting;

	return NGX_OK;
}

static void *ngx_http_newrelic_create_main_conf(ngx_conf_t *cf) {
	// ngx_log_error(NGX_LOG_NOTICE, cf->log, 0, "NR ngx_http_newrelic_create_main_conf"); 

	ngx_http_newrelic_main_conf_t *newrelic_main_conf;
	newrelic_main_conf = ngx_pcalloc(cf->pool, sizeof(ngx_http_newrelic_main_conf_t));

	if (newrelic_main_conf == NULL) {
		return NULL; 
	}

	newrelic_main_conf->enable = NGX_CONF_UNSET;

	return newrelic_main_conf;
}

static char *ngx_http_newrelic_init_main_conf(ngx_conf_t *cf, void *conf) {
	ngx_http_newrelic_main_conf_t *newrelic_main_conf = conf;

	if (newrelic_main_conf->enable == NGX_CONF_UNSET) {
		newrelic_main_conf->enable = 0;
	}

	if (!newrelic_main_conf->license_key.data) {
		newrelic_main_conf->license_key.data = "";
		newrelic_main_conf->license_key.len = 0;
	}

	return NGX_CONF_OK;
}

static void *ngx_http_newrelic_create_loc_conf(ngx_conf_t *cf) {
	// ngx_log_error(NGX_LOG_INFO, cf->log, 0, "NR ngx_http_newrelic_create_loc_conf"); 

	ngx_http_newrelic_loc_conf_t *newrelic_loc_conf;
	newrelic_loc_conf = ngx_pcalloc(cf->pool, sizeof(ngx_http_newrelic_loc_conf_t));

	if (newrelic_loc_conf == NULL) {
		return NULL;
	}

	newrelic_loc_conf->enable = NGX_CONF_UNSET;

	return newrelic_loc_conf;
}

static char *ngx_http_newrelic_merge_loc_conf(ngx_conf_t *cf, void *parent, void *child) {
	ngx_http_newrelic_loc_conf_t *prev = parent;
	ngx_http_newrelic_loc_conf_t *conf = child;

	ngx_conf_merge_value(conf->enable, prev->enable, 0);
	if (conf->enable == NGX_CONF_UNSET) {
		if (prev->enable == NGX_CONF_UNSET) {
			conf->enable = 0;
		} else {
			conf->enable = prev->enable;
		}
	}

	// ngx_log_error(NGX_LOG_INFO, cf->log, 0, "NR ngx_http_newrelic_merge_loc_conf"); 

	return NGX_CONF_OK;
}

static ngx_int_t ngx_http_newrelic_init(ngx_conf_t *cf) {
	ngx_http_handler_pt *begin_request_handler;
	ngx_http_handler_pt *end_request_handler;
	ngx_http_core_main_conf_t *core_main_conf;
	ngx_http_newrelic_main_conf_t *newrelic_main_conf;

	newrelic_main_conf = ngx_http_conf_get_module_main_conf(cf, ngx_http_newrelic_module);

	if (!newrelic_main_conf->enable) {
		ngx_log_error(NGX_LOG_NOTICE, cf->log, 0, "newrelic is not enabled");
		return NGX_OK;
	}

	if (newrelic_main_conf->license_key.data) {
		ngx_log_error(NGX_LOG_NOTICE, cf->log, 0, "newrelic license key is nil");
		return NGX_OK;
	}

	newrelic_license = newrelic_main_conf->license_key.data;

	core_main_conf = ngx_http_conf_get_module_main_conf(cf, ngx_http_core_module);

	begin_request_handler = ngx_array_push(&core_main_conf->phases[NGX_HTTP_POST_READ_PHASE].handlers);
	end_request_handler = ngx_array_push(&core_main_conf->phases[NGX_HTTP_LOG_PHASE].handlers);

	if (begin_request_handler == NULL) {
		return NGX_ERROR;
	}

	if (begin_request_handler == NULL) {
		return NGX_ERROR;
	}

	*begin_request_handler = ngx_http_newrelic_begin_request_handler;
	*end_request_handler = ngx_http_newrelic_end_request_handler;

	ngx_str_t *shm_name = "ngx_http_newrelic_shm_name";
	size_t shm_size = (ngx_pagesize * 2) + ngx_align(sizeof(ngx_http_newrelic_shm_data_t), ngx_pagesize);
	ngx_http_newrelic_shm_zone = ngx_shared_memory_add(cf, shm_name, shm_size, &ngx_http_newrelic_module);
	ngx_http_newrelic_shm_zone->init = ngx_http_newrelic_init_shm;

	// ngx_log_error(NGX_LOG_NOTICE, cf->log, 0, "ngx_http_newrelic_init");

	return NGX_OK;
}

static ngx_int_t ngx_http_newrelic_process_init(ngx_cycle_t *cycle) {
	// ngx_log_error(NGX_LOG_NOTICE, cycle->log, 0, "NR ngx_http_newrelic_init_process");

	ngx_http_newrelic_main_conf_t *newrelic_main_conf;

	newrelic_main_conf = ngx_http_cycle_get_module_main_conf(cycle, ngx_http_newrelic_module);

	if (!newrelic_main_conf->enable) {
		return NGX_OK;
	}

	newrelic_register_message_handler(newrelic_message_handler);

	// char *newrelic_license = getenv("NEWRELIC_LICENSE_KEY");
	// char *app_name = getenv("NEWRELIC_APP_NAME");
	// char *app_lang = getenv("NEWRELIC_APP_LANGUAGE");
	// char *app_lang_version = getenv("NEWRELIC_APP_LANGUAGE_VERSION");

	char *app_name = "nginx example";
	char *app_lang = "lua";
	char *app_lang_version = "5.1.5";

	int nr_error_code = newrelic_init(newrelic_license, app_name, app_lang, app_lang_version);

	return NGX_OK;
}

static void ngx_http_newrelic_process_exit(ngx_cycle_t *cycle) {
	ngx_log_error(NGX_LOG_NOTICE, cycle->log, 0, "NR ngx_http_newrelic_exit_process");

	ngx_http_newrelic_main_conf_t *newrelic_conf;

	newrelic_conf = ngx_http_cycle_get_module_main_conf(cycle, ngx_http_newrelic_module);

	if (!newrelic_conf->enable) {
		return;
	}
}

static ngx_int_t ngx_http_newrelic_begin_request_handler(ngx_http_request_t *r) {
	ngx_log_error(NGX_LOG_NOTICE, r->connection->log, 0, "NR ngx_http_newrelic_begin_request_handler");

	ngx_http_newrelic_main_conf_t *newrelic_main_conf = ngx_http_get_module_main_conf(r, ngx_http_newrelic_module); 
	ngx_http_newrelic_loc_conf_t *newrelic_loc_conf = ngx_http_get_module_loc_conf(r, ngx_http_newrelic_module);  

	long transaction_id = newrelic_transaction_begin();

	ngx_http_newrelic_update_shm_slot(transaction_id, 0, 0);

	return NGX_OK;
}

static ngx_int_t ngx_http_newrelic_end_request_handler(ngx_http_request_t *r) {
	ngx_log_error(NGX_LOG_NOTICE, r->connection->log, 0, "NR ngx_http_newrelic_end_request_handler");

	ngx_http_newrelic_shm_data_t *shm_data;

	if ((shm_data = ngx_http_newrelic_get_shm_data()) == NULL) {
	 return NGX_ERROR;
	}

	long transaction_id = shm_data->workers[ngx_process_slot].transaction_id;

	int return_code = newrelic_transaction_end(transaction_id);

	// ngx_http_newrelic_update_shm_slot(0, 0, 0);

	return NGX_OK;
}