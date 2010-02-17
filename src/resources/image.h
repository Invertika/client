/*
 *  The Mana World
 *  Copyright (C) 2004-2010  The Mana World Development Team
 *
 *  This file is part of The Mana World.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef IMAGE_H
#define IMAGE_H

#include "main.h"

#include "resources/resource.h"

#include <SDL.h>

#ifdef USE_OPENGL

/* The definition of OpenGL extensions by SDL is giving problems with recent
 * gl.h headers, since they also include these definitions. As we're not using
 * extensions anyway it's safe to just disable the SDL version.
 */
#define NO_SDL_GLEXT

#include <SDL_opengl.h>
#endif

class Dye;
class Position;

/**
 * Defines a class for loading and storing images.
 */
class Image : public Resource
{
    friend class Graphics;
#ifdef USE_OPENGL
    friend class OpenGLGraphics;
#endif

    public:
        /**
         * Destructor.
         */
        virtual ~Image();

        /**
         * Loads an image from a buffer in memory.
         *
         * @param buffer     The memory buffer containing the image data.
         * @param bufferSize The size of the memory buffer in bytes.
         *
         * @return <code>NULL</code> if an error occurred, a valid pointer
         *         otherwise.
         */
        static Resource *load(void *buffer, unsigned bufferSize);

        /**
         * Loads an image from a buffer in memory and recolors it.
         *
         * @param buffer     The memory buffer containing the image data.
         * @param bufferSize The size of the memory buffer in bytes.
         * @param dye        The dye used to recolor the image.
         *
         * @return <code>NULL</code> if an error occurred, a valid pointer
         *         otherwise.
         */
        static Resource *load(void *buffer, unsigned bufferSize,
                              Dye const &dye);

        /**
         * Loads an image from an SDL surface.
         */
        static Image *load(SDL_Surface *);

        /**
         * Frees the resources created by SDL.
         */
        virtual void unload();

        /**
         * Tells is the image is loaded
         */
        bool isLoaded()
        { return mLoaded; }

        /**
         * Returns the width of the image.
         */
        virtual int getWidth() const
        { return mBounds.w; }

        /**
         * Returns the height of the image.
         */
        virtual int getHeight() const
        { return mBounds.h; }

        /**
         * Tells if the image was loaded using OpenGL or SDL
         * @return true if OpenGL, false if SDL.
         */
        bool isAnOpenGLOne() const;

        /**
         * Tells if the image has got an alpha channel
         * @return true if it's true, false otherwise.
         */
        bool hasAlphaChannel();

        /**
         * Sets the alpha value of this image.
         */
        virtual void setAlpha(float alpha);

        /**
         * Returns the alpha value of this image.
         */
        float getAlpha() const
        { return mAlpha; }

        /**
         * Creates a new image with the desired clipping rectangle.
         *
         * @return <code>NULL</code> if creation failed and a valid
         *         object otherwise.
         */
        virtual Image *getSubImage(int x, int y, int width, int height);


        // SDL only public functions

        /**
         * Gets an scaled instance of an image.
         *
         * @param width The desired width of the scaled image.
         * @param height The desired height of the scaled image.
         *
         * @return A new Image* object.
         */
        Image* SDLgetScaledImage(int width, int height);

        /**
         * Merges two image SDL_Surfaces together. This is for SDL use only, as
         * reducing the number of surfaces that SDL has to render can cut down
         * on the number of blit operations necessary, which in turn can help
         * improve overall framerates. Don't use unless you are using it to
         * reduce the number of overall layers that need to be drawn through SDL.
         */
        Image *SDLmerge(Image *image, int x, int y);

        /**
         * Get the alpha Channel of a SDL surface.
         */
        Uint8 *SDLgetAlphaChannel() const
        { return mAlphaChannel; }

#ifdef USE_OPENGL

        // OpenGL only public functions

        /**
         * Sets the target image format. Use <code>false</code> for SDL and
         * <code>true</code> for OpenGL.
         */
        static void setLoadAsOpenGL(bool useOpenGL);

        static bool getLoadAsOpenGL() { return mUseOpenGL; }

        int getTextureWidth() const { return mTexWidth; }
        int getTextureHeight() const { return mTexHeight; }
        static int getTextureType() { return mTextureType; }
#endif

    protected:

      // -----------------------
      // Generic protected members
      // -----------------------

        SDL_Rect mBounds;
        bool mLoaded;
        float mAlpha;
        bool mHasAlphaChannel;

      // -----------------------
      // SDL protected members
      // -----------------------

        /** SDL Constructor */
        Image(SDL_Surface *image, bool hasAlphaChannel = false,
              Uint8 *alphaChannel = NULL);

        /** SDL_Surface to SDL_Surface Image loader */
        static Image *_SDLload(SDL_Surface *tmpImage);

        SDL_Surface *mSDLSurface;

        /** Alpha Channel pointer used for 32bit based SDL surfaces */
        Uint8 *mAlphaChannel;

      // -----------------------
      // OpenGL protected members
      // -----------------------
#ifdef USE_OPENGL
        /**
         * OpenGL Constructor.
         */
        Image(GLuint glimage, int width, int height,
              int texWidth, int texHeight);

        /**
         * Returns the first power of two equal or bigger than the input.
         */
        static int powerOfTwo(int input);

        static Image *_GLload(SDL_Surface *tmpImage);

        GLuint mGLImage;
        int mTexWidth, mTexHeight;

        static bool mUseOpenGL;
        static int mTextureType;
        static int mTextureSize;
#endif
};

/**
 * A clipped version of a larger image.
 */
class SubImage : public Image
{
    public:
        /**
         * Constructor.
         */
        SubImage(Image *parent, SDL_Surface *image,
                 int x, int y, int width, int height);
#ifdef USE_OPENGL
        SubImage(Image *parent, GLuint image, int x, int y,
                 int width, int height, int texWidth, int textHeight);
#endif

        /**
         * Destructor.
         */
        ~SubImage();

        /**
         * Creates a new image with the desired clipping rectangle.
         *
         * @return <code>NULL</code> if creation failed and a valid
         *         image otherwise.
         */
        Image *getSubImage(int x, int y, int width, int height);

    private:
        Image *mParent;
};

#endif
