cmake_minimum_required (VERSION 2.6)
project (bladerf-tun-interface)
# The version number.
set (BTI_VERSION_MAJOR 1)
set (BTI_VERSION_MINOR 0)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/includes/bticonfig.h.in"
  "${PROJECT_BINARY_DIR}/bticonfig.h"
  )
 
# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
include_directories("${PROJECT_BINARY_DIR}")
include_directories("${PROJECT_SOURCE_DIR}/includes")
include_directories(/usr/local/include)
include_directories(/opt/local/include)
link_directories(/usr/local/lib)
link_directories(/opt/local/lib)

# add the executable
add_executable(rx ${PROJECT_SOURCE_DIR}/src/ofdm_rx.c
	${PROJECT_SOURCE_DIR}/src/config.c
	${PROJECT_SOURCE_DIR}/src/transceive.c
	${PROJECT_SOURCE_DIR}/src/utils.c
)
add_executable(tx ${PROJECT_SOURCE_DIR}/src/ofdm_tx.c
	${PROJECT_SOURCE_DIR}/src/config.c
	${PROJECT_SOURCE_DIR}/src/transceive.c
	${PROJECT_SOURCE_DIR}/src/utils.c
)
add_executable(try ${PROJECT_SOURCE_DIR}/src/tryframing.c
	${PROJECT_SOURCE_DIR}/src/config.c
	${PROJECT_SOURCE_DIR}/src/transceive.c
	${PROJECT_SOURCE_DIR}/src/utils.c
)
add_executable(ofdm ${PROJECT_SOURCE_DIR}/src/ofdm.c
	${PROJECT_SOURCE_DIR}/src/config.c
	${PROJECT_SOURCE_DIR}/src/transceive.c
	${PROJECT_SOURCE_DIR}/src/utils.c
)

add_executable(frameexperiment ${PROJECT_SOURCE_DIR}/src/frameexperimenting.c
	${PROJECT_SOURCE_DIR}/src/config.c
	${PROJECT_SOURCE_DIR}/src/transceive.c
	${PROJECT_SOURCE_DIR}/src/utils.c
)

target_link_libraries(rx bladeRF m c liquid fec tuntap) 
target_link_libraries(tx bladeRF m c liquid fec tuntap) 
target_link_libraries(try bladeRF m c liquid fec tuntap) 
target_link_libraries(ofdm bladeRF m c liquid fec tuntap) 
target_link_libraries(frameexperiment bladeRF m c liquid fec tuntap) 
