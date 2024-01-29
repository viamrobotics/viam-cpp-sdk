#pragma once

namespace viam {
namespace sdk {

class ViamInit {
   public:
    ViamInit();
    ~ViamInit();

   private:
    static bool initialized_;
};

}  // namespace sdk
}  // namespace viam
