cmake_minimum_required(VERSION 3.24)

# Name, version, and language
project(UtilityGain VERSION 0.1 LANGUAGES CXX)

# We’re writing a plugin (shared library) in modern C++
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)  # required for macOS plugins

# Fetch JUCE at configure time and pin a version for reproducibility
include(FetchContent)
FetchContent_Declare(
    JUCE
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG 8.0.8         # pin the JUCE version you tested with
)
FetchContent_MakeAvailable(JUCE)   # creates juce:: targets we can link to

# Define the plugin target and its metadata/outputs
juce_add_plugin(UtilityGain
    COMPANY_NAME "Nico Audio"
    BUNDLE_ID com.nicoguzman.UtilityGain
    PLUGIN_MANUFACTURER_CODE Mata   # 4 chars
    PLUGIN_CODE Ugai                # 4 chars, make unique
    FORMATS AU VST3 Standalone      # build Audio Unit + VST3 on macOS
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    COPY_PLUGIN_AFTER_BUILD TRUE
    PRODUCT_NAME "UtilityGain"
)

target_sources(UtilityGain PRIVATE
    Source/PluginProcessor.cpp
    Source/PluginEditor.cpp
    Source/PluginProcessor.h
    Source/PluginEditor.h
    Source/Square.h
    Source/Square.cpp
    Source/MeterComponent.h
)

juce_generate_juce_header(UtilityGain)

# Trim unused JUCE features to reduce build size
target_compile_definitions(UtilityGain PRIVATE
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
)

# Link the JUCE modules we need (audio processors, DSP, basic GUI utilities)
target_link_libraries(UtilityGain PRIVATE
    juce::juce_audio_utils
    juce::juce_audio_processors
    juce::juce_dsp
    juce::juce_gui_extra
)
