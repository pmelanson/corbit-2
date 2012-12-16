#ifndef VERSION_HPP
#define VERSION_HPP

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "10";
	static const char MONTH[] = "12";
	static const char YEAR[] = "2012";
	static const char UBUNTU_VERSION_STYLE[] = "12.12";
	
	//Software Status
	static const char STATUS[] = "Beta";
	static const char STATUS_SHORT[] = "b";
	
	//Standard Version Type
	static const long MAJOR = 2;
	static const long MINOR = 0;
	static const long BUILD = 8;
	static const long REVISION = 35833;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 3198;
	#define RC_FILEVERSION 2,0,8,35833
	#define RC_FILEVERSION_STRING "2, 0, 8, 35833\0"
	static const char FULLVERSION_STRING[] = "2.0.8.35833";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 11;
	

}
#endif //VERSION_HPP
