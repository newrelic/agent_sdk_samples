# Nginx Sample

The sample includes an Nginx module that provides a directive to enable New
Relic for your applications and a sample Nginx configuration file that 
uses the directive to enable New Relic at the server level for all requests.

Make sure you have a [New Relic account](http://newrelic.com) before
starting. To see all the features, such as slow transaction traces, you will
need a New Relic Pro subscription (or equivalent).

## Getting started

1. [Install the Agent SDK](http://download.newrelic.com/agent_sdk/).
2. [Install OpenResty](http://openresty.org/#Installation) - it's the easiest
   way to get started. It installs Nginx, ngx_lua, and LuaJIT, which are
   required for this example.
3. Add the New Relic module to Nginx. To add the New Relic module and link to 
   the Agent SDK libraries, you'll need to configure your Nginx build like so:
   $ cd .../path/to/nginx
   $ ./configure \
     --with-luajit
     --with-ld-opt="-L /usr/local/lib -lnewrelic-transaction -lnewrelic-common -lnewrelic-collector-client" \
     --with-cc-opt="-I /usr/local/include" \
     --add-module=path/to/ngx_http_newrelic_module
4. Pass your New Relic license key to the newrelic_license_key directive
   inside your nginx.config file.
5. Start Nginx and run the sample.
   $ sudo /usr/local/openresty/nginx/sbin/nginx -p "$(pwd)" -c conf/nginx.conf
   $ curl http://localhost:7747

You should see the performance data from your requests appear within 
[the New Relic UI](https://rpm.newrelic.com/) after a few minutes. The agent 
only reports data once a minute to minimize the amount of bandwidth it
consumes.

If you don't see data show up, check the Agent SDK logs. If you don't see the
log files, make sure your log configuration file is in one of the 2 default 
paths (described in the Agent SDK How-To guide included in your download) or 
set the NEWRELIC_LOG_PROPERTIES_FILE environment variable to the correct path.
You can also change the log level to debug inside this file.

## Configuring the Agent SDK

Read the How-To guide that came with your Agent SDK download on how to
configure the Agent SDK.

## LICENSE

Agent SDK samples code is free to use. Please see LICENSE in this distribution 
for details on its license.