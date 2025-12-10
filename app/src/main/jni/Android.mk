LOCAL_PATH := $(call my-dir)
include $(CLEAR_ARS)

LOCAL_MODULE := ModMenu

# --- 1. Onde procurar os arquivos de cabeçalho (.h) ---
# Adiciona a raiz (jni) para que possamos chamar "KittyMemory/..."
LOCAL_C_INCLUDES += $(LOCAL_PATH)
LOCAL_C_INCLUDES += $(LOCAL_PATH)/Includes
LOCAL_C_INCLUDES += $(LOCAL_PATH)/Menu
LOCAL_C_INCLUDES += $(LOCAL_PATH)/KittyMemory

# --- 2. Lista Manual dos Arquivos (.cpp) ---
# Isso garante que ele ache os arquivos sem depender de wildcards
LOCAL_SRC_FILES := Main.cpp \
                   Includes/Logger.cpp \
                   Includes/Utils.cpp \
                   Menu/Menu.cpp \
                   KittyMemory/MemoryPatch.cpp \
                   KittyMemory/Utils.cpp

# --- 3. Bibliotecas do Sistema ---
LOCAL_LDLIBS := -llog -landroid -lEGL -lGLESv2

include $(BUILD_SHARED_LIBRARY)
