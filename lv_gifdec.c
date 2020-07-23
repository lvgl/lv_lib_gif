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
static lv_color_t cbuf[320*320];
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

    uint8_t buf[gif->width * gif->height * 3];

    uint32_t i;
    for(i = 0; i < gif->width * gif->height; i++) {
        cbuf[i] = lv_color_make(buf[i * 3], buf[i * 3 + 1], buf[i * 3 + 2]);
    }

    imgdsc.data = cbuf;
    imgdsc.header.always_zero = 0;
    imgdsc.header.cf = LV_IMG_CF_INDEXED_8BIT;
    imgdsc.header.h = gif->height;
    imgdsc.header.w = gif->width;

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

    uint8_t buf[gif->width * gif->height * 3];

    gd_get_frame(gif);
    gd_render_frame(gif, buf);

    counter = gif->gce.delay;

    uint32_t i;
    for(i = 0; i < gif->width * gif->height; i++) {
        cbuf[i] = lv_color_make(buf[i * 3], buf[i * 3 + 1], buf[i * 3 + 2]);
    }

    lv_obj_invalidate(img);
}
