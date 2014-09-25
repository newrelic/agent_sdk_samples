<?php

function scoped_test() {
	$transaction = hhvm_newrelic_get_scoped_transaction();
	hhvm_newrelic_transaction_set_name("scoped_transaction");
	hhvm_newrelic_transaction_set_request_url("/my/scoped/transaction");
	$generic_segment = hhvm_newrelic_get_scoped_generic_segment("generic_segment_name");
	sleep(2);
	$database_segment = hhvm_newrelic_get_scoped_database_segment("table", "select", "select * from table");
	sleep(3);
}

$loaded = extension_loaded("newrelic");

print("newrelic extension loaded: ".$loaded."\n");

print("hhvm_newrelic_enable_instrumentation exists: ".function_exists("hhvm_newrelic_enable_instrumentation")."\n");
print("hhvm_newrelic_disable_instrumentation exists: ".function_exists("hhvm_newrelic_disable_instrumentation")."\n");
print("hhvm_newrelic_transaction_begin exists: ".function_exists("hhvm_newrelic_transaction_begin")."\n");
print("hhvm_newrelic_transaction_notice_error exists: ".function_exists("hhvm_newrelic_transaction_notice_error")."\n");
print("hhvm_newrelic_transaction_add_attribute exists: ".function_exists("hhvm_newrelic_transaction_add_attribute")."\n");
print("hhvm_newrelic_transaction_set_name exists: ".function_exists("hhvm_newrelic_transaction_set_name")."\n");
print("hhvm_newrelic_transaction_set_request_url exists: ".function_exists("hhvm_newrelic_transaction_set_request_url")."\n");
print("hhvm_newrelic_transaction_set_max_trace_segments exists: ".function_exists("hhvm_newrelic_transaction_set_max_trace_segments")."\n");
print("hhvm_newrelic_transaction_end exists: ".function_exists("hhvm_newrelic_transaction_end")."\n");
print("hhvm_newrelic_segment_generic_begin exists: ".function_exists("hhvm_newrelic_segment_generic_begin")."\n");
print("hhvm_newrelic_segment_datastore_begin exists: ".function_exists("hhvm_newrelic_segment_datastore_begin")."\n");
print("hhvm_newrelic_segment_end exists: ".function_exists("hhvm_newrelic_segment_end")."\n");
print("hhvm_newrelic_get_scoped_generic_segment: ".function_exists("hhvm_newrelic_get_scoped_generic_segment")."\n");
print("hhvm_newrelic_get_scoped_database_segment: ".function_exists("hhvm_newrelic_get_scoped_database_segment")."\n");
print("hhvm_newrelic_get_scoped_transaction: ".function_exists("hhvm_newrelic_get_scoped_transaction")."\n");

hhvm_newrelic_disable_instrumentation();
hhvm_newrelic_enable_instrumentation();

$transaction_id = hhvm_newrelic_transaction_begin();
print("transaction_id: ".$transaction_id."\n");

$notice_error_error_code = hhvm_newrelic_transaction_notice_error("exception_type", "error_message", "stack_trace", "stack_frame_delimiter");
print("notice_error_error_code: ".$notice_error_error_code."\n");

$add_attribute_error_code = hhvm_newrelic_transaction_add_attribute("attr_name", "attr_value");
print("add_attribute_error_code: ".$add_attribute_error_code."\n");

$name_error_code = hhvm_newrelic_transaction_set_name("my_transaction");
print("name_error_code: ".$name_error_code."\n");

$request_url_error_code = hhvm_newrelic_transaction_set_request_url("my/path");
print("request_url_error_code: ".$name_error_code."\n");

$set_max_trace_segments_error_code = hhvm_newrelic_transaction_set_max_trace_segments(100);
print("set_max_trace_segments_error_code: ".$set_max_trace_segments_error_code."\n");

$generic_segment_id = hhvm_newrelic_segment_generic_begin("my_segment");
print("generic_segment_id: ".$generic_segment_id."\n");

sleep(2);

$datastore_segment_id = hhvm_newrelic_segment_datastore_begin("my_table", "select", "select * from table");
print("datastore_segment_id: ".$datastore_segment_id."\n");

sleep(1);

$datastore_end_error_code = hhvm_newrelic_segment_end($datastore_segment_id);
print("datastore_end_error_code: ".$datastore_end_error_code."\n");

$generic_end_error_code = hhvm_newrelic_segment_end($generic_segment_id);
print("generic_end_error_code: ".$generic_end_error_code."\n");

$end_error_code = hhvm_newrelic_transaction_end();
print("end_error_code: ".$end_error_code."\n");

scoped_test();
