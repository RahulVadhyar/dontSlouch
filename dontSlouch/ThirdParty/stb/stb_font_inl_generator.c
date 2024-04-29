// Public Domain by stb
#include <stdio.h>
#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h" // http://nothings.org/stb/stb_truetype.h
#define STB_DEFINE
#include "stb.h" // http://nothings.org/stb.h

//#include "stb_font_test.inl"

// determine whether to write the png image
#define WRITE_FILE

#ifdef WRITE_FILE // debugging
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#endif

typedef struct
{
   unsigned char *bitmap;
   int index;
   int w,h;
   int x0,y0;
   int s,t;
   int advance;
} textchar;

textchar fontchar[65536];
unsigned int glyphmap[65536];
unsigned int glyph_to_textchar[65536];

int try_packing(int w, int h, int numchar)
{
   int i,j;
   int *front = malloc(h*sizeof(*front));
   memset(front, 0, h*sizeof(*front));
   h -= 1;
   for (i=0; i < numchar; ++i) {
      int c_height = fontchar[i].h;
      int c_width = fontchar[i].w;
      int pos, best_pos = -1, best_waste = (c_height+12) * 4096, best_left;
      for (pos = 0; pos+c_height+1 < h; ++pos) {
         int j, waste=0, leftedge=0;
         for (j=0; j < c_height+1; ++j)
            if (front[pos+j] > leftedge)
               leftedge = front[pos+j];
         if (leftedge + c_width > w-2)
            continue;
         for (j=0; j < c_height+1; ++j) {
            assert(leftedge >= front[pos+j]);
            waste += leftedge - front[pos+j];
         }
         if (waste < best_waste) {
            best_waste = waste;
            best_pos = pos;
            best_left = leftedge;
         }
      }
      if (best_pos == -1) {
         free(front);
         return 0;
      }
      // leave top row and left column empty to avoid needing border color
      fontchar[i].s = best_left+1;
      fontchar[i].t = best_pos+1;
      assert(best_pos >= 0);
      assert(best_pos+c_height <= h+1);
      for (j=0; j < c_height+1; ++j)
         front[best_pos + j] = best_left + c_width+1;
   }
   free(front);
   return 1;
}

