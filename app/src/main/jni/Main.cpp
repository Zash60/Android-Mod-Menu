#include <jni.h>
#include <errno.h>
#include <pthread.h>
#include <unistd.h>

// Includes do Template
#include "Includes/Logger.h"
#include "Includes/obfuscate.h"
#include "Includes/Utils.hpp"
#include "KittyMemory/MemoryPatch.hpp"
#include "Menu/Menu.hpp"

// 1. CORREÇÃO DE NAMESPACE: Isso resolve o erro do "ProcMap"
using namespace KittyMemory;

// Lib do Jogo
#define targetLibName OBFUSCATE("libil2cpp.so")

struct MemPatches {
    MemoryPatch godMode;
    MemoryPatch slowMotion;
    MemoryPatch superSpeed;
} gPatches;

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

// --- THREAD DE INJEÇÃO (CORRIGIDA) ---
void *hack_thread(void *) {
    
    // 2. CORREÇÃO DA BUSCA: Usamos getLibraryBase que é universal
    // Retorna o endereço direto (número) em vez de um Struct complexo
    uintptr_t il2cppBase = 0;
    
    do {
        il2cppBase = getLibraryBase(targetLibName);
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
