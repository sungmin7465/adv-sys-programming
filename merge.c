#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>

int readaline_and_out(FILE *fin, FILE *fout);

#define MAX_STRLEN 100

int
main(int argc, char *argv[])
{
    FILE *file1, *file2, *fout;
    int eof1 = 0, eof2 = 0;
    long line1 = 0, line2 = 0, lineout = 0;
    struct timeval before, after;
    int duration;
    int ret = 1;

    if (argc != 4) {
        fprintf(stderr, "usage: %s file1 file2 fout\n", argv[0]);
        goto leave0;
    }
    if ((file1 = fopen(argv[1], "rt")) == NULL) {
        perror(argv[1]);
        goto leave0;
    }
    if ((file2 = fopen(argv[2], "rt")) == NULL) {
        perror(argv[2]);
        goto leave1;
    }
    if ((fout = fopen(argv[3], "wt")) == NULL) {
        perror(argv[3]);
        goto leave2;
    }
    
    gettimeofday(&before, NULL);
    do {
        if (!eof1) {
            if (!readaline_and_out(file1, fout)) {
                line1++; lineout++;
            } else
                eof1 = 1;
        }
        if (!eof2) {
            if (!readaline_and_out(file2, fout)) {
                line2++; lineout++;
            } else
                eof2 = 1;
        }
    } while (!eof1 || !eof2);
    gettimeofday(&after, NULL);
    
    duration = (after.tv_sec - before.tv_sec) * 1000000 + (after.tv_usec - before.tv_usec);
    printf("Processing time = %d.%06d sec\n", duration / 1000000, duration % 1000000);
    printf("File1 = %ld, File2= %ld, Total = %ld Lines\n", line1, line2, lineout);
    ret = 0;
    
leave3:
    fclose(fout);
leave2:
    fclose(file2);
leave1:
    fclose(file1);
leave0:
    return ret; 
}

/* Read a line from fin and write it to fout */
/* return 1 if fin meets end of file */
	//lenth of line  <= 100
int readaline_and_out(FILE *fin, FILE *fout)
{    
    int i = 0, ch, count = 0;
    int tmp_str[MAX_STRLEN] = {0,};

    do {
        if ((tmp_str[count] = fgetc(fin)) == EOF) {
            if (!count)
                return 1;
            else {
                tmp_str[count] = 0x0a; 
                break;
            }
        }

    } while (tmp_str[count++] != 0x0a);

   for(i=count; i>=0; i--) {
	fputc(tmp_str[i], fout);
   }

    return 0;
}

