LOCAL_PATH := $(call my-dir)
include $(CLEAR_ARS)

LOCAL_MODULE := ModMenu

# --- 1. Pastas de Cabeçalhos (.h / .hpp) ---
LOCAL_C_INCLUDES += $(LOCAL_PATH)
LOCAL_C_INCLUDES += $(LOCAL_PATH)/Includes
LOCAL_C_INCLUDES += $(LOCAL_PATH)/Menu
LOCAL_C_INCLUDES += $(LOCAL_PATH)/KittyMemory
LOCAL_C_INCLUDES += $(LOCAL_PATH)/And64InlineHook

# --- 2. Arquivos para Compilar (.cpp) ---
# Removi Logger.cpp pois ele não aparece no seu print da pasta Includes
LOCAL_SRC_FILES := Main.cpp \
                   Menu/Menu.cpp \
                   Menu/Jni.cpp \
                   Includes/Utils.cpp \
                   KittyMemory/MemoryPatch.cpp \
                   KittyMemory/KittyMemory.cpp \
                   KittyMemory/KittyUtils.cpp \
                   KittyMemory/KittyScanner.cpp

# --- 3. Bibliotecas do Sistema ---
LOCAL_LDLIBS := -llog -landroid -lEGL -lGLESv2

include $(BUILD_SHARED_LIBRARY)
