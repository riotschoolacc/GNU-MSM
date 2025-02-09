// LoaderAVIF.cpp
#include "Singleton.h"

namespace sys {

LoaderAVIF::LoaderAVIF()
{
}

LoaderAVIF::~LoaderAVIF()
{
}

SDL_Surface* LoaderAVIF::Open(const std::string& filePath) {
    avifDecoder* decoder = avifDecoderCreate();
    if (!decoder) {
        std::cerr << "Failed to create AVIF decoder" << std::endl;
        return nullptr;
    }

    avifDecoderSetIOFile(decoder, filePath.c_str());

    avifResult result = avifDecoderParse(decoder);
    if (result != AVIF_RESULT_OK) {
        std::cerr << "Failed to parse AVIF: " << avifResultToString(result) << std::endl;
        avifDecoderDestroy(decoder);
        return nullptr;
    }

    result = avifDecoderNextImage(decoder);
    if (result != AVIF_RESULT_OK) {
        std::cerr << "Failed to decode AVIF: " << avifResultToString(result) << std::endl;
        avifDecoderDestroy(decoder);
        return nullptr;
    }

    int width = decoder->image->width;
    int height = decoder->image->height;
    size_t dataSize = static_cast<size_t>(width) * height * 4; // RGBA (4 bytes per pixel)

    std::vector<uint8_t> imageData(dataSize);

    avifRGBImage rgb;
    avifRGBImageSetDefaults(&rgb, decoder->image);
    rgb.depth = 8; // 8 bits per channel
    rgb.format = AVIF_RGB_FORMAT_RGBA;
    rgb.pixels = imageData.data();
    rgb.rowBytes = width * 4;

    result = avifImageYUVToRGB(decoder->image, &rgb);
    if (result != AVIF_RESULT_OK) {
        std::cerr << "Failed to convert YUV to RGB: " << avifResultToString(result) << std::endl;
        avifDecoderDestroy(decoder);
        return nullptr;
    }

    avifDecoderDestroy(decoder);

    SDL_Surface* surface = SDL_CreateRGBSurface(
        0, width, height, 32,
        0x000000FF,  // Red mask
        0x0000FF00,  // Green mask
        0x00FF0000,  // Blue mask
        0xFF000000   // Alpha mask
    );
    if (!surface) {
        std::cerr << "Failed to create SDL_Surface: " << SDL_GetError() << std::endl;
        return nullptr;
    }

    if (SDL_LockSurface(surface) < 0) {
        std::cerr << "SDL_LockSurface failed: " << SDL_GetError() << std::endl;
        SDL_FreeSurface(surface);
        return nullptr;
    }

    if (static_cast<size_t>(surface->pitch) == width * 4) {
        memcpy(surface->pixels, imageData.data(), dataSize);
    } else {
        uint8_t* src = imageData.data();
        uint8_t* dst = static_cast<uint8_t*>(surface->pixels);
        for (int y = 0; y < height; ++y) {
            memcpy(dst + y * surface->pitch, src + y * width * 4, width * 4);
        }
    }
    SDL_UnlockSurface(surface);

    return surface;
}

} // namespace sys
