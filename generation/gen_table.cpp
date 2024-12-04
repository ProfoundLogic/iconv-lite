#include <errno.h>
#include <iconv.h>
#include <iomanip>
#include <iostream>
#include <qtqiconv.h>
#include <stdlib.h>
using namespace std;
int main(int argc, char * argv[]) {
  if (argc < 2) {
    cerr << "CCSID required." << endl;
    return 1;
  }
  char * end = 0;
  int ccsid = strtol(argv[1], &end, 10);
  if (ccsid < 1 || ccsid >= 65535) {
    cerr << "CCSID value \"" << argv[1] << "\" is invalid." << endl;
    return 1;
  }
  QtqCode_T from, to;
  memset(&from, 0, sizeof(from));
  memset(&to, 0, sizeof(to));
  from.CCSID = ccsid;
  to.CCSID = 1200;
  iconv_t cd = QtqIconvOpen(&to, &from);
  if (cd.return_value == -1) {
    cerr << "QtqIconvOpen(): " << strerror(errno) << endl;
    return 1;
  }
  char input[256];
  for (unsigned i = 0; i < sizeof(input); i++)
    input[i] = i;
  char output[sizeof(input) * 2];
  char * in = input;
  char * out = output;
  size_t inLeft = sizeof(input);
  size_t outLeft = sizeof(output);
  int rc = iconv(cd, &in, &inLeft, &out, &outLeft);
  int err = errno;
  iconv_close(cd);
  if (rc == -1) {
    cerr << "iconv(): " << strerror(err) << endl;
    return 1;
  }
  cout << setbase(16);
  cout << setfill('0');
  for (unsigned i = 0; i < sizeof(output); i += 2) {
    cout << "\\u";
    cout << setw(2) << (int) output[i];
    cout << setw(2) << (int) output[i + 1];
  }
  return 0;
}