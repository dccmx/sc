#include <string.h>

#include "sc_helpers.h"

string StripPath(const string& filename) {
  if (filename.find('/') != string::npos) {
    return filename.substr(filename.rfind('/') + 1, filename.size() - filename.rfind('/') - 1);
  }
  return filename;
}

// ----------------------------------------------------------------------
// StripString
//    Replaces any occurrence of the character 'remove' (or the characters
//    in 'remove') with the character 'replacewith'.
// ----------------------------------------------------------------------
void StripString(string* s, const char* remove, char replacewith) {
  const char * str_start = s->c_str();
  const char * str = str_start;
  for (str = strpbrk(str, remove);
       str != NULL;
       str = strpbrk(str + 1, remove)) {
    (*s)[str - str_start] = replacewith;
  }
}

// ----------------------------------------------------------------------
// HasSuffixString()
//    Return true if str ends in suffix.
// StripSuffixString()
//    Given a string and a putative suffix, returns the string minus the
//    suffix string if the suffix matches, otherwise the original
//    string.
// ----------------------------------------------------------------------
inline bool HasSuffixString(const string& str,
                            const string& suffix) {
  return str.size() >= suffix.size() &&
         str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

inline string StripSuffixString(const string& str, const string& suffix) {
  if (HasSuffixString(str, suffix)) {
    return str.substr(0, str.size() - suffix.size());
  } else {
    return str;
  }
}

// Returns a copy of |filename| with any trailing ".protodevel" or ".proto
// suffix stripped.
string StripProto(const string& filename) {
  const char* suffix = HasSuffixString(filename, ".protodevel")
      ? ".protodevel" : ".proto";
  return StripSuffixString(filename, suffix);
}

// Returns the Python module name expected for a given .proto filename.
string PythonModuleName(const string& filename) {
  string basename = StripProto(filename);
  StripString(&basename, "-", '_');
  StripString(&basename, "/", '.');
  return basename + "_pb2";
}

// Returns the Python service implementation name expected for a given .proto filename.
string PythonImplName(const string& filename) {
  string basename = StripProto(filename);
  StripString(&basename, "-", '_');
  StripString(&basename, "/", '.');
  return basename;
}

string PythonPackageName(const string& filename) {
  string basename = StripProto(filename);
  StripString(&basename, "-", '_');
  StripString(&basename, "/", '.');
  return basename;
}
