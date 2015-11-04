 // EXTERNAL INCLUDES
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/extensions/XInput2.h>
#include <X11/extensions/XI2.h>

#ifdef DEBUG_ENABLED
#include <dali/integration-api/debug.h>
#include <dali/public-api/common/dali-vector.h>
#include <ostream>
#include <iomanip>
#endif

namespace Dali
{

namespace Internal
{

namespace Adaptor
{

namespace X11Debug
{

#ifdef DEBUG_ENABLED

namespace
{


Integration::Log::Filter* gInputDeviceLogFilter  = Integration::Log::Filter::New(Debug::NoLogging, false, "LOG_X_INPUT_DEVICES");
Integration::Log::Filter* gInputEventLogFilter  = Integration::Log::Filter::New(Debug::NoLogging, false, "LOG_X_INPUT_EVENTS");

struct XNameId
{
  const char* const name;
  int id;
};

const XNameId eventTable[]=
{
    { "XI_KeyPress"        ,XI_KeyPress         },
    { "XI_KeyRelease"      ,XI_KeyRelease       },
    { "XI_ButtonPress"     ,XI_ButtonPress      },
    { "XI_ButtonRelease"   ,XI_ButtonRelease    },
    { "XI_Motion"          ,XI_Motion           },
    { "XI_Enter"           ,XI_Enter            },
    { "XI_Leave"           ,XI_Leave            },
    { "XI_FocusIn"         ,XI_FocusIn          },
    { "XI_FocusOut"        ,XI_FocusOut         },
    { "XI_HierarchyChanged",XI_HierarchyChanged },
    { "XI_PropertyEvent"   ,XI_PropertyEvent    },
    { "XI_RawKeyPress"     ,XI_RawKeyPress      },
    { "XI_RawKeyRelease"   ,XI_RawKeyRelease    },
    { "XI_RawButtonPress"  ,XI_RawButtonPress   },
    { "XI_RawButtonRelease",XI_RawButtonRelease },
    { "XI_RawMotion"       ,XI_RawMotion        },
    { "XI_TouchBegin"      ,XI_TouchBegin       },
    { "XI_TouchUpdate"     ,XI_TouchUpdate      },
    { "XI_TouchEnd"        ,XI_TouchEnd         },
    { "XI_TouchOwnership"  ,XI_TouchOwnership   },
    { "XI_RawTouchBegin"   ,XI_RawTouchBegin    },
    { "XI_RawTouchUpdate"  ,XI_RawTouchUpdate   },
    { "XI_RawTouchEnd"     ,XI_RawTouchEnd      }
};

const XNameId deviceTypeTable[]=
{
    { "Master Pointer "    ,XIMasterPointer     },
    { "Master Keyboard"    ,XIMasterKeyboard    },
    { "Slave Pointer  "    ,XISlavePointer      },
    { "Slave Keyboard "    ,XISlaveKeyboard     },
    { "Floating Slave "    ,XIFloatingSlave     }
};

const XNameId inputClassTable[]=
{
    { "Key"      ,XIKeyClass       },
    { "Button"   ,XIButtonClass    },
    { "Valuator" ,XIValuatorClass  },
    { "Scroll"   ,XIScrollClass    },
    { "Touch"    ,XITouchClass     }
};

const unsigned int numberEvents = sizeof( eventTable ) / sizeof( eventTable[0] );
const unsigned int numberDevices = sizeof( deviceTypeTable ) / sizeof( deviceTypeTable[0] );
const unsigned int numberInputClasses = sizeof( inputClassTable ) / sizeof( inputClassTable[0] );

const char* GetEventName( int eventId )
{
  for( unsigned int i = 0; i < numberEvents; ++i )
  {
    if( eventTable[i].id == eventId )
    {
      return eventTable[i].name;
    }
  }
  return "unknown event";
}
const char* GetDeviceHierachyName( int deviceType )
{
  for( unsigned int i = 0; i < numberDevices; ++i )
  {
    if( deviceTypeTable[i].id == deviceType )
    {
      return deviceTypeTable[i].name;
    }
  }
  return "unknown device";
}
const char* GetInputClassName( int classId )
{
  for( unsigned int i = 0; i < numberInputClasses; ++i )
  {
    if( inputClassTable[i].id == classId )
    {
      return inputClassTable[i].name;
    }
  }
  return "unknown input class name";
}

std::string GetInputDeviceInfo( const XIDeviceInfo* device, bool master )
{
  // formatted output similar to xinput -list except it includes class + source information
  int startWidth = 45;

  std::string slavePadding="  ↳ ";
  if( master )
  {
    // slave entries are shifted to the right
    startWidth += 4;
    slavePadding="";
  }

  std::ostringstream oss;
  oss << "⎜" << slavePadding << std::setw(startWidth) <<  std::left << device->name ;
  oss << std::setw(1) << " id= " << std::setw(1) << device->deviceid ;
  oss << "\t[" << GetDeviceHierachyName( device->use ) << " ("<< device->attachment << ") ]";
  oss << std::setw(1) << "\t Classes: ";

  for( int n = 0; n < device->num_classes; ++n )
  {
    XIAnyClassInfo *classInfo = device->classes[n];
    oss << GetInputClassName( classInfo->type ) << ", source ( "<< classInfo->sourceid << ")";
  }
  oss << "\n";

  return oss.str();
}


}// unanmed namespace

void LogInputDeviceInfo( const XIDeviceInfo* devices, unsigned int numberOfDevices )
{
  // early exit if the filter is not enabled in debug mode
  if( ! gInputDeviceLogFilter->IsEnabledFor( Debug::General ) )
  {
    return;
  }

  const XIDeviceInfo* device = devices;
  const  XIDeviceInfo* masterKeyboard = NULL;
  const XIDeviceInfo* masterPointer  = NULL;
  Dali::Vector< const XIDeviceInfo* > slaveKeyboards;
  Dali::Vector< const XIDeviceInfo* > slavePointers;
  Dali::Vector< const XIDeviceInfo* > floatingSlaves;

  // go through the device list and sort by type
  for( unsigned int i = 0; i < numberOfDevices; ++i, ++device )
  {
    switch( device->use )
    {
      case XIMasterPointer:
      {
        masterPointer = device;
        break;
      }
      case XIMasterKeyboard:
      {
        masterKeyboard = device;
        break;
      }
      case XISlavePointer:
      {
        slavePointers.PushBack( device );
        break;
      }
      case XISlaveKeyboard:
      {
        slaveKeyboards.PushBack( device );
        break;
      }
      case XIFloatingSlave:
      {
        floatingSlaves.PushBack( device );
        break;
      }
      default:
      {
        break;
      }
    }
  }

  std::ostringstream oss;

  oss << "\n" << GetInputDeviceInfo( masterKeyboard , true);
  for( VectorBase::SizeType i = 0; i < slaveKeyboards.Count(); ++i )
  {
    oss << GetInputDeviceInfo( slaveKeyboards[i], false );
  }
  oss <<  "\n" << GetInputDeviceInfo( masterPointer, true );
  for( VectorBase::SizeType i = 0; i < slavePointers.Count(); ++i )
  {
    oss << GetInputDeviceInfo( slavePointers[i], false);
  }
  for( VectorBase::SizeType i = 0; i < floatingSlaves.Count(); ++i )
  {
    oss <<  GetInputDeviceInfo( floatingSlaves[i], false );
  }

 // DALI_LOG_ERROR_NOFN( "%s \n",oss.str().c_str() );
  DALI_LOG_INFO( gInputDeviceLogFilter, Debug::General, "%s\n", oss.str().c_str() );
}

void LogXI2Event( XGenericEventCookie* cookie )
{
  // early exit if the filter is not enabled
  if( ! gInputEventLogFilter->IsEnabledFor( Debug::General ) )
  {
    return;
  }

  std::ostringstream oss;
  oss << "XI2 event:" << GetEventName( cookie->evtype );

  XIDeviceEvent *event = static_cast< XIDeviceEvent* >(cookie->data);

  oss << ", device_id("<< event->deviceid << ")  source_id( "<< event->sourceid << ")" << ", flags: " << event->flags;
  oss << ", root-window: " << event->root << ", event-window: "<< event->event << ", child-window:" << event->child;
  if( cookie->evtype == XI_KeyPress)
  {
    oss << "base " << event->mods.base << "latched " << event->mods.latched;
    oss << "locked " << event->mods.locked << "effective " << event->mods.effective;

    if(  event->mods.effective & ShiftMask) oss << "Shift";
    if(  event->mods.effective & LockMask) oss << "LockMask"; // caps lock
    if(  event->mods.effective & ControlMask) oss << "ControlMask";
    if(  event->mods.effective & Mod1Mask) oss << "Mod1Mask";  // alt
    if(  event->mods.effective & Mod2Mask) oss << "Mod2Mask";  // num lock
    if(  event->mods.effective & Mod3Mask) oss << "Mod3Mask";
    if(  event->mods.effective & Mod4Mask) oss << "Mod4Mask";  // WINDOWS
    if(  event->mods.effective & Mod5Mask) oss << "Mod5Mask";  // Alt gr

  }

   // Mouse button state
  oss << " button state\n";
  for( int i =0; i< event->buttons.mask_len ; i++)
  {
    oss << "," << int(event->buttons.mask[i]);
  }

 // DALI_LOG_ERROR_NOFN( "%s \n",oss.str().c_str() );
  DALI_LOG_INFO( gInputEventLogFilter, Debug::General, "%s\n", oss.str().c_str() );

}



#else

void LogInputDeviceInfo( const XIDeviceInfo* devices, unsigned int numberOfDevices)
{
}
void LogXI2Event( XGenericEventCookie* cookie )
{
}

#endif


} // X11 Debug
} // namespace Adaptor
} // namespace Internal
} // namespace Dali
