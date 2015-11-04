//CLASS HEADER
#include "x-input2-device.h"

// EXTERNAL INCLUDES


namespace Dali
{

namespace Internal
{

namespace Adaptor
{

void XInput2Device::AssignDeviceInfo( const XIDeviceInfo* device )
{
  deviceId = device->deviceid;
  attachment = device->attachment;
  use = device->use;

  for( int n = 0; n < device->num_classes; ++n )
  {
    XIAnyClassInfo *classInfo = device->classes[n];
    switch( classInfo->type  )
    {
      case XITouchClass:
      {
        touchClass = true;
        break;
      }
      case XIButtonClass:
      {
        buttonClass = true;
        break;
      }
      case XIValuatorClass:
      {
        valuatorClass = true;
        break;
      }
      case XIScrollClass:
      {
        scrollClass = true;
        break;
      }
      case XIKeyClass:
      {
        keyClass = true;
        break;
      }
      default:
      {
        // unknown
        break;
      }
    }
  }


}


} // namespace Adaptor
} // namespace Internal
} // namespace Dali
