#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char  _DATE[] = "15";
	static const char  _MONTH[] = "05";
	static const char  _YEAR[] = "2013";
	static const char  _UBUNTU_VERSION_STYLE[] = "13.05";
	
	//Software Status
	static const char  _STATUS[] = "Alpha";
	static const char  _STATUS_SHORT[] = "a";
	
	//Standard Version Type
	static const long  _MAJOR = 1;
	static const long  _MINOR = 0;
	static const long  _BUILD = 1;
	static const long  _REVISION = 1;
	
	//Miscellaneous Version Types
	static const long  _BUILDS_COUNT = 54;
	#define  _RC_FILEVERSION 1,0,1,1
	#define  _RC_FILEVERSION_STRING "1, 0, 1, 1\0"
	static const char  _FULLVERSION_STRING[] = "1.0.1.1";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long  _BUILD_HISTORY = 0;
	

}
#endif //VERSION_H
