#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gst/gst.h>
#include <gst/audio/gstaudiofilter.h>
#include "gstaudiobuff2csv.h"

GST_DEBUG_CATEGORY_STATIC (gst_audiobuff2csv_debug_category);
#define GST_CAT_DEFAULT gst_audiobuff2csv_debug_category

/* prototypes */


static void gst_audiobuff2csv_set_property (GObject * object,
    guint property_id, const GValue * value, GParamSpec * pspec);
static void gst_audiobuff2csv_get_property (GObject * object,
    guint property_id, GValue * value, GParamSpec * pspec);
static void gst_audiobuff2csv_dispose (GObject * object);
static void gst_audiobuff2csv_finalize (GObject * object);

static gboolean gst_audiobuff2csv_setup (GstAudioFilter * filter,
    const GstAudioInfo * info);
static GstFlowReturn gst_audiobuff2csv_transform (GstBaseTransform * trans,
    GstBuffer * inbuf, GstBuffer * outbuf);
static GstFlowReturn gst_audiobuff2csv_transform_ip (GstBaseTransform * trans,
    GstBuffer * buf);

enum
{
  PROP_0
};

/* pad templates */

static GstStaticPadTemplate gst_audiobuff2csv_src_template =
GST_STATIC_PAD_TEMPLATE ("src",
    GST_PAD_SRC,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS ("audio/x-raw,format=S16LE,rate=[1,max],"
        "channels=[1,max],layout=interleaved")
    );

static GstStaticPadTemplate gst_audiobuff2csv_sink_template =
GST_STATIC_PAD_TEMPLATE ("sink",
    GST_PAD_SINK,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS ("audio/x-raw,format=S16LE,rate=[1,max],"
        "channels=[1,max],layout=interleaved")
    );


/* class initialization */

G_DEFINE_TYPE_WITH_CODE (GstAudiobuff2csv, gst_audiobuff2csv,
    GST_TYPE_AUDIO_FILTER,
    GST_DEBUG_CATEGORY_INIT (gst_audiobuff2csv_debug_category, "audiobuff2csv", 0,
        "debug category for audiobuff2csv element"));

static void
gst_audiobuff2csv_class_init (GstAudiobuff2csvClass * klass)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
  GstBaseTransformClass *base_transform_class =
      GST_BASE_TRANSFORM_CLASS (klass);
  GstAudioFilterClass *audio_filter_class = GST_AUDIO_FILTER_CLASS (klass);

  /* Setting up pads and setting metadata should be moved to
     base_class_init if you intend to subclass this class. */
  gst_element_class_add_static_pad_template (GST_ELEMENT_CLASS (klass),
      &gst_audiobuff2csv_src_template);
  gst_element_class_add_static_pad_template (GST_ELEMENT_CLASS (klass),
      &gst_audiobuff2csv_sink_template);

  gst_element_class_set_static_metadata (GST_ELEMENT_CLASS (klass),
      "Audio buffer to CSV convert plugin", "Generic", "Audio buffer to CSV convert plugin",
      "Tarek NAKKACH <tareknakach@gmail.com>");

  gobject_class->set_property = gst_audiobuff2csv_set_property;
  gobject_class->get_property = gst_audiobuff2csv_get_property;
  gobject_class->dispose = gst_audiobuff2csv_dispose;
  gobject_class->finalize = gst_audiobuff2csv_finalize;
  audio_filter_class->setup = GST_DEBUG_FUNCPTR (gst_audiobuff2csv_setup);
  base_transform_class->transform =
      GST_DEBUG_FUNCPTR (gst_audiobuff2csv_transform);
  base_transform_class->transform_ip =
      GST_DEBUG_FUNCPTR (gst_audiobuff2csv_transform_ip);

}

static void
gst_audiobuff2csv_init (GstAudiobuff2csv * audiobuff2csv)
{
}

void
gst_audiobuff2csv_set_property (GObject * object, guint property_id,
    const GValue * value, GParamSpec * pspec)
{
  GstAudiobuff2csv *audiobuff2csv = GST_AUDIOBUFF2CSV (object);

  GST_DEBUG_OBJECT (audiobuff2csv, "set_property");

  switch (property_id) {
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
  }
}

void
gst_audiobuff2csv_get_property (GObject * object, guint property_id,
    GValue * value, GParamSpec * pspec)
{
  GstAudiobuff2csv *audiobuff2csv = GST_AUDIOBUFF2CSV (object);

  GST_DEBUG_OBJECT (audiobuff2csv, "get_property");

  switch (property_id) {
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
  }
}

void
gst_audiobuff2csv_dispose (GObject * object)
{
  GstAudiobuff2csv *audiobuff2csv = GST_AUDIOBUFF2CSV (object);

  GST_DEBUG_OBJECT (audiobuff2csv, "dispose");

  /* clean up as possible.  may be called multiple times */

  G_OBJECT_CLASS (gst_audiobuff2csv_parent_class)->dispose (object);
}

void
gst_audiobuff2csv_finalize (GObject * object)
{
  GstAudiobuff2csv *audiobuff2csv = GST_AUDIOBUFF2CSV (object);

  GST_DEBUG_OBJECT (audiobuff2csv, "finalize");

  /* clean up object here */

  G_OBJECT_CLASS (gst_audiobuff2csv_parent_class)->finalize (object);
}

static gboolean
gst_audiobuff2csv_setup (GstAudioFilter * filter, const GstAudioInfo * info)
{
  GstAudiobuff2csv *audiobuff2csv = GST_AUDIOBUFF2CSV (filter);
  GST_DEBUG_OBJECT (audiobuff2csv, "setup");
  
  audiobuff2csv->audioinfo = gst_audio_info_copy(info);

  return TRUE;
}

/* transform */
static GstFlowReturn
gst_audiobuff2csv_transform (GstBaseTransform * trans, GstBuffer * inbuf,
    GstBuffer * outbuf)
{
  GstAudiobuff2csv *audiobuff2csv = GST_AUDIOBUFF2CSV (trans);
  GST_DEBUG_OBJECT (audiobuff2csv, "transform");
  
  GstAudioBuffer audiobuffer;  
  gst_audio_buffer_map(&audiobuffer, audiobuff2csv->audioinfo, inbuf, GST_MAP_READWRITE);  
  gsize n_samples = audiobuffer.n_samples;
  
  gint timestamp =  GST_BUFFER_TIMESTAMP(inbuf);
  gint rate = GST_AUDIO_INFO_RATE(audiobuff2csv->audioinfo);
  gint channels = GST_AUDIO_INFO_CHANNELS(audiobuff2csv->audioinfo);

  GST_WARNING_OBJECT (audiobuff2csv, "audio buffer : timestamp %d, rate %d, channels %d, n_samples %ld", timestamp, rate, channels, n_samples);
  
  gst_audio_buffer_unmap(&audiobuffer);

  return GST_FLOW_OK;
}

static GstFlowReturn
gst_audiobuff2csv_transform_ip (GstBaseTransform * trans, GstBuffer * buf)
{
  GstAudiobuff2csv *audiobuff2csv = GST_AUDIOBUFF2CSV (trans);

  GST_DEBUG_OBJECT (audiobuff2csv, "transform_ip");

  return GST_FLOW_OK;
}
