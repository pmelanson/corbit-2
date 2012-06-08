#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "07";
	static const char MONTH[] = "06";
	static const char YEAR[] = "2012";
	static const double UBUNTU_VERSION_STYLE = 12.06;
	
	//Software Status
	static const char STATUS[] = "Beta";
	static const char STATUS_SHORT[] = "b";
	
	//Standard Version Type
	static const long MAJOR = 1;
	static const long MINOR = 0;
	static const long BUILD = 27;
	static const long REVISION = 176;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 2197;
	#define RC_FILEVERSION 1,0,27,176
	#define RC_FILEVERSION_STRING "1, 0, 27, 176\0"
	static const char FULLVERSION_STRING[] = "1.0.27.176";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 9;
	

}
#endif //VERSION_h
