# Fixes 
## #001 Use of smart pointers 

[Link to pull request](https://github.com/AndrewHolm/MNIST/pull/3)

The program can potentially dereference a null pointer, thereby raising a NullPointerException. An attacker can intentionally trigger a null pointer dereference, the attacker might be able to use the resulting exception to bypass security logic or to cause the application to reveal debugging information that will be valuable in planning subsequent attacks. The use of a smart pointer prevents this from happening. Smart pointers were implemented using unique_ptr from the standard template library. 


## #002 GitHub Changes

Made it so that it is required that there exists a pull request before merging. This ensures that no code will be merger without the notification of the members of the repo. Further made it necessary for the pull request to be aproved before it can be completed. Implemented CodeQL to scan code for potential vulnerabilities.

[Link to CodeQL Implementation](https://github.com/AndrewHolm/MNIST/tree/main/.github/workflows)

[link to CodeQl analysis](https://github.com/AndrewHolm/MNIST/actions/workflows/codeql.yml)
