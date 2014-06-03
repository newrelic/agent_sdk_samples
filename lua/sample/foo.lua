local transaction_id = ngx.var.transaction_id;

newrelic.set_transaction_name(tonumber(transaction_id), "foo")