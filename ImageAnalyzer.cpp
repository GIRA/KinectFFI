// Archivo DLL principal.

#include "stdafx.h"
#include "stdlib.h"
#include "math.h"
#include "ImageAnalyzer.h"
extern "C"
{
	//deberia funcionar para imagenes con 16 bits de depth

	double minHue=0;
	double maxHue=360;

	double minSat=0;
	double maxSat=1.0;


	double minV=0;
	double maxV=1.0;


	double OminHue=0;
	double OmaxHue=360;

	double OminSat=0;
	double OmaxSat=1.0;


	double OminV=0;
	double OmaxV=1.0;

	struct rectangle
	{
		int left;
		int top;
		int right;
		int bottom;
		int centerX;
		int centerY;
	};

	struct hsv
	{
		double h;/*0 to 360*/
		double s;/*0 to 1.0*/
		double v;/*0 to 1.0*/
	};
	
	struct rgb
	{
		double r;/*0 to 1.0*/
		double g;/*0 to 1.0*/
		double b;/*0 to 1.0*/
	};
	
	double getMax(double a,double b, double c)
	{
		double max = a;
		if(max < b) max=b;
		if (max < c) max = c;
		return max;
	}
	
	double getMin(double a, double b, double c)
	{
		double min = a;
		if(min>b)min=b;
		if(min>c)min=c;
		return min;
	}
	
	
	hsv getHsv(rgb color)
	{
		struct hsv ret;
		double max, min,dif;
		double r,g,b;
		r=color.r;
		g=color.g;
		b=color.b;
		
		max =  getMax(r,g,b); 	
	    min =  getMin(r,g,b);	
		dif=max-min;
		ret.v = max;
	
		if(ret.v==0)
		{
			ret.h=ret.s=0;
			return ret;
		}		 
		ret.s = (max - min)/ret.v;
	    
		if (ret.s == 0) 
		{
			ret.h= 0;
			return ret;
		}else{
			if ( r == max)
			{
				ret.h = 60 * (( g -  b) / dif);
				
			if (ret.h < 0) ret.h += 360;
			}
			else
			{
				if ( g == max)
				{
					ret.h = 120+ (60 * (( b- r) / dif));
				}
				else
				{
					ret.h = 240 + (60 * (( r -  g) / dif));
				}
			}
		}
		return ret;
	}
	
	__declspec(dllexport) bool isValid(int,int,int);
	
	
	
	
	__declspec(dllexport) void RGBtoHSV(double r, double g, double b, double *h, double *s ,double *v)
	{
		struct rgb color;
		color.r=r;
		color.g=g;
		color.b=b;
		
		struct hsv res = getHsv(color);
		(*h)=res.h;
		(*s)=res.s;
		(*v)=res.v;
	}
	
	int rowSkip=0;
	__declspec(dllexport) void setRowSkip(int value)
	{
		if(rowSkip>=0)
		{
			rowSkip=value;
		}
	}
	__declspec(dllexport) int getRowSkip()
	{return rowSkip;}

	int columnSkip=1;
		__declspec(dllexport) void setColumnSkip(int value)
	{
		if(columnSkip>=0)
		{
			columnSkip=value;
		}
	}
	__declspec(dllexport) int getColumnSkip()
	{
		return columnSkip;
	}
	/*this returns the number of rectangles found in the image a with the width and height specified, and puts those rectangles
	as an array on results
	*/

	int rectangleOverlapping=20;

	__declspec(dllexport) void setRectangleOverlapping(int value)
	{
		if(rectangleOverlapping>=3)
		{
			rectangleOverlapping=value;
		}
	}
	__declspec(dllexport) int getrectangleOverlapping()
	{return rectangleOverlapping;}


	__declspec(dllexport) int findAllRectangles(unsigned short* a, int width,int height,int* results )
	{
		//a is the image's first pixel pointer
		if(minHue!=0 || maxHue !=360){
			int* pointsFoundPointer =(int*) malloc(width * height * 2*sizeof(int)); ;


			int rectFound=0;
			int pointsFound=0;
			int length = width*height;
			int px,py;
			px= py=0;
			int i =0;
				for(int iy=0; iy<height;iy+= 1 + columnSkip)
				{
						for(int ix=0;ix<width;ix+= 1+ rowSkip)
						{
							i= ix + (iy * width);
							unsigned short value = *(a+i);
							int r,g,b;
							b= value & 31;
							g= (value >>5) & 31;
							r= (value>>10) & 31;
							if(!isValid(r,g,b))
							{
								//*(a+i)=0;
								//i es el indice de pixel que estoy trabajando
								//los pixeles vienen de la siguiente manera:
								/*
								1 2 3			
								4 5 6
								7 8 9
								*/
							}else{
								px+=ix;
								py+=iy;
								*(pointsFoundPointer+(pointsFound*2))=ix;
								*(pointsFoundPointer +1 +(pointsFound*2))=iy;
								pointsFound++;
							}
						}
				}
			
			
			struct rectangle* rects = (struct rectangle*) malloc(width * height * 6*sizeof(int));

			if(pointsFound >0)
			{
				px/=pointsFound;
				py/=pointsFound;
				int top,left;
				top = height;
				left = width;
				for(int i=0;i<pointsFound;i+=2)
				{
	
					int x = *(pointsFoundPointer+(i*2)) ;
					int y = *(pointsFoundPointer+(i*2)+1) ;
					bool found=false;
					for(int j = 0 ; j< rectFound;j++)
					{
						struct rectangle rect = *(rects+ (j*6));

						double dx =x - rect.centerX;
						double dy = y-   rect.centerY;

						if(sqrt(pow(dx ,2)+ pow(dy,2))<rectangleOverlapping)
						{
							if(x <= rect.left) rect.left = x;
							if(y <= rect.top) rect.top= y;
							if(x >= rect.right) rect.right = x;
							if(y >= rect.bottom) rect.bottom= y;
							
							rect.centerX =   (rect.right-  rect.left)/2 + rect.left;
							rect.centerY=  (rect.bottom-  rect.top)/2 + rect.top;
							*(rects+ (j*6)) = rect;
							found=true;
						}
					}
	
					if(!found)
					{
						struct rectangle newRectangle  ;
						newRectangle.top=y;
						newRectangle.centerY=y;
						newRectangle.left=x;
						newRectangle.centerX=x;
						newRectangle.bottom=newRectangle.top;
						newRectangle.right=newRectangle.left;
						*(rects +(rectFound *6))= newRectangle;
						rectFound+=1;
					}
				
				}
			}
			free(pointsFoundPointer);
			/*ahora deberia comprimir los rectangulos ya que se sobreponen y me asegure de eso*/
	
	
				int n=0;
				int j=0;
			
			bool unchanged = true;
			do{		
				unchanged=false;
	  				j++;
					if(j>=rectFound)
					{
						n++;
						j=n+1;
					}
					if(j<rectFound){
						struct rectangle r1 = *(rects + (n*6));
						struct rectangle r2 = *(rects + (j*6));
						if((r1.left>=r2.left && r1.left<=r2.right )|| (r1.right>=r2.left && r1.right<=r2.right) || (r1.left<=r2.left && r1.right>=r2.right) || (r1.left>=r2.left && r1.right<=r2.right))
						{
						//mi Left esta entre su left y su right, o mi right esta entre su left y su right
							if((r1.top>=r2.top && r1.top<=r2.bottom)|| (r1.bottom>=r2.top&& r1.bottom<=r2.bottom) || (r1.bottom>=r2.bottom&& r1.top<=r2.top) || (r1.bottom<=r2.bottom&& r1.top>=r2.top))
							{
								//mi top esta entre su top y su bottom, o mi bottom esta entre su top y su bottom
								//INTERSECTA!
							
								if(r2.left <=   r1.left)   r1.left = r2.left;
								if(r2.top<=   r1.top)   r1.top= r2.top;
								if(r2.right>=   r1.right)   r1.right = r2.right;
								if(r2.bottom>=   r1.bottom)   r1.bottom= r2.bottom;
									
								r1.centerX =   (r1.right-  r1.left)/2 + r1.left;
								r1.centerY=  (r1.bottom-  r1.top)/2 + r1.top;
								
								*(rects + (n*6))=r1;

								for(int p = j; p<rectFound-1;p++)
								{
									*(rects+(p*6)) = *(rects + ((p+1)*6));
								}
								rectFound --;
								j=0;
								n=0;
								unchanged=false;
							}
						}
	
					}else
					{
						unchanged=true;
					}
	 
			}while(!unchanged);
	
		
			 for(int i =0; i<rectFound*6;i++)
			 {
				 *(results+i)= *( (&(*(rects)).left)+i);
			}	
			
			free(rects);
			return rectFound;
		}
		return 0;
	}

	int getArea(struct rectangle r)
	{return (r.right - r.left) * (r.bottom-r.top);}
	
	void balanceColor(struct hsv color)
	{
		double difH,difS,difV;
		difH= (maxHue-minHue)/2;
		difS=(maxSat-minSat)/2;
		difV=(maxV-minV)/2;

		struct hsv originalColor;
		originalColor.h = OminHue + difH;
		originalColor.s = OminSat + difS;
		originalColor.v = OminV + difV;

		struct hsv newColor;
		newColor.h = (originalColor.h + color.h)/2;
		newColor.s = (originalColor.s + color.s)/2;
		newColor.v = (originalColor.v + color.v)/2;
		//if the color is in the same angle, for example coz it is now brighter i will update the
		//valid ranges. If not i will set the original value
		if( abs ( originalColor.h - newColor.h) < 20)
		{
		minHue = newColor.h-difH;
		maxHue = newColor.h+difH;
		minSat = newColor.s -difS;
		maxSat = newColor.s+difS;
		minV = newColor.v - difV;
		maxV = newColor.v + difV;
		}else{
		minHue = originalColor.h-difH;
		maxHue = originalColor.h+difH;
		minSat = originalColor.s -difS;
		maxSat = originalColor.s+difS;
		minV = originalColor.v - difV;
		maxV = originalColor.v + difV;
		}
	}

	
	//this should return the pointer of the biggest rectangle.
	__declspec(dllexport) struct rectangle trackMainRectangle(unsigned short* a, int width,int height)
	{
		
		struct rectangle foundRects[10000];	
		int size = findAllRectangles(a,width,height,(&foundRects[0].left));
		struct rectangle currentRect = foundRects[0];

			 for(int i =1; i<size;i++)
			 {
				if( getArea(currentRect) < getArea(foundRects[i]))
				{
					currentRect = foundRects[i];
				}
			}
			 /*here i should have iun currentRect the biggest rectangle of the lot.*/
			struct hsv color;
			color.h=0;
			color.s=0;
			color.v=0;
			 if(size!=0){

			 	unsigned short value = *(a+ currentRect.centerX + (currentRect.centerY * (currentRect.right-currentRect.left)));
				int r,g,b;
	
				b= value & 31;
				g= (value >>5) & 31;
				r= (value>>10) & 31;
				
				RGBtoHSV(r,g,b,&color.h,&color.s,&color.v);
				balanceColor(color);
			 }
			 else{
				 if( minHue !=0 && maxHue !=360)
				 {
				 //it was initialized and i did not find anything. Reset color... just in case
				balanceColor(color);
				 }
			 }
		
				 return currentRect;
			
	}




	 bool isValid(int ir,int ig, int ib)
	{
	
		double r = (double)ir/(double)31;
		double g = (double)ig/(double)63;
		double b= (double)ib/(double)31;
		struct rgb color;
		color.r=r;
		color.g=g;
		color.b=b;
	
		struct hsv _hsv= getHsv(color);
	
		return (_hsv.h>=minHue && _hsv.h<=maxHue) && (_hsv.s >= minSat && _hsv.s <= maxSat) && (_hsv.v>=minV && _hsv.v<=maxV);
	}

	  
	__declspec(dllexport) void setHue(double min,double max)
	{
		minHue=OminHue=min;
		maxHue=OmaxHue=max;
	}
	__declspec(dllexport) void setSaturation(double min,double max)
	{
		minSat=OminSat=min;
		maxSat=OmaxSat=max;
	}
	__declspec(dllexport) void setBrightness(double min,double max)
	{
		minV=OminV=min;
		maxV=OmaxV=max;
	}
}
