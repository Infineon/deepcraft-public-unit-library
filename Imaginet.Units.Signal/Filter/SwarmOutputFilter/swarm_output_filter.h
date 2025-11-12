#pragma IMAGINET_INCLUDES_BEGIN
#include <stdint.h>
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "swarm_output_filter_f32"

static inline void swarm_output_filter_f32(const float* restrict input, float* restrict output, int count, float confidence_threshold, int num_triggers, int filter_length, int filter_cooloff)
{
	static int triggers = 0;
	static int timer = 0;
	static int cooloff = 0;
	static int last_trigger_signal = -1;

	// If timer is expired, reset triggers
	if (timer <= 0)
	{
		triggers = 0;
	}

	// Loop through the input signals
	for (int i = 1; i < count; ++i) // skip input[0] as this is the "unknown/unlabelled/null" class
	{
		// Check if signal surpasses confidence threshold
		if (input[i] > confidence_threshold)
		{
			timer = filter_length;

			// Check if cooloff period has expired
			if (cooloff <= 0)
			{
				// Reset triggers if new signal is different from last one
				if (last_trigger_signal != i)
					triggers = 0;

				// Increment triggers and reset cooloff period
				triggers++;
				last_trigger_signal = i;
				cooloff = filter_cooloff;
			}
		}
	}

	// Decrement timers if they are active
	if (cooloff > 0) cooloff--;
	if (timer > 0) timer--;

	// Set output based on triggers and reset if necessary
	*output = (triggers >= num_triggers) ? 1.0f : 0.0f;
	if (*output == 1.0f)
		triggers = timer = 0;
}

#pragma IMAGINET_FRAGMENT_END