#include <stdlib.h>
#include <string.h>

#include <math.h>

#include "lv2.h"

#define AMP_URI       "https://lv2.naptastic.com/nap-smash.lv2";
#define AMP_INPUT     0
#define AMP_OUTPUT    1

static LV2_Descriptor *ampDescriptor = NULL;

typedef struct {
	float *input;
	float *output;
} Amp;

static void cleanupAmp(LV2_Handle instance)
{
	free(instance);
}

static void connectPortAmp(LV2_Handle instance, uint32_t port,
			   void *data)
{
	Amp *plugin = (Amp *)instance;

	switch (port) {
	case AMP_INPUT:
		plugin->input = data;
		break;
	case AMP_OUTPUT:
		plugin->output = data;
		break;
	}
}

static LV2_Handle instantiateAmp(const LV2_Descriptor *descriptor,
	    double s_rate, const char *path,
	    const LV2_Feature * const* features)
{
	Amp *plugin_data = (Amp *)malloc(sizeof(Amp));

	return (LV2_Handle)plugin_data;
}

#define DB_CO(g) ((g) > -90.0f ? powf(10.0f, (g) * 0.05f) : 0.0f)

static void runAmp(LV2_Handle instance, uint32_t sample_count)
{
	Amp *plugin_data = (Amp *)instance;

	const float * const input = plugin_data->input;
	float * const output = plugin_data->output;

	int pos;
    float amp;

	for (pos = 0; pos < sample_count; pos++) {
        amp = input[pos] * 4;
        amp = amp - pow( amp, 3 ) / 3;
        output[pos] = amp;
	}
}

static void init()
{
	ampDescriptor =
	 (LV2_Descriptor *)malloc(sizeof(LV2_Descriptor));

	ampDescriptor->URI = AMP_URI;
	ampDescriptor->activate = NULL;
	ampDescriptor->cleanup = cleanupAmp;
	ampDescriptor->connect_port = connectPortAmp;
	ampDescriptor->deactivate = NULL;
	ampDescriptor->instantiate = instantiateAmp;
	ampDescriptor->run = runAmp;
	ampDescriptor->extension_data = NULL;
}

LV2_SYMBOL_EXPORT
const LV2_Descriptor *lv2_descriptor(uint32_t index)
{
	if (!ampDescriptor) init();

	switch (index) {
	case 0:
		return ampDescriptor;
	default:
		return NULL;
	}
}
