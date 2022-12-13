#include "Light.h"

bool LightParams::isValidLight()
{
	if (!(diffuseIntensity >= 0.0 && diffuseIntensity <= 1.0))
		return false;

	if (!(specularIntensity >= 0.0 && specularIntensity <= 1.0))
		return false;

	if (!(colorR >= 0 && colorR <= 255))
		return false;

	if (!(colorG >= 0 && colorG <= 255))
		return false;

	if (!(colorB >= 0 && colorB <= 255))
		return false;
}