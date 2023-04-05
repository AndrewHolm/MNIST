# Fixes 
## #001 Use of smart pointers 

[Link to pull request](https://github.com/AndrewHolm/MNIST/pull/3)

The program can potentially dereference a null pointer, thereby raising a NullPointerException. An attacker can intentionally trigger a null pointer dereference, the attacker might be able to use the resulting exception to bypass security logic or to cause the application to reveal debugging information that will be valuable in planning subsequent attacks. The use of a smart pointer prevents this from happening. Smart pointers were implemented using unique_ptr from the standard template library. 


## #002 
