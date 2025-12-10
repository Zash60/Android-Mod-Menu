APP_ABI := armeabi-v7a
APP_PLATFORM := android-21
APP_STL := c++_static

# CORREÇÃO: Adicionamos flags para ignorar erros de formato e usar C++17
APP_CPPFLAGS += -frtti -fexceptions -std=c++17 -Wno-format-security -Wno-error=format-security
