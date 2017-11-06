//===-- Runtime race detection module of EmbedSanitizer - for Embeded ARM--===//
//
//
// This file is distributed under the BSD 3-clause "New" or "Revised" License
// License. See LICENSE.md for details.
//
//===----------------------------------------------------------------------===//
//
// (c) 2017 - Hassan Salehe Matar, Koc University
//            Email: hmatar@ku.edu.tr
//===----------------------------------------------------------------------===//

// Defines Race class for storing race metadata

#ifndef ETSAN_RACE_H_
#define ETSAN_RACE_H_

/**
 * This class saves information of race reported by the tool
 */
class Race {

public:
  unsigned int tid;
  int lineNo;
  string accessType;
  string objName;
  string fileName;
  vector<char*> trace;

  Race(unsigned int _tid,
       int _lineNo,
       string _accessType,
       char* _objName,
       char * _fileName) {

    tid = _tid;
    lineNo = _lineNo;
    accessType = _accessType;
    objName = _objName;
    fileName = _fileName;
  }

  /*
  bool operator==(Race &rhs) {

    if(lineNo != rhs.lineNo)
      return false;

    if(accessType != rhs.accessType)
      return false;

    if(fileName != rhs.fileName)
      return false;

    return true;
  }
 */

  /**
   * Prints the call stack of a thread when a race is found
   */
  string printStack() const {

    stringstream ss;

    int depth = 1;
    for( char * func : trace) {
      string msg(depth, ' ');
      depth += 4;
      ss << msg << " '--->" << func << "(...)" << endl;
    }

    return ss.str();
  }

  /**
   * Constructs a nicely reading message about the race
   * and stores the result in "msg" string. */
  void createRaceMessage(string& msg) const {
    stringstream ss;

    ss << "=============================================\n"  ;
    ss << "\033[1;32mEMBEDSANITIZER Race report\033[m\n"     ;
    ss << "\033[1;31m A race detected at: " << fileName << "\033[m\n";
    ss << "  At line number: "     << lineNo        << "\n"  ;
    ss << "  Thread (tid=" << tid << ") "                    ;
    ss <<    accessType << " \"" << objName  << "\"     \n"  ;
    ss << "                                             \n"  ;
    ss << "\033[1;33m Call stack:   \033[m              \n"  ;
    ss << printStack();
    ss << "=============================================\n"  ;

    msg.append( ss.str() );
  }

};


/** Comparison functor for comparing between two race reports
 */
struct race_compare {
  bool operator() (const Race& lhs, const Race& rhs) const {

    if(lhs.fileName == rhs.fileName) {
      if(lhs.lineNo < rhs.lineNo)
        return -1;
      else if (lhs.lineNo > rhs.lineNo)
        return 1;
      else if(lhs.accessType == rhs.accessType)
        return 0;
      else
        return -1;
    }

    return lhs.fileName < rhs.fileName;
  }
};


#endif // ETSAN_RACE_H_
