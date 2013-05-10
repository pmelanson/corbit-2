#ifndef VERSION_HPP
#define VERSION_HPP

namespace AutoVersion{
	
	//Date Version Types
	static const char  _DATE[] = "10";
	static const char  _MONTH[] = "05";
	static const char  _YEAR[] = "2013";
	static const char  _UBUNTU_VERSION_STYLE[] = "13.05";
	
	//Software Status
	static const char  _STATUS[] = "Beta";
	static const char  _STATUS_SHORT[] = "b";
	
	//Standard Version Type
	static const long  _MAJOR = 2;
	static const long  _MINOR = 0;
	static const long  _BUILD = 10;
	static const long  _REVISION = 35849;
	
	//Miscellaneous Version Types
	static const long  _BUILDS_COUNT = 3198;
	#define  _RC_FILEVERSION 2,0,10,35849
	#define  _RC_FILEVERSION_STRING "2, 0, 10, 35849\0"
	static const char  _FULLVERSION_STRING[] = "2.0.10.35849";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long  _BUILD_HISTORY = 0;
	

}
#endif //VERSION_HPP
