#include <switch.h>
// #include <g711.h>
// #include <libteletone.h>

int main(int argc, char **argv)
{
    teletone_dtmf_detect_state_t dtmf_detect;
    int rate = 8000;
    int fd = -1;
    unsigned char data[8000 * 2];
    int len = 0;
    char digit;
    unsigned int duration;

    printf("hi\n");
    teletone_dtmf_detect_init(&dtmf_detect, rate);

    fd = open(argv[1], O_RDONLY);

    if (fd < 0) return 1;

    while (1) {
        len = read(fd, data, sizeof(data));

        if (len <=0) break;

        if (teletone_dtmf_detect(&dtmf_detect, (short *)data, len / 2) == TT_HIT_END) {
            teletone_dtmf_get(&dtmf_detect, &digit, &duration);
            printf("%c ", digit);
        }

    }

    close(fd);
    printf("\n");
}