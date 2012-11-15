#ifndef VERSION_HPP
#define VERSION_HPP

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "14";
	static const char MONTH[] = "11";
	static const char YEAR[] = "2012";
	static const char UBUNTU_VERSION_STYLE[] = "12.11";
	
	//Software Status
	static const char STATUS[] = "Beta";
	static const char STATUS_SHORT[] = "b";
	
	//Standard Version Type
	static const long MAJOR = 2;
	static const long MINOR = 0;
	static const long BUILD = 5;
	static const long REVISION = 35824;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 2857;
	#define RC_FILEVERSION 2,0,5,35824
	#define RC_FILEVERSION_STRING "2, 0, 5, 35824\0"
	static const char FULLVERSION_STRING[] = "2.0.5.35824";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 11;
	

}
#endif //VERSION_HPP
