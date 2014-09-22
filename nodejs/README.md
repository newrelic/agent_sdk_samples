# Node.js Sample

This sample demonstrates how to get started using the Agent SDK with Node.js.
If you're looking for an out-of-the-box New Relic solution for your Node.js 
code, you should not use the Agent SDK. Instead, you should use the [Node.js 
agent](http://newrelic.com/nodejs) that will automatically instrument your code.

Make sure you have a [New Relic account](http://newrelic.com) before starting.

## Getting started

1. [Install the Agent SDK](http://download.newrelic.com/agent_sdk/)

2. Install [node-gyp](https://github.com/TooTallNate/node-gyp/)

3. Modify wrapper/binding.gyp if you put the Agent SDK libraries and header
   files in directories other than /usr/local/lib and /usr/local/include, 
   respectively.

4. Configure and build

```
   $ cd wrapper
   $ node-gyp configure
   $ node-gyp build
```

5. Set environment variables required for the Agent SDK daemon to connect to 
   New Relic, e.g.:

```
   $ export NEWRELIC_LICENSE_KEY=<your license key>
   $ export NEWRELIC_APP_NAME="My Demo App"
   $ export NEWRELIC_APP_LANGUAGE="Node.js"
   $ export NEWRELIC_APP_LANGUAGE_VERSION="0.10.30"
```

6. Start the newrelic-collector-client-daemon that comes with your Agent SDK
   download:

```
   $ ./newrelic-collector-client-daemon
```

7. Run the sample code inside nodejs/sample

```
   $ node hello_new_relic.js
````

The sample code only sends one transaction and one custom metric up to New 
Relic. You should see data show up [the New Relic UI](https://rpm.newrelic.com/) 
after a few minutes. In order to see the custom metric, you'll have to create
a custom dashboard.

If you don't see data show up, check the Agent SDK logs. If you don't see the
log files, make sure your log configuration file is in one of the 2 default 
paths (described in the Agent SDK How-To guide included in your download) or 
set the NEWRELIC_LOG_PROPERTIES_FILE environment variable to the correct path.
You can also change the log level to debug inside this file.

## Configuring the Agent SDK

Read the How-To guide (AgentSDK.pdf) that came with your Agent SDK download on 
how to configure the Agent SDK.
