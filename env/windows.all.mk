CC := g++

_MINGW := C:/msys64/mingw64
_SFML := C:/programming/SFML
_SFML_BIN := $(_SFML)/build/lib

LIB_DIRS := \
	$(_SFML)/build/lib

INCLUDE_DIRS := \
	$(_SFML)/include

BUILD_DEPENDENCIES := \
	$(_SFML_BIN)/openal32.dll

LINK_LIBRARIES := \
	$(LINK_LIBRARIES) \
	stdc++fs \
	gdi32

PRODUCTION_DEPENDENCIES := \
	$(PRODUCTION_DEPENDENCIES) \
	$(_MINGW)/libgcc_s_dw2-1.dll \
	$(_MINGW)/libstdc++-6.dll \
	$(_MINGW)/libwinpthread-1.dll \
	$(_SFML_BIN)/openal32.dll \
	$(_SFML_BIN)/sfml-audio-3.dll \
	$(_SFML_BIN)/sfml-graphics-3.dll \
	$(_SFML_BIN)/sfml-network-3.dll \
	$(_SFML_BIN)/sfml-system-3.dll \
	$(_SFML_BIN)/sfml-window-3.dll
