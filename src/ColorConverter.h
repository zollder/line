
/*
 * Implemented based on the examples
 * 	from here: http://shervinemami.info/colorConversion.html
 * 	and here: http://cs.haifa.ac.il/hagit/courses/ist/Lectures/Demos/ColorApplet2/t_convert.html
 */

#include <cv.h>

#ifndef colorconverter_h
#define colorconverter_h

using namespace std;


/*
 * ColorConverter class definition.
 * Is a helper class.
 * Implements improved color conversion between RGB and HSV formats.
 */
class ColorConverter
{
	//-----------------------------------------------------------------------------------------
	// Public members
	//-----------------------------------------------------------------------------------------
	public:

		// constructor
		ColorConverter();

		// destructor
		~ColorConverter();

		/**------------------------------------------------------------------------------------
		 * Converts RGB to HSV.
		 * Allocates memory and for the target image that has to be freed-up.
		 * Uses full 8-bits, since OpenCV only allows Hues up to 180 instead of 255.
		 --------------------------------------------------------------------------------------*/
		IplImage* convertRgbToHsv(const IplImage *rgbImage_p);

		/**------------------------------------------------------------------------------------
		 * Converts HSV to RGB.
		 * Allocates memory for the target image that has to be freed-up.
		 * Uses full 8-bits, since OpenCV only allows Hues up to 180 instead of 255.
		 --------------------------------------------------------------------------------------*/
		IplImage* convertHsvToRgb(const IplImage *hsvImage_p);
};

#endif
