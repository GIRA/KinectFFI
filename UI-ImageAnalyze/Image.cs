using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace WindowsFormsApplication1
{
    class Image
    {
        [DllImport("ImageAnalyzer", CallingConvention = CallingConvention.Cdecl)]
        public static extern int setHue(double min,double max);

        [DllImport("ImageAnalyzer", CallingConvention = CallingConvention.Cdecl)]
        public static extern int setSaturation(double min, double max);

        [DllImport("ImageAnalyzer", CallingConvention = CallingConvention.Cdecl)]
        public static extern int setBrightness(double min, double max);
   
        [DllImport("ImageAnalyzer", CallingConvention = CallingConvention.Cdecl)]
        public unsafe static extern Rect trackMainRectangle(ushort* a, int w, int h);


        [DllImport("ImageAnalyzer", CallingConvention = CallingConvention.Cdecl)]
        public static extern int getRowSkip();
        [DllImport("ImageAnalyzer", CallingConvention = CallingConvention.Cdecl)]
        public static extern int setRowSkip(int value);

        [DllImport("ImageAnalyzer", CallingConvention = CallingConvention.Cdecl)]
        public static extern int getColumnSkip();
        [DllImport("ImageAnalyzer", CallingConvention = CallingConvention.Cdecl)]
        public static extern int setColumnSkip(int value);

        [DllImport("ImageAnalyzer", CallingConvention = CallingConvention.Cdecl)]
        public static extern int getRectangleOverlapping();
        [DllImport("ImageAnalyzer", CallingConvention = CallingConvention.Cdecl)]
        public static extern int setRectangleOverlapping(int value);
      
    }
    struct Rect
    {
        int left;
        int top;
        int right;
        int bottom;
        int centerX;
        int centerY;
    }
}
