////////////////////////////////////////////////////////////////////////////////
// version.h
//
////////////////////////////////////////////////////////////////////////////////
#ifndef VERSION_H_
#define VERSION_H_

////////////////////////////////////////////////////////////////////////////////
// Version Definion
//

#define APPLICATION_MAJOR					0
#define APPLICATION_MINOR					0
#define APPLICATION_PATCH					0
#define APPLICATION_BUILD					3


////////////////////////////////////////////////////////////////////////////////

#define STR(X) #X
#define SW_VERSION(v1, v2, v3, v4)  STR(v1) "." STR(v2) "." STR(v3) "." STR(v4) ""

#define VERSION_STR  SW_VERSION(APPLICATION_MAJOR,APPLICATION_MINOR,APPLICATION_PATCH,APPLICATION_BUILD)

#define VERSION_BCD_SHORT  ((APPLICATION_MAJOR<<12)|(APPLICATION_MINOR<<8)|(APPLICATION_PATCH<<4)|APPLICATION_BUILD)

#define VERSION_LONG  	(APPLICATION_MAJOR<<24)|(APPLICATION_MINOR<<16)|(APPLICATION_PATCH<<8)|APPLICATION_BUILD)

// #define COPYRIGHT "(c) 2022 - Chistoph Schulz"
#define COPYRIGHT "(c)"

#endif 
//
////////////////////////////////////////////////////////////////////////////////