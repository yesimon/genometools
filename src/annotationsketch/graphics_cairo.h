/*
  Copyright (c) 2008 Sascha Steinbiss <ssteinbiss@stud.zbh.uni-hamburg.de>
  Copyright (c) 2008 Center for Bioinformatics, University of Hamburg

  Permission to use, copy, modify, and distribute this software for any
  purpose with or without fee is hereby granted, provided that the above
  copyright notice and this permission notice appear in all copies.

  THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
  WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
  MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
  ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
  ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
  OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#ifndef GRAPHICS_CAIRO_H
#define GRAPHICS_CAIRO_H

#include <cairo.h>
#include "annotationsketch/graphics.h"

/* Implements the GT_Graphics interface.
   This implementation uses the Cairo 2D vector graphics library as a
   drawing back-end. */
typedef struct GT_GraphicsCairo GT_GraphicsCairo;

const GT_GraphicsClass* gt_graphics_cairo_class(void);
GT_Graphics*            gt_graphics_cairo_new(GT_GT_GraphicsOutType type,
                                        unsigned int width,
                                        unsigned int height);
GT_Graphics*            gt_graphics_cairo_new_from_context(cairo_t *context,
                                                     unsigned int width,
                                                     unsigned int height);
#endif
