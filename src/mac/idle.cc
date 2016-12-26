#import <CoreFoundation/CoreFoundation.h>
#import <IOKit/IOKitLib.h>
#import <AppKit/AppKit.h>
#import <AppKit/NSAccessibility.h>

/**
 Returns the number of seconds the machine has been idle or -1 if an error occurs.
 The code is compatible with Tiger/10.4 and later (but not iOS).
 */
int32_t SystemIdleTime(void)
{
  int32_t idlesecs = -1;
  io_iterator_t iter = 0;
  if (IOServiceGetMatchingServices(kIOMasterPortDefault, IOServiceMatching("IOHIDSystem"), &iter) == KERN_SUCCESS)
  {
    io_registry_entry_t entry = IOIteratorNext(iter);
    if (entry)
    {
      CFMutableDictionaryRef dict = NULL;
      if (IORegistryEntryCreateCFProperties(entry, &dict, kCFAllocatorDefault, 0) == KERN_SUCCESS)
      {
        CFNumberRef obj = (CFNumberRef)CFDictionaryGetValue(dict, CFSTR("HIDIdleTime"));
        if (obj)
        {
          int64_t nanoseconds = 0;
          if (CFNumberGetValue(obj, kCFNumberSInt64Type, &nanoseconds))
          {
            idlesecs = (int32_t)(nanoseconds / (1000L * 1000L));
          }
        }
        CFRelease(dict);
      }
      IOObjectRelease(entry);
    }
    IOObjectRelease(iter);
  }
  return idlesecs;
}

char* SystemIdleTime(void)
{
  // Get the process ID of the frontmost application.
  NSRunningApplication *app = [[NSWorkspace sharedWorkspace]
      frontmostApplication];
  pid_t pid = [app processIdentifier];

  // See if we have accessibility permissions, and if not, prompt the user to
  // visit System Preferences.
  NSDictionary *options = @{(id)kAXTrustedCheckOptionPrompt : @YES };
  Boolean appHasPermission = AXIsProcessTrustedWithOptions(
      (__bridge CFDictionaryRef)options);
  if (!appHasPermission)
  {
    return NULL; // we don't have accessibility permissions

    // Get the accessibility element corresponding to the frontmost application.
    AXUIElementRef appElem = AXUIElementCreateApplication(pid);
    if (!appElem)
    {
      return NULL;
    }

    // Get the accessibility element corresponding to the frontmost window
    // of the frontmost application.
    AXUIElementRef window = NULL;
    if (AXUIElementCopyAttributeValue(appElem,
                                      kAXFocusedWindowAttribute, (CFTypeRef *)&window) != kAXErrorSuccess)
    {
      CFRelease(appElem);
      return NULL;
    }

    // Finally, get the title of the frontmost window.
    CFStringRef title = NULL;
    AXError result = AXUIElementCopyAttributeValue(window, kAXTitleAttribute,
                                                   (CFTypeRef *)&title);

    // At this point, we don't need window and appElem anymore.
    CFRelease(window);
    CFRelease(appElem);

    if (result != kAXErrorSuccess)
    {
      // Failed to get the window title.
      return NULL;
    }

    // Success! Now, do something with the title, e.g. copy it somewhere.
    char* buffer = CFStringCopyUTF8String(title);

    // Once we're done with the title, release it.
    CFRelease(title);

    return buffer;
  }

  char* CFStringCopyUTF8String(CFStringRef aString)
  {
    if (aString == NULL)
    {
      return NULL;
    }

    CFIndex length = CFStringGetLength(aString);
    CFIndex maxSize =
        CFStringGetMaximumSizeForEncoding(length, kCFStringEncodingUTF8) + 1;
    char *buffer = (char *)malloc(maxSize);
    if (CFStringGetCString(aString, buffer, maxSize,
                           kCFStringEncodingUTF8))
    {
      return buffer;
    }
    return NULL;
  }