#include "mic.h"

void Mic::record() {
    // if(hx_drv_mic_timestamp_get(&time_cur) == HX_DRV_LIB_PASS) {
    //     hx_drv_uart_print("get timestamp success\n");
    // } else {
    //     hx_drv_uart_print("get timestamp fail\n");
    // }
    hx_drv_mic_timestamp_get(&time_cur);
    // hx_drv_uart_print("%d\n", time_cur);
    if(time_prev == time_cur) return;

    // hx_drv_uart_print("%d\n", queue_index * AUD_BLK_100MS_SZ);
    // hx_drv_uart_print("%d\t%d\t%d\n",
    //     audio_circular_queue[queue_index * AUD_BLK_100MS_SZ + 10].left,
    //     *(uint8_t*)&audio_circular_queue[queue_index * AUD_BLK_100MS_SZ + 10].left,
    //     *((uint8_t*)(&audio_circular_queue[queue_index * AUD_BLK_100MS_SZ + 10].left) + 1)
    // );
    // hx_drv_uart_print("%d\t%d\n", audio_circular_queue[queue_index * AUD_BLK_100MS_SZ + 10].left, audio_circular_queue[queue_index * AUD_BLK_100MS_SZ + 10].right);
    // memcpy(
    //     (void*)&audio_circular_queue[queue_index * AUD_BLK_100MS_SZ],
    //     (void*)slt_audio_config.data_address,
    //     slt_audio_config.data_size*sizeof(uint8_t)
    // );

    if(hx_drv_mic_capture_dual(&slt_audio_config) == HX_DRV_LIB_PASS)
        memcpy(
            (void*) &audio_circular_queue[queue_index * AUD_BLK_100MS_SZ],
            (void*) slt_audio_config.data_address,
            slt_audio_config.data_size*sizeof(uint8_t)
        );
    // hx_drv_uart_print("%d\t%d\n",
    //     audio_circular_queue[queue_index * AUD_BLK_100MS_SZ].left,
    //     audio_circular_queue[queue_index * AUD_BLK_100MS_SZ].right);
    // for(int i = 0; i < AUD_BLK_100MS_SZ; i++) {
    //     hx_drv_uart_print("%d\t%d\n",
    //     audio_circular_queue[queue_index * AUD_BLK_100MS_SZ + i].left,
    //     audio_circular_queue[queue_index * AUD_BLK_100MS_SZ + i].right);
    // }
    queue_index++;
    queue_index %= QUEUE_LEN;
    // hx_drv_uart_print("%d\n", queue_index);
    time_prev = time_cur;
}
