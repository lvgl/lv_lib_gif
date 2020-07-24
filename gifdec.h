#ifndef GIFDEC_H
#define GIFDEC_H

#include <stdint.h>
#include <sys/types.h>
#include <lvgl/lvgl.h>

typedef struct gd_GCE {
    uint16_t delay;
    uint8_t tindex;
    uint8_t disposal;
    int input;
    int transparency;
} gd_GCE;

typedef struct gd_GIF {
    lv_fs_file_t * fd;
    off_t anim_start;
    uint16_t width, height;
    uint16_t depth;
    uint16_t loop_count;
    gd_GCE gce;
    uint16_t fx, fy, fw, fh;
    uint16_t gct_size;
    uint8_t global_palette[256 * 3];
    uint8_t bgindex;
    uint8_t *palette;
    uint8_t *canvas;
} gd_GIF;

gd_GIF *gd_open_gif(const char *fname);
int gd_get_frame(gd_GIF *gif);
void gd_rewind(gd_GIF *gif);
void gd_close_gif(gd_GIF *gif);

#endif /* GIFDEC_H */
