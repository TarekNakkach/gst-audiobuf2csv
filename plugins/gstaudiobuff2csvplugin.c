#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <gst/gst.h>
#include "gstaudiobuff2csv.h"

static gboolean
plugin_init (GstPlugin * plugin)
{
  return gst_element_register (plugin, "audiobuff2csv", GST_RANK_NONE,
      GST_TYPE_AUDIOBUFF2CSV);
}

#ifndef VERSION
#define VERSION "0.0.1"
#endif
#ifndef PACKAGE
#define PACKAGE "audiobuff2csv"
#endif
#ifndef PACKAGE_NAME
#define PACKAGE_NAME "audiobuff2csv"
#endif
#ifndef GST_PACKAGE_ORIGIN
#define GST_PACKAGE_ORIGIN "https://github.com/TarekNakkach/gst-audiobuf2csv.git"
#endif

GST_PLUGIN_DEFINE (GST_VERSION_MAJOR,
    GST_VERSION_MINOR,
    audiobuff2csv,
    "Audio buffer to CSV convert plugin",
    plugin_init, VERSION, "LGPL", PACKAGE_NAME, GST_PACKAGE_ORIGIN)
