CppTestframework
================
A mini test framework to play with the hard(nasty?) stuff in `__cplusplus`.

Here's my brief plan:

###Reflection
+ Customized RTTI support, jsut get rid of the standard "typeid" stuff
+ Manual reflection (reflection code is added by hand)
+ Parser based reflection (reflection code is generated by hpp parser & generator)

###Serialization
+ Based on the reflection data(rather than hand-added code for each class)
+ So it is none-intrusive (since the reflection is already intrusive)
+ Pointer/reference graph walker: tree graph
+ Maybe I can find time for directional cycled or even general graph
+ Still I can look into Ogre for reference

###Garbage Collection & Smart Pointers
+ GC can be easily built upon the serialization's pointer graph walker(to mark and sweep)
+ GC need a custom memory pool that provides a set of memory allocators for different purpose(Like havok)
+ Reference counter, weak pointer, pointer graph walking

###Language Binding
+ LuaPlus
+ Extend Lua using C++
+ Extend C++ using Lua
+ (Optional):wxLua for wxWidget

###Multi Threading
+ Look into boost and Ogre and Havok for inspiration
+ Implement a custom(OO?) wrapper for the standard C APIs

###Sort, Search, and Container
A summary of my preparations for recent interviews
+ Quick/Merge/Heap/Select/Bubble sorting, with profilers to give an intuitive understanding
+ Binary Search vs Hash
+ Implement a container from the scratch(Data structures, sorting, memory management, object management)



