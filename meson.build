project(
	'alsamap', 'c',
	default_options: ['warning_level=3',
		'werror=true',
		'buildtype=debugoptimized',
		'default_library=static']
)

executable(
	'alsamap',
	'alsamap.c',
	dependencies : [
        dependency('alsa'),
	],
)
