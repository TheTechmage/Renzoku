#!/bin/bash

cd "$(dirname "$0")"
exec ./test_runner -r tap -s
