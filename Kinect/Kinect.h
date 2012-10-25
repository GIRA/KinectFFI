// Kinect.h
#include "Stdafx.h"
#include <Windows.h>
#include "NuiApi.h"

#define EXTERN extern "C" __declspec(dllexport)

INuiSensor *_current = NULL;

const NUI_TRANSFORM_SMOOTH_PARAMETERS DefaultParams = {0.5f, 0.5f, 0.5f, 0.05f, 0.04f};
const NUI_TRANSFORM_SMOOTH_PARAMETERS SomewhatLatentParams = {0.5f, 0.1f, 0.5f, 0.1f, 0.1f};
const NUI_TRANSFORM_SMOOTH_PARAMETERS VerySmoothParams = {0.7f, 0.3f, 1.0f, 1.0f, 1.0f};


EXTERN long getKinectCount(void);
EXTERN void uninitialize(void);
EXTERN void initializeOn(long);
EXTERN void setAngle(long);
EXTERN long getAngle(void);
EXTERN NUI_SKELETON_DATA* getNextFrame(void);



