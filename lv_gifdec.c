/**
 * @file lv_gifenc.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "lvgl/lvgl.h"
#include "gifdec.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void next_frame_tesk_cb(lv_task_t * t);

/**********************
 *  STATIC VARIABLES
 **********************/
static gd_GIF *gif;
static uint8_t buf[256*4 + 320 *320];
static lv_obj_t * img;
static lv_img_dsc_t imgdsc;

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

lv_obj_t * lv_gif_create(lv_obj_t * parent)
{

    gif = gd_open_gif("./a.gif");

    printf("canvas size: %ux%u\n", gif->width, gif->height);
    printf("number of colors: %d\n", gif->palette->size);


    imgdsc.data = buf;
    imgdsc.header.always_zero = 0;
    imgdsc.header.cf = LV_IMG_CF_INDEXED_8BIT;
    imgdsc.header.h = gif->height;
    imgdsc.header.w = gif->width;

    uint32_t i;
    for(i = 0; i < 256; i++) {
        lv_color_t c = lv_color_make(gif->palette->colors[3 * i], gif->palette->colors[3 * i + 1], gif->palette->colors[3 * i + 2]);
        lv_img_buf_set_palette(&imgdsc, i, c);
    }

    img = lv_img_create(parent, NULL);
    lv_img_set_src(img, &imgdsc);

    lv_task_create(next_frame_tesk_cb, 10, LV_TASK_PRIO_HIGH, NULL);


    return NULL;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static void next_frame_tesk_cb(lv_task_t * t)
{
    static uint32_t counter = 0;
    if(counter > 0) {
        counter --;
        return;
    }

    gd_get_frame(gif);
    gd_render_frame(gif, buf+256*4);

    counter = gif->gce.delay;

    lv_obj_invalidate(img);
}
