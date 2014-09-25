#!/bin/sh

DIRNAME=`dirname $0`
REALPATH=`which realpath`
if [ ! -z "${REALPATH}" ]; then
	DIRNAME=`realpath ${DIRNAME}`
fi

hhvm -vDynamicExtensions.0=${DIRNAME}/newrelic.so ${DIRNAME}/test.php
