#include <am.h>
#include <nemu.h>
#include <string.h>

static inline int min(int x, int y) {
  return (x < y) ? x : y;                                               
}

#define SYNC_ADDR (VGACTL_ADDR + 4)

void __am_gpu_init() {
}

void __am_gpu_config(AM_GPU_CONFIG_T *cfg) {
	uint32_t info = inl(VGACTL_ADDR);
  *cfg = (AM_GPU_CONFIG_T) {
    .present = true, .has_accel = false,
    .width = info >> 16, .height = info & 0x0000ffff,
    .vmemsz = 0
  };
}

void __am_gpu_fbdraw(AM_GPU_FBDRAW_T *ctl) {
	int x = ctl->x, y = ctl->y, w = ctl->w, h = ctl->h;
	if (w == 0 || h == 0) return;
	uint32_t *pixels = ctl->pixels;
	int W = io_read(AM_GPU_CONFIG).width;
    int H = io_read(AM_GPU_CONFIG).height;
	int cp_bytes = sizeof(uint32_t) * min(w, W - x);
    uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;
    for (int j = 0; j < h && y + j < H; j++){
      memcpy(&fb[(y + j) * W + x], pixels, cp_bytes);
      pixels += w;
    }
	if (ctl->sync) {
		outl(SYNC_ADDR, 1);
  }
}

void __am_gpu_status(AM_GPU_STATUS_T *status) {
  status->ready = true;
}
