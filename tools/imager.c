#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

#define MAX_IN_COLS (128)
#define MAX_IN_ROWS (32)
#define MAX_IN_SIZE                                                                                \
    (MAX_IN_ROWS * (MAX_IN_COLS + 1)) /* The newline character requires an extra col */
#define MAX_OUT_COLS MAX_IN_COLS
#define MAX_OUT_PAGES (MAX_IN_ROWS / 8)

int main(int argc, char* argv[])
{
    size_t page                                    = 0;
    size_t col                                     = 0;
    size_t row                                     = 0;
    uint8_t out_image[MAX_OUT_COLS][MAX_OUT_PAGES] = {0};
    char buff[MAX_IN_SIZE]                         = {0};
    size_t counter                                 = 0;
    int rows                                       = 0;
    int cols                                       = 0;

    if (argc < 4)
    {
        printf("Missing args");
        return 1;
    }
    rows = atoi(argv[2]);
    cols = atoi(argv[3]);

    int fd = open(argv[1], O_RDONLY);
    if (fd < 0)
    {
        perror("open");
        return 2;
    }
    ssize_t bytes_read = read(fd, buff, MAX_IN_SIZE);
    if (bytes_read < 0)
    {
        perror("read");
        return 3;
    }
    printf("Read: %zd bytes\n", bytes_read);

    for (size_t b = 0; b < bytes_read && row < rows; b++)
    {
        if (buff[b] == '\n')
        {
            // End of line reached
            row++;
            continue;
        }
        col  = counter % cols;
        page = row / 8;
        out_image[col][page] |= (buff[b] == '0') << (row % 8);
        printf(
            "Page: %zu Col %zu Val %d byte: %d\n",
            page,
            col,
            buff[b] == '0',
            out_image[col][page]);
        counter++;
    }

    printf("{\n");
    for (size_t p = 0; p < rows / 8; p++)
    {
        printf("{");
        for (size_t c = 0; c < cols; c++)
        {
            printf("0x%02x, ", out_image[c][p]);
        }
        printf("},\n");
    }
    printf("}\n");
    return 0;
}
