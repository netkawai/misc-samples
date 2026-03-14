Platform dependent part in programing language

This is a quick note to describe APIs part of native platforms specifically UWP(Microsoft), Mac/iOS(Apple). Both companies primary native language was evolved from C to C++ (Microsoft) and Objective-C/Swift (Apple)
there are similarity or difference.

## Microsoft (UWP)
C++(struct template) + COM

C++ does not have any language-defined root class. The author is a professor, he introduced some OOP paradigm, and Ada(Generic) paradigm into C.
However, in UWP(Windows), there is universal interface call IUnknown (Component model)
All Windows Runtime class (except generic class e.g. collections) should implement struct template (which equivalent to interface) which inherited IUnknown (+IInspectable introduced in Windows 8)
All implementation has done through template metaprogramming which introduced into C++ in 1998 (C++98)
This makes C++ more macro-orient language rather than OOP, which sometimes is the target of criticism

https://learn.microsoft.com/en-us/windows/win32/api/unknwn/nn-unknwn-iunknown <br/>
https://learn.microsoft.com/en-us/windows/win32/api/inspectable/nn-inspectable-iinspectable

Collections <br/>
https://learn.microsoft.com/en-us/uwp/api/windows.foundation.collections.iiterable-1



## Apple (Mac/iOS)
Objective-C (CoreFoundation)/Swift

Next developed a new language call Objective-C inspired by SmallTalk which is a early message passing system.
Objective-C does not require root class. However, since Objective-C itself was developed with NextStep operating system,
NSObject(Object) is a sort of root class. (but user-defined root class is possible)

https://opensource.apple.com/source/objc4/objc4-818.2/


Swift is modernizing the grammar of Objective-C, but inherited lots of concept. It is the same as Objective-C, universal root class is not required. but inherited from CoreFoundation which originally generic classes (collections, string etc) to manage memory or objects there is a root composited class called CFRuntime, which in C level is struct. <br/>
On the other hand, Swift is not OOP language. The function is first-class. and has the own runtime library, it does not necessary depend on objc4, however on Apple platform, it is required to interop with objc. <br/>
NOTE: I suspect that Apple is using Swift to improve their server/cloud side service. I have heard that Apple is one of big customer in Azure. I assume that iCloud is running on Linux with Apple's proprietary server software.

Foundation kit <br/>
https://github.com/apple/swift-corelibs-foundation/  

Grand Central Dispatch <br/>
https://github.com/apple/swift-corelibs-libdispatch 

Std lib<br/>
https://github.com/apple/swift/tree/main/stdlib/public/core


## Android(Google)
This is an exception, since it was developed at early 2000s. The root class is java.lang.Object. The language definition includes low level runtime APIs. (string, collection etc)
