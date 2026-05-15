if (NOT FREERTOS_KERNEL_PATH AND NOT DEFINED ENV{FREERTOS_KERNEL_PATH})
    message("Skipping FreeRTOS examples as FREERTOS_KERNEL_PATH not defined")
    return()
endif()

include(FreeRTOS_Kernel_import.cmake)

set(MODEL_FOLDER ${CMAKE_SOURCE_DIR}/ei-model)
set(EI_SDK_FOLDER ${MODEL_FOLDER}/edge-impulse-sdk)

# ---------- Fase B: firmware de inferência ----------
add_executable(pico_emb
    infer_main.cpp
    ei_wrapper.cpp
    mpu6050.c
    rgb_led.c
)

target_include_directories(pico_emb PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${MODEL_FOLDER}
    ${MODEL_FOLDER}/tflite-model
    ${MODEL_FOLDER}/model-parameters
    ${EI_SDK_FOLDER}
    ${EI_SDK_FOLDER}/third_party
    ${EI_SDK_FOLDER}/third_party/ruy
    ${EI_SDK_FOLDER}/third_party/gemmlowp
    ${EI_SDK_FOLDER}/third_party/flatbuffers/include
    ${EI_SDK_FOLDER}/tensorflow
    ${EI_SDK_FOLDER}/dsp
    ${EI_SDK_FOLDER}/classifier
    ${EI_SDK_FOLDER}/anomaly
    ${EI_SDK_FOLDER}/CMSIS/NN/Include
    ${EI_SDK_FOLDER}/CMSIS/DSP/PrivateInclude
    ${EI_SDK_FOLDER}/CMSIS/DSP/Include
    ${EI_SDK_FOLDER}/CMSIS/Core/Include
    ${EI_SDK_FOLDER}/porting
)

target_link_libraries(pico_emb PRIVATE
    pico_stdlib
    pico_unique_id
    hardware_i2c
    hardware_gpio
    pico_async_context_freertos
    FreeRTOS-Kernel-Heap4
)

target_compile_definitions(pico_emb PRIVATE
    configNUMBER_OF_CORES=1
)

# Glob completo da EI SDK
include(${EI_SDK_FOLDER}/cmake/utils.cmake)
RECURSIVE_FIND_FILE(EI_CPP   ${EI_SDK_FOLDER} "*.cpp")
RECURSIVE_FIND_FILE(EI_CC    ${EI_SDK_FOLDER} "*.cc")
RECURSIVE_FIND_FILE(EI_C     ${EI_SDK_FOLDER} "*.c")
RECURSIVE_FIND_FILE(EI_S     ${EI_SDK_FOLDER} "*.s")
RECURSIVE_FIND_FILE(EI_MODEL ${MODEL_FOLDER}/tflite-model "*.cpp")

target_sources(pico_emb PRIVATE
    ${EI_CPP} ${EI_CC} ${EI_C} ${EI_S} ${EI_MODEL}
)

# USB CDC para serial (115200 padrão) — diferente do exemplo, que usa UART.
pico_enable_stdio_usb(pico_emb 1)
pico_enable_stdio_uart(pico_emb 0)

pico_add_extra_outputs(pico_emb)

set_target_properties(pico_emb PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)
