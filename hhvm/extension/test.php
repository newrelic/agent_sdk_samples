<?php
$loaded = extension_loaded("newrelic");

print("newrelic loaded: ".$loaded."\n");

print("hhvm_newrelic_instrumentation_enable exists: ".function_exists("hhvm_newrelic_instrumentation_enable")."\n");
print("hhvm_newrelic_instrumentation_disable exists: ".function_exists("hhvm_newrelic_instrumentation_disable")."\n");
print("hhvm_newrelic_transaction_begin exists: ".function_exists("hhvm_newrelic_transaction_begin")."\n");
print("hhvm_newrelic_transaction_notice_error exists: ".function_exists("hhvm_newrelic_transaction_notice_error")."\n");
print("hhvm_newrelic_transaction_add_attribute exists: ".function_exists("hhvm_newrelic_transaction_add_attribute")."\n");
print("hhvm_newrelic_transaction_set_name exists: ".function_exists("hhvm_newrelic_transaction_set_name")."\n");
print("hhvm_newrelic_transaction_set_request_url exists: ".function_exists("hhvm_newrelic_transaction_set_request_url")."\n");
print("hhvm_newrelic_transaction_set_threshold exists: ".function_exists("hhvm_newrelic_transaction_set_threshold")."\n");
print("hhvm_newrelic_transaction_set_max_trace_segments exists: ".function_exists("hhvm_newrelic_transaction_set_max_trace_segments")."\n");
print("hhvm_newrelic_transaction_end exists: ".function_exists("hhvm_newrelic_transaction_end")."\n");
print("hhvm_newrelic_segment_generic_begin exists: ".function_exists("hhvm_newrelic_segment_generic_begin")."\n");
print("hhvm_newrelic_segment_datastore_begin exists: ".function_exists("hhvm_newrelic_segment_datastore_begin")."\n");
print("hhvm_newrelic_segment_end exists: ".function_exists("hhvm_newrelic_segment_end")."\n");

$instrumentation_enable_disable_code = hhvm_newrelic_instrumentation_disable();
print("instrumentation_enable_disable_code: ".$instrumentation_enable_disable_code."\n");

$instrumentation_enable_error_code = hhvm_newrelic_instrumentation_enable();
print("instrumentation_enable_error_code: ".$instrumentation_enable_error_code."\n");

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

$threshold_error_code = hhvm_newrelic_transaction_set_threshold(100);
print("threshold_error_code: ".$threshold_error_code."\n");

$set_max_trace_segments_error_code = hhvm_newrelic_transaction_set_max_trace_segments(100);
print("set_max_trace_segments_error_code: ".$set_max_trace_segments_error_code."\n");

$generic_segment_id = hhvm_newrelic_segment_generic_begin("my_segment");
print("generic_segment_id: ".$generic_segment_id."\n");
sleep(2);

$datastore_segment_id = hhvm_newrelic_segment_datastore_begin("my_table", "select");
print("datastore_segment_id: ".$datastore_segment_id."\n");

sleep(1);

$datastore_end_error_code = hhvm_newrelic_segment_end($datastore_segment_id);
print("datastore_end_error_code: ".$datastore_end_error_code."\n");

$generic_end_error_code = hhvm_newrelic_segment_end($generic_segment_id);
print("generic_end_error_code: ".$generic_end_error_code."\n");

$end_error_code = hhvm_newrelic_transaction_end();
print("end_error_code: ".$end_error_code."\n");
