# 1. Build Targets:
## Debug:
All debugging information are enabled
No optimization
Inline functions won't be used
Intrinsic functions won't be used

## Release:
Debug information is available
Some optimization
Inline functions will be used if suitable
Intrinsic functions will be used

## Final:
No debug information is available
Full optimization
Inline functions will be used if suitable
Intrinsic functions will be used


# 2. Defines:
## _DEBUG
For debugging
Is only available in Debug and Release target

## NDEBUG
is only available in Final target

## _LOG
is used for logging so it can be removed easily from the final build


# 3. Libraries:

1. Cinder lib

# 4. Development guidelines:

## 1. Version Control System

1. Push buildable code only!

## 2. Coding guidelines

1. Document your code with doxygen!

## 3. Conditioning methods

1. If one of the rules above was not followed by person from our team, this s/he will donate a bar of chocolate to the dev team! :) Yummy, please be sloppy!

# 5. Info:
VS 2010 SP1 required  
boost::log is used for runtime logging and debugging