#include <switch.h>
// #include <g711.h>
// #include <libteletone.h>

int main(int argc, char **argv)
{
    teletone_dtmf_detect_state_t dtmf_detect = {0};
    int rate = 8000;
    int fd = -1;
    unsigned char data[8000 * 2];
    int len = 0;
    char digit;
    unsigned int duration;

    printf("hi\n");
    teletone_dtmf_detect_init(&dtmf_detect, rate);

    fd = open(argv[1], O_RDONLY);

    if (fd < 0) {
        printf("error\n");
        return 1;
    }

    while (1) {
        len = read(fd, data, 320);

        if (len <=0) break;

        teletone_hit_type_t ret = teletone_dtmf_detect(&dtmf_detect, (short *)data, len / 2);
        // printf("ret = %d\n", ret);

        if (ret == TT_HIT_END) {
            teletone_dtmf_get(&dtmf_detect, &digit, &duration);
            printf("DTMF = %c\n", digit);
        }
    }

    close(fd);
    printf("\n");
}
