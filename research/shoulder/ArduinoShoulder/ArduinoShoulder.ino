/**
 * Replicating shoulder movement with stretch sensors
 * 
 * 2014-12-16
 * 
 * 
 */

// Raw data from sensors
// [rawValue, minValue, maxValue]
int rawTop[3]	= {0, 0, 1000};
int rawBack[3]	= {0, 0, 1000};
int rawBody[3]	= {0, 0, 1000};

void setup()
{
  
}

void loop()
{
  
}

float getUpward(int rawTop, int rawBack, int rawBody)
{
	// 
	float filtered = 0.0;
	float top = getPercent(rawTop[0], rawTop[1], rawTop[2]) * 1000.0;
	float back = getPercent(rawBack[0], rawBack[1], rawBack[2]) * 1000.0;
	float body = getPercent(rawBody[0], rawBody[1], rawBody[2]) * 1000.0;

	// Threshold above which to ignore top sensor data (out of 1,000)
	int bodyThreshold = 100;

	// Threshold above which to ignore body sensor data (out of 1,000)
	int topThreshold = 100;

	// Relying only on body sensor
	if (body > bodyThreshold) {
		filtered = (body - 100.0) + 1000.0;
	}

	// Relying only on top sensor
	else if (top > topThreshold) {
		filtered = 1000.0 - (top + 100.0);
	}

	// 
	else
	{
		filtered = 1000.0;
	}

	return filtered / 2.0;
}

float getForward(int rawTop, int rawBack, int rawBody)
{
	float filtered = getPercent(rawBack[0], rawBack[1], rawBack[2]) * 1000.0;

	return filtered;
}

float getPercent(int val, int min, int max)
{
	// Change reference (min value) to zero
	if (min < 0) {
		val += abs(min);
		max += abs(min);
		min = 0;
	} else if (min > 0) {
		val -= min;
		max -= min;
		min = 0;
	}

	// Check that value is not below min
	if (val < 0) {
		return 0.0;
	}

	// Return a float, as a percentage of "max"
	float percent = val / max;

	return percent;
}

