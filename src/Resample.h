#ifndef RESAMPLE_H_
#define RESAMPLE_H_
typedef std::string String;
std::map<String,String> ResampleImageGeotiffs( std::map<String,String>& );
String ResampleImageFile(const char*,const char*);
#endif
