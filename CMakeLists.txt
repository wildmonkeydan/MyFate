# PSn00bSDK example CMake script
# (C) 2021 spicyjpeg - MPL licensed

cmake_minimum_required(VERSION 3.21)

project(
	hellocpp
	LANGUAGES    CXX
	VERSION      1.0.0
	DESCRIPTION  "PSn00bSDK C++ hello world example"
	HOMEPAGE_URL "http://lameguy64.net/?page=psn00bsdk"
)

file(GLOB _sources *.cpp *.h)
psn00bsdk_add_executable(hellocpp GPREL ${_sources} data.cpp data.h camera.cpp camera.h ground.cpp ground.h utils.cpp utils.h pad.cpp pad.h player.cpp player.h npc.cpp npc.h dialouge.cpp dialouge.h background.cpp background.h item.cpp item.h)
psn00bsdk_add_cd_image(hellocpp_iso hellocpp iso.xml DEPENDS hellocpp)

install(FILES ${PROJECT_BINARY_DIR}/hellocpp.exe TYPE BIN)
