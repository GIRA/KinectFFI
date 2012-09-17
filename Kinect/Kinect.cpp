// Archivo DLL principal.

#include "stdafx.h"
#include <Windows.h>
#include "NuiApi.h"

extern "C"
{
	bool initialized=false;
	__declspec(dllexport) int getKinectCount()
	{
		int numDevices = 0;
		HRESULT hr = NuiGetSensorCount(&numDevices);
		if ( FAILED(hr) )
		{
			return 0;
		}
		else
		{
			return numDevices;
		}
	};
	
	INuiSensor *_current = NULL;
    
	NUI_SKELETON_DATA* data;
	void createArray()
	{ data = new NUI_SKELETON_DATA[6];}
	
	void deleteArray()
	{ delete [] data;}

	__declspec(dllexport) void uninitialize()
	{
		if(_current!=NULL)
		{
			initialized=false;
			_current -> NuiShutdown();
			_current -> Release();
			_current = NULL;
			deleteArray();
		}
	}

	__declspec(dllexport) void initializeOn(int index)
	{
		/*this returns the pointer of a INuiSensor, that has all the methods present at nuiApi, 
		but for a specific instace of kinect*/
	    HRESULT hr = NuiCreateSensorByIndex(index,  &_current);
		/*for now i'll only initialize some of the functions coz i say so.*/
		createArray();

		if(SUCCEEDED(hr))
		{  
			DWORD nuiFlags = NUI_INITIALIZE_FLAG_USES_DEPTH_AND_PLAYER_INDEX | NUI_INITIALIZE_FLAG_USES_SKELETON |  NUI_INITIALIZE_FLAG_USES_COLOR;
			_current -> NuiInitialize(nuiFlags);
			
			_current -> NuiSkeletonTrackingEnable(NULL, NUI_SKELETON_TRACKING_FLAG_SUPPRESS_NO_FRAME_DATA | NUI_SKELETON_TRACKING_FLAG_ENABLE_IN_NEAR_RANGE);
			DWORD ID[5] = {0,1,3,4,5};
			_current -> NuiSkeletonSetTrackedSkeletons(ID);
			initialized=true;
		}
	};

	__declspec(dllexport) void setAngle(long angle)
	{
		if(initialized && angle <= 27 && angle >= -27)
		{
			_current -> NuiCameraElevationSetAngle(angle);
			
		}
	}

	__declspec(dllexport) long getAngle()
	{
		if(initialized)
		{
			long temp=0;
		 _current ->NuiCameraElevationGetAngle(&temp);
		 return temp;
		}
		return 0;
	}

	const NUI_TRANSFORM_SMOOTH_PARAMETERS DefaultParams = 
{0.5f, 0.5f, 0.5f, 0.05f, 0.04f};
	const NUI_TRANSFORM_SMOOTH_PARAMETERS SomewhatLatentParams = 
{0.5f, 0.1f, 0.5f, 0.1f, 0.1f};
	const NUI_TRANSFORM_SMOOTH_PARAMETERS VerySmoothParams = 
{0.7f, 0.3f, 1.0f, 1.0f, 1.0f};



	__declspec(dllexport) NUI_SKELETON_DATA* getNextFrame()
	{
		 NUI_SKELETON_FRAME pSkeletonFrame;
		 
		 memset(data, 0, sizeof(data));
		 HRESULT t = NuiSkeletonGetNextFrame(30,&pSkeletonFrame);
		 int index=0;
		 int count=0;
		if(t == 0)
		{
			_current ->NuiTransformSmooth(&pSkeletonFrame,&DefaultParams);
			for(int i = 0 ;i <6 ; i++)
			{
				/*for each skeleton*/
				NUI_SKELETON_DATA  sd = pSkeletonFrame.SkeletonData[i];
				if(sd.eTrackingState !=0)
				{
					int a = sizeof(sd.dwTrackingID);
					data[count]=sd;
					count++;
					
				/*	*(address+index) = (float)(sd.dwTrackingID);
					index++;
					*(address+index) = (float)(sd.eTrackingState);
			
					index++;
					for(int j=0;j<20;j++)
					{
						Vector4 position = sd.SkeletonPositions[j];
						*(address+index) = position.x;
						index++;
						*(address+index) = position.y;
						index++;
						*(address+index) = position.z;
						index++;
						*(address+index) = position.w;
						index++;
					}*/
				}
			}
		}
		return data;
	}

}