int main(int argc, char **argv)
{
   unsigned char *packed;
   int zero_compress = 0;
   int area,spaces,words=0;
   int i,j;
   int packed_width, packed_height, packing_step, numbits,bitpack;
   float height, scale;
   unsigned char *data;
   char *name = "stbfont";
   stbtt_fontinfo font;
   int numchars;
   int ascent, descent, linegap, spacing;
   int numglyphs=0;

   if (argc != 4 && argc != 5) {
      fprintf(stderr, "Usage: stb_makefont {truetype-filename} {font-height-in-pixels} {highest-Unicode-codepoint} [{symbolic-name}]\n");
      return 1;
   } 

   data = stb_file(argv[1], NULL);
   if (data == NULL) { fprintf(stderr, "Couldn't open '%s'\n", argv[1]); return 1; }
   height = (float) atof(argv[2]);
   if (height < 1 || height > 600) { fprintf(stderr, "Font-height must be between 1 and 200\n"); return 1; }
   numchars = atoi(argv[3]) - 32 + 1;
   if (numchars < 0 || numchars > 66000) { fprintf(stderr, "Highest codepoint must be between 33 and 65535\n"); return 1; }
   if (argc == 5)
      name = argv[4];

   stbtt_InitFont(&font, data, 0);
   scale = stbtt_ScaleForPixelHeight(&font, height);
   stbtt_GetFontVMetrics(&font, &ascent,&descent,&linegap);

   spacing = (int) (scale * (ascent + descent + linegap) + 0.5);
   ascent = (int) (scale * ascent);

   for (i=0; i < 65536; ++i)
      glyph_to_textchar[i] = -1;

   area = 0;
   for (i=0; i < numchars; ++i) {
      int glyph = stbtt_FindGlyphIndex(&font, 32+i);
      if (glyph_to_textchar[glyph] == -1) {
         int n = numglyphs++;
         int advance, left_bearing;
         fontchar[n].bitmap = stbtt_GetGlyphBitmap(&font, scale,scale, glyph, &fontchar[n].w, &fontchar[n].h, &fontchar[n].x0, &fontchar[n].y0);
         fontchar[n].index = n;
         stbtt_GetGlyphHMetrics(&font, glyph, &advance, &left_bearing);
         fontchar[n].advance = advance;
         glyph_to_textchar[glyph] = n;
         area += (fontchar[n].w+1) * (fontchar[n].h+1);
      }
      glyphmap[i] = glyph_to_textchar[glyph];
      // compute the total area covered by the font
   }

   #if 0 // check that all digits are same width
   for (i='1'; i <= '9'; ++i) {
      assert(fontchar[glyphmap[i-32]].advance == fontchar[glyphmap['0'-32]].advance);
   }
   #endif

   // we sort before packing, but this may actually not be ideal for the current
   // packing algorithm -- it misses the opportunity to pack in some tiny characters
   // into gaps early on... maybe it needs to choose the best character greedily instead
   // of presorted? that will be hugely inefficient to compute

   if (height < 20 && numglyphs < 128)
      packed_width = 128;
   else
      packed_width = 256;

   if (numglyphs > 256 && height > 25)
      packed_width = 512;

   packed_height = area / packed_width + 4;

   packing_step = 2;
   if (packed_height % packing_step)
      packed_height += packing_step - (packed_height % packing_step);

   // sort by height
   qsort(fontchar, numglyphs, sizeof(fontchar[0]), stb_intcmp(offsetof(textchar,h)));
   // reverse
   for (i=0; i < numglyphs/2; ++i)
      stb_swap(&fontchar[i], &fontchar[numglyphs-1 - i], sizeof(fontchar[0]));

   for(;;) {
      if (try_packing(packed_width, packed_height, numchars))
         break;
      packed_height += packing_step;
      if (packed_height > 256) {
         if (packing_step <= 16)
            ++packing_step;
      }
   }

   // sort by index so we can lookup by index
   qsort(fontchar, numglyphs, sizeof(fontchar[0]), stb_intcmp(offsetof(textchar,index)));

   packed = malloc(packed_width * packed_height);
   memset(packed, 0, packed_width * packed_height);
   for (i=0; i < numglyphs; ++i) {
      int j;
      for (j=0; j < fontchar[i].h; ++j)
         memcpy(packed + (fontchar[i].t+j)*packed_width + fontchar[i].s, fontchar[i].bitmap + j*fontchar[i].w, fontchar[i].w);
      assert(fontchar[i].s + fontchar[i].w + 1 <= packed_width);
   }

   {
      int zc_bits=0;
      int db_bits=0;
      for (i=0; i < packed_width * packed_height; ++i) {
         if (packed[i] < 16)
            zc_bits += 1;
         else
            zc_bits += 4;
         if (packed[i] < 16 || packed[i] > 240)
            db_bits += 2;
         else
            db_bits += 4;
      }
      if (zc_bits < packed_width * packed_height * 32 / 10)
         zero_compress = 1;

      // always force zero_compress on, as the other path hasn't been fully tested,
      // as in my test fonts they never mattered
      zero_compress = 1;
   }

   if (zero_compress) {
      // squeeze to 9 distinct values
      for (i=0; i < packed_width * packed_height; ++i) {
         int n = (packed[i] + (255/8)/2) / (255/8);
         packed[i] = n;
      }
   } else {
      // UNTESTED
      // squeeze to 3 bits per pixel (2 bits didn't look good enough)
      for (i=0; i < packed_width * packed_height; ++i) {
         int n = (packed[i] >> 5);
         packed[i] = n;
      }
   }

   printf("// Font generated by stb_font_inl_generator.c ");
   if (zero_compress)
      printf("(4/1 bpp)\n");
   else
      printf("(3.2 bpp)\n");
   printf("//\n");
   printf("// Following instructions show how to use the only included font, whatever it is, in\n");
   printf("// a generic way so you can replace it with any other font by changing the include.\n");
   printf("// To use multiple fonts, replace STB_SOMEFONT_* below with STB_FONT_%s_*,\n", name);
   printf("// and separately install each font. Note that the CREATE function call has a\n");
   printf("// totally different name; it's just 'stb_font_%s'.\n", name);
   printf("//\n");
   printf("/* // Example usage:\n");
   printf("\n");
   printf("static stb_fontchar fontdata[STB_SOMEFONT_NUM_CHARS];\n");
   printf("\n");
   printf("static void init(void)\n");
   printf("{\n");
   printf("    // optionally replace both STB_SOMEFONT_BITMAP_HEIGHT with STB_SOMEFONT_BITMAP_HEIGHT_POW2\n");
   printf("    static unsigned char fontpixels[STB_SOMEFONT_BITMAP_HEIGHT][STB_SOMEFONT_BITMAP_WIDTH];\n");
   printf("    STB_SOMEFONT_CREATE(fontdata, fontpixels, STB_SOMEFONT_BITMAP_HEIGHT);\n");
   printf("    ... create texture ...\n");
   printf("    // for best results rendering 1:1 pixels texels, use nearest-neighbor sampling\n");
   printf("    // if allowed to scale up, use bilerp\n");
   printf("}\n");
   printf("\n");
   printf("// This function positions characters on integer coordinates, and assumes 1:1 texels to pixels\n");
   printf("// Appropriate if nearest-neighbor sampling is used\n");
   printf("static void draw_string_integer(int x, int y, char *str) // draw with top-left point x,y\n");
   printf("{\n");
   printf("    ... use texture ...\n");
   printf("    ... turn on alpha blending and gamma-correct alpha blending ...\n");
   printf("    glBegin(GL_QUADS);\n");
   printf("    while (*str) {\n");
   printf("        int char_codepoint = *str++;\n");
   printf("        stb_fontchar *cd = &fontdata[char_codepoint - STB_SOMEFONT_FIRST_CHAR];\n");
   printf("        glTexCoord2f(cd->s0, cd->t0); glVertex2i(x + cd->x0, y + cd->y0);\n");
   printf("        glTexCoord2f(cd->s1, cd->t0); glVertex2i(x + cd->x1, y + cd->y0);\n");
   printf("        glTexCoord2f(cd->s1, cd->t1); glVertex2i(x + cd->x1, y + cd->y1);\n");
   printf("        glTexCoord2f(cd->s0, cd->t1); glVertex2i(x + cd->x0, y + cd->y1);\n");
   printf("        // if bilerping, in D3D9 you'll need a half-pixel offset here for 1:1 to behave correct\n");
   printf("        x += cd->advance_int;\n");
   printf("    }\n");
   printf("    glEnd();\n");
   printf("}\n");
   printf("\n");
   printf("// This function positions characters on float coordinates, and doesn't require 1:1 texels to pixels\n");
   printf("// Appropriate if bilinear filtering is used\n");
   printf("static void draw_string_float(float x, float y, char *str) // draw with top-left point x,y\n");
   printf("{\n");
   printf("    ... use texture ...\n");
   printf("    ... turn on alpha blending and gamma-correct alpha blending ...\n");
   printf("    glBegin(GL_QUADS);\n");
   printf("    while (*str) {\n");
   printf("        int char_codepoint = *str++;\n");
   printf("        stb_fontchar *cd = &fontdata[char_codepoint - STB_SOMEFONT_FIRST_CHAR];\n");
   printf("        glTexCoord2f(cd->s0f, cd->t0f); glVertex2f(x + cd->x0f, y + cd->y0f);\n");
   printf("        glTexCoord2f(cd->s1f, cd->t0f); glVertex2f(x + cd->x1f, y + cd->y0f);\n");
   printf("        glTexCoord2f(cd->s1f, cd->t1f); glVertex2f(x + cd->x1f, y + cd->y1f);\n");
   printf("        glTexCoord2f(cd->s0f, cd->t1f); glVertex2f(x + cd->x0f, y + cd->y1f);\n");
   printf("        // if bilerping, in D3D9 you'll need a half-pixel offset here for 1:1 to behave correct\n");
   printf("        x += cd->advance;\n");
   printf("    }\n");
   printf("    glEnd();\n");
   printf("}\n");
   printf("*/\n\n");

   printf("%s",
      "#ifndef STB_FONTCHAR__TYPEDEF\n"
      "#define STB_FONTCHAR__TYPEDEF\n"
      "typedef struct\n"
      "{\n"
      "    // coordinates if using integer positioning\n"
      "    float s0,t0,s1,t1;\n"
      "    signed short x0,y0,x1,y1;\n"
      "    int   advance_int;\n"
      "    // coordinates if using floating positioning\n"
      "    float s0f,t0f,s1f,t1f;\n"
      "    float x0f,y0f,x1f,y1f;\n"
      "    float advance;\n"
      "} stb_fontchar;\n"
      "#endif\n\n");

   printf("#define STB_FONT_%s_BITMAP_WIDTH        %4d\n", name, packed_width);
   printf("#define STB_FONT_%s_BITMAP_HEIGHT       %4d\n", name, packed_height);
   printf("#define STB_FONT_%s_BITMAP_HEIGHT_POW2  %4d\n\n", name, 1 << stb_log2_ceil(packed_height));
   printf("#define STB_FONT_%s_FIRST_CHAR            32\n", name);
   printf("#define STB_FONT_%s_NUM_CHARS           %4d\n\n", name, numchars);
   printf("#define STB_FONT_%s_LINE_SPACING        %4d\n\n", name, spacing);

   printf("static unsigned int stb__%s_pixels[]={\n", name);
   spaces = 0;
   numbits = 0;
   bitpack=0;

   if (zero_compress) {
      for (j=1; j < packed_height-1; ++j) {
         for (i=1; i < packed_width-1; ++i) {
            int v = packed[j*packed_width+i];
            if (v)
               bitpack |= (1 << numbits);
            ++numbits;
            if (numbits == 32 || (v && numbits > 29)) {
               if (spaces == 0) {
                  printf("    ");
                  spaces += 4;
               }
               printf("0x%08x,", bitpack);
               ++words;
               spaces += 11;
               if (spaces+11 >= 79) {
                  printf("\n");
                  spaces=0;
               }
               bitpack = 0;
               numbits=0;
            }
            if (v) {
               bitpack |= ((v-1) << numbits);
               numbits += 3;
               assert(numbits <= 32);
               if (numbits == 32) {
                  if (spaces == 0) {
                     printf("    ");
                     spaces += 4;
                  }
                  printf("0x%08x,", bitpack);
                  ++words;
                  spaces += 11;
                  if (spaces+11 >= 79) {
                     printf("\n");
                     spaces=0;
                  }
                  bitpack = 0;
                  numbits=0;
               }                  
            }
         }
      }
      if (numbits) {
         if (spaces == 0) {
            printf("    ");
            spaces += 4;
         }
         printf("0x%08x,", bitpack);
         ++words;
      }
   } else {
      // UNTESTED
      // pack the bitmap, 10 pixels per 32-bit word
      for (j=1; j < packed_height-1; ++j) {
         for (i=1; i < packed_width-1; i += 10) {
            int k,bits=0;
            for (k=0; k < 10; ++k)
               if (i+k < packed_width) {
                  bits += packed[j*packed_width + i+k] << (k*3);
               }
            if (spaces == 0) {
               printf("    ");
               spaces += 4;
            }
            printf("0x%08x,", bits);
            ++words;
            spaces += 11;
            if (spaces+11 >= 79) {
               printf("\n");
               spaces=0;
            }
         }
      }
   }
   
   if (spaces) printf("\n");
   printf("};\n\n");

   printf("static signed short stb__%s_x[%d]={ ", name, numchars);
   for (i=0; i < numchars; ++i) {
      printf("%d,", fontchar[glyphmap[i]].x0);
      if (i % 30 == 13)
         printf("\n");
   }
   printf(" };\n");

   printf("static signed short stb__%s_y[%d]={ ", name, numchars);
   for (i=0; i < numchars; ++i) {
      printf("%d,", fontchar[glyphmap[i]].y0+ascent);
      if (i % 30 == 13)
         printf("\n");
   }
   printf(" };\n");

   printf("static unsigned short stb__%s_w[%d]={ ", name, numchars);
   for (i=0; i < numchars; ++i) {
      printf("%d,", fontchar[glyphmap[i]].w);
      if (i % 30 == 13)
         printf("\n");
   }
   printf(" };\n");

   printf("static unsigned short stb__%s_h[%d]={ ", name, numchars);
   for (i=0; i < numchars; ++i) {
      printf("%d,", fontchar[glyphmap[i]].h);
      if (i % 30 == 13)
         printf("\n");
   }
   printf(" };\n");

   printf("static unsigned short stb__%s_s[%d]={ ", name, numchars);
   for (i=0; i < numchars; ++i) {
      printf("%d,", fontchar[glyphmap[i]].s);
      if (i % 20 == 10)
         printf("\n");
   }
   printf(" };\n");

   printf("static unsigned short stb__%s_t[%d]={ ", name, numchars);
   for (i=0; i < numchars; ++i) {
      printf("%d,", fontchar[glyphmap[i]].t);
      if (i % 20 == 10)
         printf("\n");
   }
   printf(" };\n");

   printf("static unsigned short stb__%s_a[%d]={ ", name,numchars);
   for (i=0; i < numchars; ++i) {
      printf("%d,", (int) (fontchar[glyphmap[i]].advance * scale * 16+0.5));
      if (i % 16 == 7)
         printf("\n");
   }
   printf(" };\n\n");

   printf("// Call this function with\n");
   printf("//    font: NULL or array length\n");
   printf("//    data: NULL or specified size\n");
   printf("//    height: STB_FONT_%s_BITMAP_HEIGHT or STB_FONT_%s_BITMAP_HEIGHT_POW2\n", name,name);
   printf("//    return value: spacing between lines\n");
   printf("static void stb_font_%s(stb_fontchar font[STB_FONT_%s_NUM_CHARS],\n", name,name);
   printf("                unsigned char data[STB_FONT_%s_BITMAP_HEIGHT][STB_FONT_%s_BITMAP_WIDTH],\n", name,name);
   printf("                int height)\n");
   printf("{\n");
   printf("    int i,j;\n");

   if (zero_compress) {
      printf("    if (data != 0) {\n");
      printf("        unsigned int *bits = stb__%s_pixels;\n", name);
      printf("        unsigned int bitpack = *bits++, numbits = 32;\n");
      printf("        for (i=0; i < STB_FONT_%s_BITMAP_WIDTH*height; ++i)\n", name);
      printf("            data[0][i] = 0;  // zero entire bitmap\n");
      printf("        for (j=1; j < STB_FONT_%s_BITMAP_HEIGHT-1; ++j) {\n", name);
      printf("            for (i=1; i < STB_FONT_%s_BITMAP_WIDTH-1; ++i) {\n", name);
      printf("                unsigned int value;\n");
      printf("                if (numbits==0) bitpack = *bits++, numbits=32;\n");
      printf("                value = bitpack & 1;\n");
      printf("                bitpack >>= 1, --numbits;\n");
      printf("                if (value) {\n");
      printf("                    if (numbits < 3) bitpack = *bits++, numbits = 32;\n");
      printf("                    data[j][i] = (bitpack & 7) * 0x20 + 0x1f;\n");
      printf("                    bitpack >>= 3, numbits -= 3;\n");
      printf("                } else {\n");
      printf("                    data[j][i] = 0;\n");
      printf("                }\n");
      printf("            }\n");
      printf("        }\n");
      printf("    }\n\n");
   } else {
      // UNTESTED
      printf("    if (data != 0) {\n");
      printf("        unsigned int *bits = stb__%s_pixels;\n", name);
      printf("        unsigned int bitpack = *bits++, numbits = 30;\n");
      printf("        for (i=0; i < STB_FONT_%s_BITMAP_WIDTH*height; ++i)\n", name);
      printf("            data[0][i] = 0;  // zero entire bitmap\n");
      printf("        for (j=1; j < STB_FONT_%s_BITMAP_HEIGHT-1; ++j) {\n", name);
      printf("            numbits=0;\n");
      printf("            for (i=1; i < STB_FONT_%s_BITMAP_WIDTH-1; ++i) {\n", name);
      printf("                unsigned int value;\n");
      printf("                if (numbits==0) bitpack = *bits++, numbits=30;\n");
      printf("                value = bitpack & 7;\n");
      printf("                data[j][i] = (value<<5) + (value << 2) + (value >> 1);\n");
      printf("                bitpack >>= 3, numbits -= 3;\n");
      printf("            }\n");
      printf("        }\n");
      printf("    }\n\n");
   }

   printf("    // build font description\n");
   printf("    if (font != 0) {\n");
   printf("        float recip_width = 1.0f / STB_FONT_%s_BITMAP_WIDTH;\n", name);
   printf("        float recip_height = 1.0f / height;\n");
   printf("        for (i=0; i < STB_FONT_%s_NUM_CHARS; ++i) {\n", name);
   printf("            // pad characters so they bilerp from empty space around each character\n");
   printf("            font[i].s0 = (stb__%s_s[i]) * recip_width;\n", name);
   printf("            font[i].t0 = (stb__%s_t[i]) * recip_height;\n", name);
   printf("            font[i].s1 = (stb__%s_s[i] + stb__%s_w[i]) * recip_width;\n", name, name);
   printf("            font[i].t1 = (stb__%s_t[i] + stb__%s_h[i]) * recip_height;\n", name, name);
   printf("            font[i].x0 = stb__%s_x[i];\n", name);
   printf("            font[i].y0 = stb__%s_y[i];\n", name);
   printf("            font[i].x1 = stb__%s_x[i] + stb__%s_w[i];\n", name, name);
   printf("            font[i].y1 = stb__%s_y[i] + stb__%s_h[i];\n", name, name);
   printf("            font[i].advance_int = (stb__%s_a[i]+8)>>4;\n", name);
   printf("            font[i].s0f = (stb__%s_s[i] - 0.5f) * recip_width;\n", name);
   printf("            font[i].t0f = (stb__%s_t[i] - 0.5f) * recip_height;\n", name);
   printf("            font[i].s1f = (stb__%s_s[i] + stb__%s_w[i] + 0.5f) * recip_width;\n", name, name);
   printf("            font[i].t1f = (stb__%s_t[i] + stb__%s_h[i] + 0.5f) * recip_height;\n", name, name);
   printf("            font[i].x0f = stb__%s_x[i] - 0.5f;\n", name);
   printf("            font[i].y0f = stb__%s_y[i] - 0.5f;\n", name);
   printf("            font[i].x1f = stb__%s_x[i] + stb__%s_w[i] + 0.5f;\n", name, name);
   printf("            font[i].y1f = stb__%s_y[i] + stb__%s_h[i] + 0.5f;\n", name, name);
   printf("            font[i].advance = stb__%s_a[i]/16.0f;\n", name);
   printf("        }\n");
   printf("    }\n");
   printf("}\n\n");

   printf("#ifndef STB_SOMEFONT_CREATE\n");
   printf("#define STB_SOMEFONT_CREATE              stb_font_%s\n", name);
   printf("#define STB_SOMEFONT_BITMAP_WIDTH        STB_FONT_%s_BITMAP_WIDTH\n",name);
   printf("#define STB_SOMEFONT_BITMAP_HEIGHT       STB_FONT_%s_BITMAP_HEIGHT\n",name);
   printf("#define STB_SOMEFONT_BITMAP_HEIGHT_POW2  STB_FONT_%s_BITMAP_HEIGHT_POW2\n",name);
   printf("#define STB_SOMEFONT_FIRST_CHAR          STB_FONT_%s_FIRST_CHAR\n",name);
   printf("#define STB_SOMEFONT_NUM_CHARS           STB_FONT_%s_NUM_CHARS\n",name);
   printf("#define STB_SOMEFONT_LINE_SPACING        STB_FONT_%s_LINE_SPACING\n", name);
   printf("#endif\n\n");

#ifdef WRITE_FILE
   for (i=0; i < packed_width * packed_height; ++i) {
      if (zero_compress)
         // unpack with same math
         packed[i] = (packed[i] ? 0x20*(packed[i]-1) + 0x1f : 0);
      else
         packed[i] = (packed[i] << 5) | (packed[i] << 2) | (packed[i] >> 1);
      // gamma correct, use gamma=2.2 rather than sRGB out of laziness
      packed[i] = (unsigned char) (pow(packed[i]/255.0, 1.0f/2.2f)*255);
   }
   stbi_write_png(stb_sprintf("stb_font_%s.png", name), packed_width, packed_height, 1, packed, packed_width);
#endif

   return 0;
}

// 591
