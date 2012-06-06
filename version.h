#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "05";
	static const char MONTH[] = "06";
	static const char YEAR[] = "2012";
	static const double UBUNTU_VERSION_STYLE = 12.06;
	
	//Software Status
	static const char STATUS[] = "Beta";
	static const char STATUS_SHORT[] = "b";
	
	//Standard Version Type
	static const long MAJOR = 1;
	static const long MINOR = 0;
	static const long BUILD = 25;
	static const long REVISION = 173;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 1833;
	#define RC_FILEVERSION 1,0,25,173
	#define RC_FILEVERSION_STRING "1, 0, 25, 173\0"
	static const char FULLVERSION_STRING[] = "1.0.25.173";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 7;
	

}
#endif //VERSION_h
