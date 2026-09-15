#define _POSIX_C_SOURCE 200809L
#define _DEFAULT_SOURCE

#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

#define W 80
#define H 28

static volatile int running = 1;
static void on_sigint(int s) { (void)s; running = 0; }

/* Classic implicit heart surface:
 * (x² + 9/4 y² + z² - 1)³ - x² z³ - 9/80 y² z³ = 0
 *
 * A point (x, y, z) is "inside" the heart when this value is <= 0. */

static float heart (float x, float y, float z) {
  float a = x * x + 2.25f * y * y + z * z - 1.0f;
  return a * a * a - x * x * z * z * z - 0.1125f * y * y * z * z * z;
}

int main(void) {
    struct sigaction sa;
    sa.sa_handler = on_sigint;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);

    /* Clear the screen and hide cursor */
    printf("\033[2J\033[?25l");
    fflush(stdout);

    float A = 0.0f; // rotation angle around Y axis
    float T = 0.0f; // time, used for heartbeat

    while (running) {
        printf("\033[H"); /* cursor home (flicker-free) */

        /* Big buffer for the whole frame to minimize flicker */
        char buf[H * (W * 24 + 2) + 512];
        int p = 0;

        /* Title */
        p += sprintf(buf + p,
            "\033[1;38;2;255;130;170m"
            "                  ♥  S P I N N I N G  H E A R T ♥ \033[0m\n");

        float cA = cosf(A), sA = sinf(A);
        float beat = 1.0f + 0.06 * sinf(T * 4.0f); /* heartbeat pulse */

        int bodyH = H - 2; /* rows used for the heart itself */

        for (int j = 0; j < bodyH; j++) {
            /* Map screen row to world y (inverted, because screen y grows down) */
            float sy = -(j - bodyH / 2.0f) / (bodyH / 3.0f) / beat;

            for (int i = 0; i < W; i++) {
                /* Map screen column to world x. IMPORTANT: this must span the
                 * same world-space range as the depth raycast below (+-1.6,
                 * from k*0.02 with k in [-80,80]). x and depth get mixed
                 * together by the rotation, so if their scales don't match,
                 * the rotation isn't rigid, it's rotation+shear, which is
                 * what makes the heart wobble/distort instead of spinning
                 * cleanly. */
                float sx = (i - W / 2.0f) / (W / 2.0f) * 1.6f / beat;

                int hit = 0;
                float light = 0.0f;

                /* Scan depth from far to near (ray casting) */
                for (int k = -80; k <= 80; k++) {
                    float z = k * 0.02f / beat;

                    /* Rotate (sx, z) around the vertical axis by angle A.
                     * Note: in heart(x,y,z), only z is cubed, z is the axis
                     * that must carry the notch(top)/point(bottom) asymmetry,
                     * so the "fixed" screen-vertical coordinate (sy) has to be
                     * passed as z, not y. The rotated depth term goes in y
                     * instead (x and y are both square-only / symmetric, so
                     * it's fine for them to mix under the spin). */
                    float rx = sx * cA - z * sA;
                    float ry = sx * sA + z * cA;
                    float rz = sy;

                    if (heart(rx, ry, rz) <= 0.0f) {
                        /* Approximate surface normal via central differences */
                        float e = 0.01f;
                        float nx = heart(rx + e, ry, rz) - heart(rx - e, ry, rz);
                        float ny = heart(rx, ry + e, rz) - heart(rx, ry - e, rz);
                        float nz = heart(rx, ry, rz + e) - heart(rx, ry, rz - e);

                        float L = sqrt(nx * nx + ny * ny + nz * nz) + 1e-6f;
                        nx /= L; ny /= L; nz /= L;

                        /* Diffuse lighting from upper-right-front */
                        float d = nx * 0.4f + ny * 0.7f + nz * 0.5f;
                        if (d < 0.0f) d = 0.0f;
                        
                        /* Add a little rim lighting for the edge */
                        float rim = 1.0f - nz;
                        if (rim < 0.0f) rim = 0.0f;
                        d = d * 0.85f + rim * rim * 0.25f;
                        if (d > 1.0f) d = 1.0f;

                        light = d;
                        hit   = 1;
                        break; /* first hit wins (frontmost surface) */
                    }
                }

                if (hit) {
                    /* Dard red in shadow -> bright pink in highlight */
                    int r = 90 + (int)(light * 165);
                    int g = 10 + (int)(light * 40);
                    int b = 30 + (int)(light * 70);
                    p += sprintf(buf + p,
                                 "\033[48;2;%d;%d;%dm \033[0m",
                                 r, g, b);
                } else {
                    buf[p++] = ' ';
                }
            }
            buf[p++] = '\n';
        }

        /* Footer */
        p += sprintf(buf + p,
             "\033[38;2;170;172;178m"
             "                   press Ctrl+C to exit\033[0m\n");
        buf[p] = '\0';
        fputs(buf, stdout);
        fflush(stdout);

        A += 0.02f; /* Spin */
        T += 0.05f; /* heartbeat clock */
        usleep(35000); /* arround 28 fps */
    }

    /* Restore terminal */
    printf("\033[?25h\033[0m\033[2J\033[H");
    return 0;
}
