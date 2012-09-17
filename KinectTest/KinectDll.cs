using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace KinectTest
{
    class KinectDll
    {

        [DllImport("Kinect", CallingConvention = CallingConvention.Cdecl)]
        public static extern int getKinectCount();

        [DllImport("Kinect", CallingConvention = CallingConvention.Cdecl)]
        public static extern void initializeOn(int index);

        [DllImport("Kinect", CallingConvention = CallingConvention.Cdecl)]
        public static extern void setAngle(long index);

        [DllImport("Kinect", CallingConvention = CallingConvention.Cdecl)]
        public static extern int getAngle();

        [DllImport("Kinect", CallingConvention = CallingConvention.Cdecl)]
        public static extern int getNextFrame();
        
        [DllImport("Kinect", CallingConvention = CallingConvention.Cdecl)]
        public static extern void uninitialize();
    }
}
