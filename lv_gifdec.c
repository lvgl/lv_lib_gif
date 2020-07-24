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
static void next_frame_task_cb(lv_task_t * t);

/**********************
 *  STATIC VARIABLES
 **********************/
static gd_GIF *gif;
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

    imgdsc.data = gif->palette;
    imgdsc.header.always_zero = 0;
    imgdsc.header.cf = LV_IMG_CF_INDEXED_8BIT;
    imgdsc.header.h = gif->height;
    imgdsc.header.w = gif->width;

    img = lv_img_create(parent, NULL);
    lv_img_set_src(img, &imgdsc);

    lv_task_create(next_frame_task_cb, 10, LV_TASK_PRIO_HIGH, NULL);

    return NULL;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static void next_frame_task_cb(lv_task_t * t)
{
    static uint32_t counter = 0;
    if(counter > 0) {
        counter --;
        return;
    }

    gd_get_frame(gif);
    lv_obj_invalidate(img);

    counter = gif->gce.delay;
}
