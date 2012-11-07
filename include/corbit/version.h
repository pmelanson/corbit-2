#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{

	//Date Version Types
	static const char DATE[] = "06";
	static const char MONTH[] = "11";
	static const char YEAR[] = "2012";
	static const char UBUNTU_VERSION_STYLE[] = "12.11";

	//Software Status
	static const char STATUS[] = "Beta";
	static const char STATUS_SHORT[] = "b";

	//Standard Version Type
	static const long MAJOR = 2;
	static const long MINOR = 0;
	static const long BUILD = 1;
	static const long REVISION = 35795;

	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 2772;
	#define RC_FILEVERSION 2,0,1,35795
	#define RC_FILEVERSION_STRING "2, 0, 1, 35795\0"
	static const char FULLVERSION_STRING[] = "2.0.1.35795";

	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 11;


}
#endif //VERSION_H
