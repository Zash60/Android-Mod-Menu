#include <jni.h>
#include <errno.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Includes/Logger.h"
#include "Includes/obfuscate.h"
#include "KittyMemory/MemoryPatch.hpp"
#include "Menu/Menu.hpp"

// Define a Lib do Jogo
#define targetLibName OBFUSCATE("libil2cpp.so")

struct MemPatches {
    MemoryPatch godMode;
    MemoryPatch slowMotion;
    MemoryPatch superSpeed;
} gPatches;

// --- FUNÇÃO MANUAL PARA ENCONTRAR A LIB (SEM DEPENDER DE HEADERS) ---
uintptr_t get_lib_address(const char* lib_name) {
    uintptr_t addr = 0;
    char line[1024];
    
    // Abre o mapa de memória do jogo
    FILE *fp = fopen("/proc/self/maps", "r");
    if (fp) {
        while (fgets(line, sizeof(line), fp)) {
            // Procura pela linha que tem o nome da lib
            if (strstr(line, lib_name)) {
                // Converte o endereço inicial (Hex) para número
                addr = (uintptr_t)strtoul(line, NULL, 16);
                break;
            }
        }
        fclose(fp);
    }
    return addr;
}

// --- BOTÕES DO MENU ---
jobjectArray GetFeatureList(JNIEnv *env, jobject context) {
    jobjectArray ret;
    const char *features[] = {
            OBFUSCATE("Category_Mods Subway"),
            OBFUSCATE("Toggle_God Mode (Atravessar)"),
            OBFUSCATE("Toggle_Camera Lenta"),
            OBFUSCATE("Toggle_Super Velocidade"),
            OBFUSCATE("Button_Fechar Jogo")
    };
    int Total_Feature = (sizeof features / sizeof features[0]);
    ret = (jobjectArray) env->NewObjectArray(Total_Feature, env->FindClass(OBFUSCATE("java/lang/String")), env->NewStringUTF(""));
    for (int i = 0; i < Total_Feature; i++) env->SetObjectArrayElement(ret, i, env->NewStringUTF(features[i]));
    return (ret);
}

// --- AÇÃO DOS BOTÕES ---
void Changes(JNIEnv *env, jclass clazz, jobject obj, jint featNum, jstring featName, jint value, jlong Lvalue, jboolean boolean, jstring text) {
    switch (featNum) {
        case 0: // God Mode
            if (boolean) gPatches.godMode.Modify(); else gPatches.godMode.Restore();
            break;
        case 1: // Slow Motion
            if (boolean) { gPatches.superSpeed.Restore(); gPatches.slowMotion.Modify(); }
            else gPatches.slowMotion.Restore();
            break;
        case 2: // Super Speed
            if (boolean) { gPatches.slowMotion.Restore(); gPatches.superSpeed.Modify(); }
            else gPatches.superSpeed.Restore();
            break;
        case 3: exit(0); break;
    }
}

// --- THREAD DE INJEÇÃO ---
void *hack_thread(void *) {
    
    uintptr_t il2cppBase = 0;
    
    // Usa nossa função manual para esperar a lib carregar
    do {
        il2cppBase = get_lib_address("libil2cpp.so"); // Nome sem ofuscação aqui para a busca manual funcionar melhor com strstr
        sleep(1);
    } while (il2cppBase == 0);

    // --- OFFSETS (32-BITS) ---
    // God Mode (Offset: 0x7BC5C8)
    gPatches.godMode = MemoryPatch::createWithHex(il2cppBase + 0x7BC5C8, "1E FF 2F E1");

    // Slow Motion (Offset: 0x846704)
    gPatches.slowMotion = MemoryPatch::createWithHex(il2cppBase + 0x846704, "00 00 A0 E3 A0 01 44 E3 10 0A 07 EE 1E FF 2F E1");

    // Super Speed (Offset: 0x846704)
    gPatches.superSpeed = MemoryPatch::createWithHex(il2cppBase + 0x846704, "00 00 A0 E3 00 01 4C E3 10 0A 07 EE 1E FF 2F E1");

    return nullptr;
}

__attribute__((constructor)) void lib_main() {
    pthread_t ptid;
    pthread_create(&ptid, NULL, hack_thread, NULL);
}
