
/*

  iqlord | Legion2000 Security Research

  Modify the linker to minimize bin size
   - building your patch as 'release' and using
     this tweak, you should (WITH an included icon)
     get a patch bin size of approx. 3072 byte.

*/

#ifndef _DEBUG

#pragma comment(linker,"/ENTRY:main")            // set entry main

#pragma optimize("gsy",on)                       // gsy

#pragma comment(linker,"/MERGE:.rdata=.data")    // merge sections
#pragma comment(linker,"/MERGE:.text=.data")     // merge sections
//#pragma comment(linker,"/MERGE:.reloc=.data")    // merge sections

// check compiler version
#if (_MSC_VER < 1300)
    #pragma comment(linker,"/RELEASE")           // release
    #pragma comment(linker,"/IGNORE:4078")       // ignore
    #pragma comment(linker,"/OPT:NOWIN98")       // force 512 byte section alignment (/FILEALIGN:512)
#endif

//#pragma comment(linker,"/nodefaultlib:libc.lib") // skip standard c library
//#pragma comment(linker,"/defaultlib:msvcrt.lib") // use dynamic msvcrt instead
#define WIN32_LEAN_AND_MEAN                      // reduce size of precompiled header

#endif
