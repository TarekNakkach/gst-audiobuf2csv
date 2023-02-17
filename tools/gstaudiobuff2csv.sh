#!/bin/bash

GST_DEBUG=$3 gst-launch-1.0  audiotestsrc num-buffers=$2 ! audiobuff2csv ! filesink location=$1
