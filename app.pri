TEMPLATE = app
CONFIG += console
CONFIG -= qt
CONFIG += sdl2

INCLUDEPATH += ../../seed/include ../../seed/contrib
include("../../seed/compiler.pri")

CONFIG(debug, debug|release) {
	DESTDIR = bin
	DEFINES += DEBUG
	LIBS += -L../../seed/lib/debug
} else {
	DESTDIR = bin
	DEFINES += RELEASE
	LIBS += -L../../seed/lib/release
}

unix:!macx {
		DEFINES += LINUX
		LIBS += -lseed -lseedcontrib -lGL -lopenal -lX11 -lm -ldl -lpthread -lrt -lc++abi
		QMAKE_CXXFLAGS += -std=c++11

		APP_QML_FILES.files = $$OTHER_FILES
		APP_QML_FILES.path = Contents/Resources
		QMAKE_BUNDLE_DATA += APP_QML_FILES
}

macx {
		DEFINES += LINUX
		INCLUDEPATH += ../../seed/contrib/osx/
		LIBS += -lseed -lseedcontrib -framework OpenAL -framework OpenGL -framework Cocoa -framework IOKit -lSDL
		CONFIG -= sdl
		CONFIG += glfw

		#Configs
		APP_CONFIG_FILES.files = $$OTHER_FILES_CONFIG
		APP_CONFIG_FILES.path = Contents/Resources/configs
		#Fonts
		APP_FONT_FILES.files = $$OTHER_FILES_FONT
		APP_FONT_FILES.path = Contents/Resources/fonts
		#Scenes
		APP_SCENE_FILES.files = $$OTHER_FILES_SCENE
		APP_SCENE_FILES.path = Contents/Resources/scenes
		#Textures
		APP_TEXTURE_FILES.files = $$OTHER_FILES_TEXTURE
		APP_TEXTURE_FILES.path = Contents/Resources/textures
		#Gui
			#Styles
			APP_GUI_STYLE_FILES.files = $$OTHER_FILES_GUI_STYLE
			APP_GUI_STYLE_FILES.path = Contents/Resources/gui/styles
			#Views
			APP_GUI_VIEW_FILES.files = $$OTHER_FILES_GUI_VIEW
			APP_GUI_VIEW_FILES.path = Contents/Resources/gui/views
			#Texures
			APP_GUI_TEXTURE_FILES.files = $$OTHER_FILES_GUI_TEXTURE
			APP_GUI_TEXTURE_FILES.path = Contents/Resources/gui/textures
		#Sounds
		APP_SOUND_FILES.files = $$OTHER_FILES_SOUND
		APP_SOUND_FILES.path = Contents/Resources/sounds
		#Maps
		APP_MAPS_FILES.files = $$OTHER_FILES_MAPS
		APP_MAPS_FILES.path = Contents/Resources/

		QMAKE_BUNDLE_DATA += APP_CONFIG_FILES APP_FONT_FILES APP_SCENE_FILES \
						APP_TEXTURE_FILES APP_GUI_STYLE_FILES APP_GUI_VIEW_FILES \
						APP_GUI_TEXTURE_FILES APP_SOUND_FILES APP_MAPS_FILES
}

win32 {
		LIBS += -L../../seed/contrib/windows/ -lseed -lseedcontrib -mwindows -lmingw32 -lopengl32 -lopenal32
		INCLUDEPATH += ../../seed/contrib/windows/
		CONFIG -= glfw
		CONFIG += sdl
		sdl {
				DEFINES += WIN32 main=SDL_main
				LIBS += -lSDLmain -lSDL -lSDL_image -lgdi32
		}
}

glfw {
		DEFINES += BUILD_GLFW
} else:sdl {
		DEFINES += BUILD_SDL
} else:sdl2 {
		DEFINES += BUILD_SDL2
}
