CC := clang++
CFLAGS := $(CFLAGS:-s=)

LIB_DIRS := \
	/opt/homebrew/Cellar/sfml/3.0.0/lib/
INCLUDE_DIRS := \
	/opt/homebrew/Cellar/sfml/3.0.0/include/

BUILD_FLAGS :=

MACOS_FRAMEWORK_PATHS := \
	/Library/Frameworks

# Name, no extension (eg. CoreFoundation, ogg)
MACOS_FRAMEWORKS := \
	CoreFoundation

# Icon .png
PRODUCTION_MACOS_ICON := sfml

PRODUCTION_DEPENDENCIES := \
	$(PRODUCTION_DEPENDENCIES)

PRODUCTION_MACOS_BUNDLE_DEVELOPER := developer
PRODUCTION_MACOS_BUNDLE_DISPLAY_NAME := SFML Boilerplate
PRODUCTION_MACOS_BUNDLE_NAME := SFML Boilerplate
PRODUCTION_MACOS_MAKE_DMG := true
PRODUCTION_MACOS_BACKGROUND := dmg-background

PRODUCTION_MACOS_DYLIBS := \
	/opt/homebrew/Cellar/sfml/3.0.0/lib/libsfml-graphics \
	/opt/homebrew/Cellar/sfml/3.0.0/lib/libsfml-audio \
	/opt/homebrew/Cellar/sfml/3.0.0/lib/libsfml-network \
	/opt/homebrew/Cellar/sfml/3.0.0/lib/libsfml-window \
	/opt/homebrew/Cellar/sfml/3.0.0/lib/libsfml-system

# Path, no extension (eg. /Library/Frameworks/ogg)
PRODUCTION_MACOS_FRAMEWORKS :=
