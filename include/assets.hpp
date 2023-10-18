#pragma once

#include <string_view>
#include "beatsaber-hook/shared/utils/typedefs.h"

struct IncludedAsset {

    IncludedAsset(uint8_t* start, uint8_t* end) : array(reinterpret_cast<Array<uint8_t>*>(start)) {
        array->klass = nullptr;
        array->monitor = nullptr;
        array->bounds = nullptr;
        array->max_length = end - start - 33;
        *(end - 1)= '\0';
    }

    operator ArrayW<uint8_t>() const {
        init();
        return array;
    }

    Array<uint8_t>* Raw() const {
        init();
        return array;
    }

    operator std::string_view() const {
        return { reinterpret_cast<char*>(array->values), array->Length() };
    }

    operator std::span<uint8_t>() const {
        return { array->values, array->Length() };
    }

    void init() const {
        if(!array->klass)
            array->klass = classof(Array<uint8_t>*);
    }

    private:
        Array<uint8_t>* array;

};

#define DECLARE_FILE(name)                         \
    extern "C" uint8_t _binary_##name##_start[];  \
    extern "C" uint8_t _binary_##name##_end[];    \
    const IncludedAsset name { _binary_##name##_start, _binary_##name##_end};

namespace IncludedAssets {

	DECLARE_FILE(ChatPlexLogoLoading_webp)
	DECLARE_FILE(ChatPlexLogoTransparent_png)
	DECLARE_FILE(DefaultPackCover_png)
	DECLARE_FILE(Heart_png)
	DECLARE_FILE(NJS_png)
	DECLARE_FILE(Offset_png)
	DECLARE_FILE(QuestFonts_bundle)
	DECLARE_FILE(UIButton_png)
	DECLARE_FILE(UIColorPickerFBG_png)
	DECLARE_FILE(UIColorPickerHBG_png)
	DECLARE_FILE(UIColorPickerSBG_png)
	DECLARE_FILE(UIColorPickerVBG_png)
	DECLARE_FILE(UIDownArrow_png)
	DECLARE_FILE(UIIconGear_png)
	DECLARE_FILE(UIIconLocked_png)
	DECLARE_FILE(UIIconUnlocked_png)
	DECLARE_FILE(UIRectBG_png)
	DECLARE_FILE(UIRoundBG_png)
	DECLARE_FILE(UIRoundRectLeftBG_png)
	DECLARE_FILE(UIRoundRectRightBG_png)
	DECLARE_FILE(UIRoundSmoothFrame_png)
	DECLARE_FILE(UISliderBG_png)
	DECLARE_FILE(UISliderHandle_png)

}
