#ifndef _SC_HELPERS_H
#define _SC_HELPERS_H

#include <string>

using namespace std;

string PythonModuleName(const string& filename);
string PythonImplName(const string& filename);
string PythonPackageName(const string& filename);
string StripProto(const string& filename);
string StripPath(const string& filename);

#endif  // _SC_HELPERS_H
