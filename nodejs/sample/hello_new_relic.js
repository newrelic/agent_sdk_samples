var agentsdk = require('../wrapper/agentsdk');

agentsdk.record_metric("CustomTransaction/CustomMetric", 0.5);

var transaction_id = agentsdk.begin_transaction();

agentsdk.set_transaction_name(transaction_id, "NodeJsRocks");

setTimeout(function() {
	agentsdk.end_transaction(transaction_id);
}, 1000);