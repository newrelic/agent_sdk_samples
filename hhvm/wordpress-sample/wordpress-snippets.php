// =================================================================
// Inside plugin.php
// ==================================================================

// Paste this code at the start of the do_action function right after the
// declaration of the global variables.
$segment;
if ("muplugins_loaded" == $tag) {
   // This action gets called near the start of the request.  This is a good
   // place to identify the start of a transaction
   $request_url=strtok($_SERVER["REQUEST_URI"],'?');
   hhvm_newrelic_transaction_begin();
   hhvm_newrelic_transaction_set_request_url($request_url);
} else if ("shutdown" == $tag) {
   // This action gets called near the end of the request. This is a good
   // place to identify the end of the transaction and determine its
   // name

   $transaction_name = "";

   if (function_exists("get_current_screen")) {
       // The name of the screen seems like a good name for transactions
       // that are part of the admin portion of a word press site.
       $screen = get_current_screen();

       if ($screen->parent_base != "") {
           $transaction_name = $screen->parent_base."/".$screen->base."/".$screen->action;
       }
   }

   if ($transaction_name == "") {
       // For non-admin actions, we name the transaction based on the type
       // of the page requested.
       if    (is_404()) $transaction_name = "404";
       elseif (is_search()) $transaction_name = "search";
       elseif (is_front_page()) $transaction_name = "front_page";
       elseif (is_home()) $transaction_name = "home";
       elseif (is_post_type_archive()) $transaction_name = "post_type_archive";
       elseif (is_tax()) $transaction_name = "tax";
       elseif (is_attachment()) $transaction_name = "attachment";
       elseif (is_single()) $transaction_name = "single";
       elseif (is_page()) $transaction_name = "page";
       elseif (is_category()) $transaction_name = "category";
       elseif (is_tag()) $transaction_name = "tag";
       elseif (is_author()) $transaction_name = "author";
       elseif (is_date()) $transaction_name = "date";
       elseif (is_archive()) $transaction_name = "archive";
       elseif (is_comments_popup()) $transaction_name = "comments_popup";
       else $transaction_name = "index";
   }

   if ($transaction_name != "") {
       hhvm_newrelic_transaction_set_name($transaction_name);
   }

   hhvm_newrelic_transaction_end();
}
else {
   // For all other actions, we create a generic segment. This is useful
   // for building the transaction trace tree
   $segment = hhvm_newrelic_get_scoped_generic_segment($tag);
}

// ==================================================================
// Inside wp-db.php
// ==================================================================

// Paste this code inside the query function right before @mysql_query is called
$segment;
// NOTE: This is just an example of how to get a table name from a query.
// It will only work for very simple queries.
if (preg_match( '/^\s*(select)\s/i', $query)) {
   if (preg_match("/\s+FROM\s+`?([a-z\d_]+)`?/i", $query, $match)) {
       $segment = hhvm_newrelic_get_scoped_database_segment($match[1], "select");
   }
}

// Paste this code at the very beginning of the insert function
$segment = hhvm_newrelic_get_scoped_database_segment($table, "insert");
