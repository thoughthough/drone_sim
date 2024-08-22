#ifndef IOBSERVER_H_
#define IOBSERVER_H_

#include <string>

class IObserver {
 public:
  virtual void notify(const std::string &message) const = 0;
};

#endif  // IOBSERVER_H_
