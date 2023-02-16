#ifndef _GST_AUDIOBUF2CSV_H_
#define _GST_AUDIOBUF2CSV_H_

#include <gst/audio/gstaudiofilter.h>
#include <gst/audio/audio-buffer.h>

G_BEGIN_DECLS

#define GST_TYPE_AUDIOBUFF2CSV   (gst_audiobuff2csv_get_type())
#define GST_AUDIOBUFF2CSV(obj)   (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_AUDIOBUFF2CSV,GstAudiobuff2csv))
#define GST_AUDIOBUFF2CSV_CLASS(klass)   (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_AUDIOBUFF2CSV,GstAudiobuff2csvClass))
#define GST_IS_AUDIOBUFF2CSV(obj)   (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_AUDIOBUFF2CSV))
#define GST_IS_AUDIOBUFF2CSV_CLASS(obj)   (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_AUDIOBUFF2CSV))

typedef struct _GstAudiobuff2csv GstAudiobuff2csv;
typedef struct _GstAudiobuff2csvClass GstAudiobuff2csvClass;

struct _GstAudiobuff2csv
{
  GstAudioFilter base_audiobuff2csv;
  GstAudioInfo *audioinfo;
};

struct _GstAudiobuff2csvClass
{
  GstAudioFilterClass base_audiobuff2csv_class;
};

GType gst_audiobuff2csv_get_type (void);

G_END_DECLS

#endif
