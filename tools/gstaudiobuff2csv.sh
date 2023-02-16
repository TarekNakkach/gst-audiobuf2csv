#!/bin/bash

DEBUG_LEVEL=$1

GST_DEBUG=$DEBUG_LEVEL gst-launch-1.0  audiotestsrc  ! audiobuff2csv ! autoaudiosink
