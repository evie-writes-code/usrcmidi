# Evie/ULRIKE, 26/3/2020
# Building a library for midi handling using pd-lib-builder from:
# https://github.com/pure-data/pd-lib-builder.git

# edit to reflect local install of pd-lib-builder
local-pdlibbuilder-path = /Applications/Pd-0.48-1.app/Contents/Resources/pd-lib-builder/Makefile.pdlibbuilder

lib.name = midi

class.sources = src/mono.c src/indextomode.c

datafile = mono-help.pd indextomode-help.pd

# include Makefile.pdlibbuilder from directory 'pd-lib-builder' in Pd local install
include $(local-pdlibbuilder-path)