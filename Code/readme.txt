Build Targets:
	Debug:
		All debugging information are enabled
		No optimization
		Inline functions won't be used
		Intrinsic functions won't be used
		
	Release:
		Debug information is available
		Some optimization
		Inline functions will be used if suitable
		Intrinsic functions will be used
		
	Final:
		No debug information is available
		Full optimization
		Inline functions will be used if suitable
		Intrinsic functions will be used

		
Defines:
	_DEBUG
		For debugging
		Is only available in Debug and Release target
		
	NDEBUG
		is only available in Final target
		

Info:
	VS 2010 SP1 required
	boost::log is used for runtime logging and debugging