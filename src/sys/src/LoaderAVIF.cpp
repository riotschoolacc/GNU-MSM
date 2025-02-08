// LoaderAVIF.cpp
#include <iostream>
#include <vector>
#include <string>
#include <cstring>    // For memcpy
#include <avif/avif.h>
#include <SDL2/SDL.h>

class LoaderAVIF {
public:
    LoaderAVIF() = default;
    ~LoaderAVIF() = default;

    SDL_Surface* load(const std::string& filePath) {
        // Create the AVIF decoder.
        avifDecoder* decoder = avifDecoderCreate();
        if (!decoder) {
            std::cerr << "Failed to create AVIF decoder" << std::endl;
            return nullptr;
        }

        // Set the decoder to read from the specified file.
        avifDecoderSetIOFile(decoder, filePath.c_str());

        // Parse the AVIF file.
        avifResult result = avifDecoderParse(decoder);
        if (result != AVIF_RESULT_OK) {
            std::cerr << "Failed to parse AVIF: " << avifResultToString(result) << std::endl;
            avifDecoderDestroy(decoder);
            return nullptr;
        }

        // Decode the image.
        result = avifDecoderNextImage(decoder);
        if (result != AVIF_RESULT_OK) {
            std::cerr << "Failed to decode AVIF: " << avifResultToString(result) << std::endl;
            avifDecoderDestroy(decoder);
            return nullptr;
        }

        int width = decoder->image->width;
        int height = decoder->image->height;
        size_t dataSize = static_cast<size_t>(width) * height * 4; // RGBA (4 bytes per pixel)

        // Allocate a temporary buffer to hold the converted image.
        std::vector<uint8_t> imageData(dataSize);

        // Prepare an avifRGBImage for the conversion.
        avifRGBImage rgb;
        avifRGBImageSetDefaults(&rgb, decoder->image);
        rgb.depth = 8; // 8 bits per channel
        rgb.format = AVIF_RGB_FORMAT_RGBA;
        rgb.pixels = imageData.data();
        rgb.rowBytes = width * 4;

        // Convert the decoded YUV image to RGBA.
        result = avifImageYUVToRGB(decoder->image, &rgb);
        if (result != AVIF_RESULT_OK) {
            std::cerr << "Failed to convert YUV to RGB: " << avifResultToString(result) << std::endl;
            avifDecoderDestroy(decoder);
            return nullptr;
        }

        // We're done with the decoder.
        avifDecoderDestroy(decoder);

        // Create an SDL_Surface that allocates its own pixel memory.
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

        // Copy the image data into the SDL_Surface.
        if (SDL_LockSurface(surface) < 0) {
            std::cerr << "SDL_LockSurface failed: " << SDL_GetError() << std::endl;
            SDL_FreeSurface(surface);
            return nullptr;
        }

        // Check if the pitch (bytes per row) matches our expected row size.
        if (static_cast<size_t>(surface->pitch) == width * 4) {
            memcpy(surface->pixels, imageData.data(), dataSize);
        } else {
            // Otherwise, copy row by row.
            uint8_t* src = imageData.data();
            uint8_t* dst = static_cast<uint8_t*>(surface->pixels);
            for (int y = 0; y < height; ++y) {
                memcpy(dst + y * surface->pitch, src + y * width * 4, width * 4);
            }
        }
        SDL_UnlockSurface(surface);

        return surface;
    }
};
