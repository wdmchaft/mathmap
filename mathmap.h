/* -*- c -*- */

/*
 * MathMap.h
 *
 * MathMap
 *
 * Copyright (C) 1997-2004 Mark Probst
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef __MATHMAP_H__
#define __MATHMAP_H__

#include "gtypes.h"
#include "exprtree.h"
#include "userval.h"
#include "internals.h"
#include "vars.h"
#include "cgen.h"
#include "postfix.h"
#include "color.h"

#ifdef GIMP
#include <libgimp/gimp.h>
#endif

#define MATHMAP_DATE          "April 2004"

typedef struct _mathmap_t
{
    userval_info_t *userval_infos;
    variable_t *variables;
    internal_t *internals;

    exprtree *exprtree;

    int num_uservals;

    int is_native;

    initfunc_t initfunc;
    void *module_info;

    postfix *expression;
    int exprlen;
} mathmap_t;

/*
 * this variable is set by the compiler.  it's ok that it is global
 * because the compiler is non-reentrant (which is ok because it's
 * fast).
 */
extern mathmap_t *the_mathmap;

#ifndef OPENSTEP
extern color_t gradient_samples[USER_GRADIENT_POINTS];
#endif

#define EDGE_BEHAVIOUR_COLOR          1	/* all three used in new_template.c */
#define EDGE_BEHAVIOUR_WRAP           2
#define EDGE_BEHAVIOUR_REFLECT        3

typedef struct _mathmap_invocation_t
{
    mathmap_t *mathmap;

    userval_t *uservals;
    tuple_t *variables;
    tuple_t *internals;

    int antialiasing;
    int supersampling;

    int output_bpp;

    int edge_behaviour;
    color_t edge_color;

    int current_frame;
    int origin_x, origin_y;
    int img_width, img_height;
    float middle_x, middle_y;
    float image_R, image_X, image_Y, image_W, image_H;
    float scale_x, scale_y;

    float current_x, current_y, current_r, current_a, current_t;

    tuple_t *stack;
    int stackp;

    int exprp;

    int row_stride;
    volatile int num_rows_finished;
    int uses_ra;

    mathfunc_t mathfunc;

    void *xy_vars;
    void *y_vars;
} mathmap_invocation_t;

#ifndef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif
#ifndef MAX
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#endif

#ifndef M_PI
#define M_PI     3.14159265358979323846
#endif

void unload_mathmap (mathmap_t *mathmap);
void free_mathmap (mathmap_t *mathmap);
void free_invocation (mathmap_invocation_t *invocation);

int check_mathmap (char *expression);
mathmap_t* compile_mathmap (char *expression, FILE *template);
mathmap_invocation_t* invoke_mathmap (mathmap_t *mathmap, mathmap_invocation_t *template, int img_width, int img_height);
void call_invocation (mathmap_invocation_t *invocation, int first, int last, unsigned char *p);

void carry_over_uservals_from_template (mathmap_invocation_t *invocation, mathmap_invocation_t *template);

void update_image_internals (mathmap_invocation_t *invocation);

color_t mathmap_get_pixel (mathmap_invocation_t *invocation, int drawable_index, int frame, int x, int y);
color_t mathmap_get_fast_pixel (mathmap_invocation_t *invocation, int drawable_index, int x, int y);

#ifdef GIMP
void user_value_changed (void);

int alloc_input_drawable (GimpDrawable *drawable);
void free_input_drawable (int index);
GimpDrawable* get_input_drawable (int index);

/* GIMP 1.2/2.0 compatiblity macros */
#ifndef GIMP2
#define DRAWABLE_ID(d)     ((d)->id)
#define gimp_image_undo_group_start       gimp_undo_push_group_start
#define gimp_image_undo_group_end         gimp_undo_push_group_end
#define gimp_drawable_set_name            gimp_layer_set_name
#else
#define DRAWABLE_ID(d)     ((d)->drawable_id)
#endif

#endif

#endif
