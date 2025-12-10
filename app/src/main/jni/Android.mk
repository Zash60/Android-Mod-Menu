LOCAL_PATH := $(call my-dir)
include $(CLEAR_ARS)

LOCAL_MODULE := ModMenu

# --- 1. Includes ---
LOCAL_C_INCLUDES += $(LOCAL_PATH)
LOCAL_C_INCLUDES += $(LOCAL_PATH)/Includes
LOCAL_C_INCLUDES += $(LOCAL_PATH)/Menu
LOCAL_C_INCLUDES += $(LOCAL_PATH)/KittyMemory
LOCAL_C_INCLUDES += $(LOCAL_PATH)/And64InlineHook

# --- 2. Arquivos ---
LOCAL_SRC_FILES := Main.cpp \
                   Menu/Menu.cpp \
                   Menu/Jni.cpp \
                   Includes/Utils.cpp \
                   KittyMemory/MemoryPatch.cpp \
                   KittyMemory/KittyMemory.cpp \
                   KittyMemory/KittyUtils.cpp \
                   KittyMemory/KittyScanner.cpp

# --- 3. CORREÇÃO DE ERROS (A Mágica) ---
# Isso força o compilador a ignorar o erro de "format string" e ativa C++17
LOCAL_CPPFLAGS += -std=c++17 -Wno-error=format-security -Wno-format-security

# --- 4. Bibliotecas ---
LOCAL_LDLIBS := -llog -landroid -lEGL -lGLESv2

include $(BUILD_SHARED_LIBRARY)
