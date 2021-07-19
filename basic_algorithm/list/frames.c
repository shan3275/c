/**************************************************************
* Copyright (C) 2014-2021 All rights reserved.
* @Version: 1.0
* @Created: 2021-07-19 21:59
* @Author: SamLiu - shan3275@gmail.com
* @Description: 
*
* @History: 
**************************************************************/
#include <stdio.h>

#include "frames.h"
#include "list.h"

/* alloc frame */
int alloc_frame(List *frames)
{
    int frame_number, *data;

    if (list_size(frames) == 0) {
        /* Return that there are no frames avaliable */
        return -1;
    } else {
        if (list_rem_next(frames, NULL, (void **)&data) != 0) {
            /* Return that a frame could not be retrieved */
            return -1;
        } else {
            /* Store the number of the avaiable frame. */
            frame_number = *data;
            free(data);
        }
    }

    return frame_number;
}

/* free frame */
int free_frame(List *frames, int frame_number)
{
    int *data;

    /* Allocate storage for the frame number */
    if ((data = (int *)malloc(sizeof(int))) == NULL) {
        return -1;
    }

    /* Put the frame back in the list of avaliable frames */
    *data = frame_number;

    if (list_ins_next(frames, NULL, data) != 0) {
        return -1;
    }

    return 0;
}