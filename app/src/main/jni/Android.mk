LOCAL_PATH := $(call my-dir)
include $(CLEAR_ARS)

# Nome do arquivo final (.so)
LOCAL_MODULE := ModMenu

# --- ONDE PROCURAR OS ARQUIVOS (.h) ---
# Aqui dizemos: "Olhe dentro destas pastas!"
LOCAL_C_INCLUDES += $(LOCAL_PATH)
LOCAL_C_INCLUDES += $(LOCAL_PATH)/KittyMemory
LOCAL_C_INCLUDES += $(LOCAL_PATH)/Menu
LOCAL_C_INCLUDES += $(LOCAL_PATH)/Includes
LOCAL_C_INCLUDES += $(LOCAL_PATH)/And64InlineHook

# --- QUAIS ARQUIVOS COMPILAR (.cpp) ---
# Aqui listamos todos os arquivos de código
LOCAL_SRC_FILES := Main.cpp \
                   KittyMemory/MemoryPatch.cpp \
                   KittyMemory/Utils.cpp \
                   Menu/Menu.cpp \
                   Includes/Utils.cpp \
                   Includes/Logger.cpp

# Bibliotecas do sistema Android necessárias
LOCAL_LDLIBS := -llog -landroid -lEGL -lGLESv2

include $(BUILD_SHARED_LIBRARY)
