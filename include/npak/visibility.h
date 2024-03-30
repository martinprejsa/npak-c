#ifndef NPAK_API
    #if defined _WIN32 || defined __CYGWIN__
      #ifdef BUILDING_DLL
        #ifdef __GNUC__
          #define NPAK_API __attribute__ ((dllexport))
        #else
          #define NPAK_API __declspec(dllexport) // Note: actually gcc seems to also supports this syntax.
        #endif
      #else
        #ifdef __GNUC__
          #define NPAK_API __attribute__ ((dllimport))
        #else
          #define NPAK_API __declspec(dllimport) // Note: actually gcc seems to also supports this syntax.
        #endif
      #endif
      #define NPAK_INTERNAL
    #else
      #if __GNUC__ >= 4
        #define NPAK_API __attribute__ ((visibility ("default")))
        #define NPAK_INTERNAL  __attribute__ ((visibility ("hidden")))
      #else
        #define NPAK_API
        #define NPAK_INTERNAL
      #endif
    #endif
#endif

