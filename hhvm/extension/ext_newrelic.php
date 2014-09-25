<?hh
<<__Native>>
function hhvm_newrelic_enable_instrumentation(): void;

<<__Native>>
function hhvm_newrelic_disable_instrumentation(): void;

<<__Native>>
function hhvm_newrelic_transaction_begin(): int;

<<__Native>>
function hhvm_newrelic_transaction_notice_error(string $exception_type, string $error_message, string $stack_trace, string $stack_frame_delimiter): int;

<<__Native>>
function hhvm_newrelic_transaction_add_attribute(string $name, string $value): int;

<<__Native>>
function hhvm_newrelic_transaction_set_name(string $name): int;

<<__Native>>
function hhvm_newrelic_transaction_set_request_url(string $name): int;

<<__Native>>
function hhvm_newrelic_transaction_set_max_trace_segments(int $max_trace_segments): int;

<<__Native>>
function hhvm_newrelic_transaction_end(): int;

<<__Native>>
function hhvm_newrelic_segment_generic_begin(string $name): int;

<<__Native>>
function hhvm_newrelic_segment_datastore_begin(string $table, string $operation, string $sql): int;

<<__Native>>
function hhvm_newrelic_segment_end(int $id): int;

<<__Native>>
function hhvm_newrelic_get_scoped_generic_segment(string $name): mixed;

<<__Native>>
function hhvm_newrelic_get_scoped_database_segment(string $table, string $operation, string $sql): mixed;

<<__Native>>
function hhvm_newrelic_get_scoped_transaction(): mixed;
