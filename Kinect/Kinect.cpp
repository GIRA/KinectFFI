#include "Kinect.h"

EXTERN long getKinectCount(void)
{
	int numDevices = 0;
	HRESULT hr = NuiGetSensorCount(&numDevices);
	if (FAILED(hr))
	{
		return 0;
	}
	else
	{
		return numDevices;
	}
}

EXTERN void uninitialize(void)
{
	if(_current != NULL)
	{
		_current -> NuiShutdown();
		_current -> Release();
		_current = NULL;
	}
}

EXTERN void initializeOn(long index)
{
	/*this returns the pointer of a INuiSensor, that has all the methods present at nuiApi, 
	but for a specific instace of kinect*/
    HRESULT hr = NuiCreateSensorByIndex(index,  &_current);
	/*for now i'll only initialize some of the functions coz i say so.*/

	if(SUCCEEDED(hr))
	{  
		DWORD nuiFlags = NUI_INITIALIZE_FLAG_USES_DEPTH_AND_PLAYER_INDEX | NUI_INITIALIZE_FLAG_USES_SKELETON |  NUI_INITIALIZE_FLAG_USES_COLOR;
		_current -> NuiInitialize(nuiFlags);
		
		_current -> NuiSkeletonTrackingEnable(NULL, NUI_SKELETON_TRACKING_FLAG_SUPPRESS_NO_FRAME_DATA | NUI_SKELETON_TRACKING_FLAG_ENABLE_IN_NEAR_RANGE);
		DWORD ID[5] = {0,1,3,4,5};
		_current -> NuiSkeletonSetTrackedSkeletons(ID);			
	}
}

EXTERN void setAngle(long angle)
{
	if(_current != NULL && angle <= 27 && angle >= -27)
	{
		_current -> NuiCameraElevationSetAngle(angle);			
	}
}

EXTERN long getAngle(void)
{
	if(_current != NULL)
	{
		long temp = 0;
		_current -> NuiCameraElevationGetAngle(&temp);
		return temp;
	}
	return 0;
}

EXTERN NUI_SKELETON_DATA* getNextFrame(void)
{
	NUI_SKELETON_FRAME pSkeletonFrame;
	HRESULT hr = NuiSkeletonGetNextFrame(30, &pSkeletonFrame);
	if(SUCCEEDED(hr))
	{
		_current -> NuiTransformSmooth(&pSkeletonFrame, &DefaultParams);
		return pSkeletonFrame.SkeletonData;
	}
	return 0;
}