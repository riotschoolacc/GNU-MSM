// avif_loader.cpp
#include <SDL2/SDL.h>
#include <GLES3/gl3.h>
#include <avif/avif.h>
#include <iostream>

GLuint createQuad() {
    GLfloat vertices[] = {
        // positions       // texture coordinates
        -1.0f,  1.0f,     0.0f, 1.0f,   // top-left
        -1.0f, -1.0f,     0.0f, 0.0f,   // bottom-left
         1.0f, -1.0f,     1.0f, 0.0f,   // bottom-right
         1.0f,  1.0f,     1.0f, 1.0f    // top-right
    };

    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    return VAO;
}

GLuint loadAVIF(const char* filepath) {
    avifDecoder* decoder = avifDecoderCreate();
    avifImage* image = avifImageCreateEmpty();

    avifResult result = avifDecoderReadFile(decoder, image, filepath);
    if (result != AVIF_RESULT_OK) {
        std::cerr << "Failed to load AVIF image: " << filepath
                  << ", Error: " << avifResultToString(result) << std::endl;
        avifImageDestroy(image);
        avifDecoderDestroy(decoder);
        return 0;
    }

    int width = image->width;
    int height = image->height;

    avifRGBImage rgb;
    avifRGBImageSetDefaults(&rgb, image);
    rgb.format = AVIF_RGB_FORMAT_RGBA;
    avifRGBImageAllocatePixels(&rgb);

    result = avifImageYUVToRGB(image, &rgb);
    if (result != AVIF_RESULT_OK) {
        std::cerr << "Failed to convert AVIF image to RGB: " << filepath
                  << ", Error: " << avifResultToString(result) << std::endl;
        avifRGBImageFreePixels(&rgb);
        avifImageDestroy(image);
        avifDecoderDestroy(decoder);
        return 0;
    }

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgb.pixels);
    glGenerateMipmap(GL_TEXTURE_2D);

    avifRGBImageFreePixels(&rgb);
    avifImageDestroy(image);
    avifDecoderDestroy(decoder);

    return texture;
}
