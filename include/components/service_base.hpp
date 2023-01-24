#ifndef SERVICE_BASE_H
#define SERVICE_BASE_H

#include <components/resource_manager.h>

// CR erodkin: this is literally just a ResourceManager. We don't need this,
// just use ResourceManager instead
class ComponentServiceBase {
   public:
    ResourceManager manager;
};

#endif
