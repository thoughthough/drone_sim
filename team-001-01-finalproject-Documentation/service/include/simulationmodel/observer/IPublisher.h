#ifndef IPUBLISHER_H_
#define IPUBLISHER_H_

#include <set>
#include <string>

#include "IObserver.h"

class IPublisher {
 public:
  void addObserver(const IObserver* o);
  void removeObserver(const IObserver* o);
  void notifyObservers(const std::string& message) const;

 private:
  std::set<const IObserver*> observers;
};

#endif  // IPUBLISHER_H_
