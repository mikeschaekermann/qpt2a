# 1. Build Targets:
## Debug:
* All debugging information are enabled
* No optimization
* Inline functions won't be used
* Intrinsic functions won't be used

## Release:
* Debug information is available
* Some optimization
* Inline functions will be used if suitable
* Intrinsic functions will be used

## Final:
* No debug information is available
* Full optimization
* Inline functions will be used if suitable
* Intrinsic functions will be used


# 2. Defines:
## _DEBUG
* For debugging
* Is only available in Debug and Release target

## NDEBUG
* Is only available in Final target

## _LOG
* Is used for logging so it can be removed easily from the final build


# 3. Libraries:

* Cinder lib
* FMod for sound
* Assimp as asset importer

# 4. Info:
VS 2010 SP1 required