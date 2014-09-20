#include <nan.h>
#include <newrelic_common.h>
#include <newrelic_transaction.h>

NAN_METHOD(record_metric) {
	NanScope();
	v8::String::Utf8Value metric_name_str(args[0]->ToString());
	char *metric_name = *(metric_name_str);
	double metric_value = args[1]->NumberValue();
	int return_code = newrelic_record_metric(metric_name, metric_value);

	NanReturnValue(NanNew<v8::Number>(return_code));
}

NAN_METHOD(begin_transaction) {
	NanScope();
	long transaction_id = newrelic_transaction_begin();

	NanReturnValue(NanNew<v8::Number>(transaction_id));
}

NAN_METHOD(end_transaction) {
	NanScope();
	long transaction_id = args[0]->NumberValue();

	NanReturnValue(NanNew<v8::Number>(newrelic_transaction_end(transaction_id)));
}

NAN_METHOD(set_transaction_name) {
	NanScope();
	long transaction_id = args[0]->NumberValue();
	v8::String::Utf8Value transaction_name_str(args[1]->ToString());
	char *transaction_name = *(transaction_name_str);

	NanReturnValue(NanNew<v8::Number>(newrelic_transaction_set_name(transaction_id, transaction_name)));
}

void Init(v8::Handle<v8::Object> exports) {
    exports->Set(NanNew<v8::String>("record_metric"), NanNew<v8::FunctionTemplate>(record_metric)->GetFunction());
    exports->Set(NanNew<v8::String>("begin_transaction"), NanNew<v8::FunctionTemplate>(begin_transaction)->GetFunction());
    exports->Set(NanNew<v8::String>("end_transaction"), NanNew<v8::FunctionTemplate>(end_transaction)->GetFunction());
    exports->Set(NanNew<v8::String>("set_transaction_name"), NanNew<v8::FunctionTemplate>(set_transaction_name)->GetFunction());
}

NODE_MODULE(agentsdk, Init)