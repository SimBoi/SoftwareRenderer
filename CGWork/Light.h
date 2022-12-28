#ifndef CG_LIGHT
#define CG_LIGHT

#include "CG_Buffer.h"
#include "CG_Object.h"

typedef enum
{
    LIGHT_ID_AMBIENT=-1,
    LIGHT_ID_1=0,
    LIGHT_ID_2,
    LIGHT_ID_3,
    LIGHT_ID_4,
    LIGHT_ID_5,
    LIGHT_ID_6,
    LIGHT_ID_7,
    LIGHT_ID_8,
    MAX_LIGHT
} LightID;

typedef  enum
{
    LIGHT_TYPE_DIRECTIONAL,
    LIGHT_TYPE_POINT,
    LIGHT_TYPE_SPOT
} LightType;

typedef  enum
{
    LIGHT_SPACE_VIEW,
    LIGHT_SPACE_LOCAL
} LightSpace;


class LightParams
{
public:

    //light enabled
    bool enabled;
    //type directional,point,spot
    LightType type;
    //local or view space
    LightSpace space;

    //color 0-255 RGB
    int colorR;
    int colorG;
    int colorB;

    //position
    double posX;
    double posY;
    double posZ;

    //direction
    double dirX;
    double dirY;
    double dirZ;

    double spotLightAngle; // for spot light only

    //light intensity
    double ambientIntensity; // for ambient light only
    double diffuseIntensity;
    double specularIntensity;

    CG::Camera directionalPerspective;
    CG::ZBuffer directionalBuffer;
    CG::Camera pointPerspective[6];
    CG::ZBuffer pointBuffer[6];
    int shadowNearPlane, shadowFarPlane;

    void CalculatePerspective()
    {
        if (type == LIGHT_TYPE_DIRECTIONAL)
        {
            CG::vec4 pos(posX, posY, posZ, 1);
            CG::vec4 dir(dirX, dirY, dirZ);
            directionalPerspective.LookAt(pos, pos + dir, CG::vec4(0, 1, 0));
            directionalPerspective.Ortho(-1000, 1000, -1000, 1000, shadowNearPlane, shadowFarPlane);
        }
        else
        {

        }
    }

    bool isValidLight();

    LightParams():
	    enabled(false),type(LIGHT_TYPE_DIRECTIONAL),space(LIGHT_SPACE_VIEW),
	    colorR(255),colorG(255),colorB(255),posX(0),posY(0),posZ(0),
	    dirX(0),dirY(0),dirZ(0),
        spotLightAngle(20),
        ambientIntensity(0.2),
        diffuseIntensity(0.8),
        specularIntensity(1),
        shadowNearPlane(10),
        shadowFarPlane(1000)
    {}

protected:
private:
};

#endif