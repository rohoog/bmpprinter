#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

/* block-sextants from the unicode block "symbols for legacy computing" */
char *sexts[]={
" ","🬀","🬁","🬂","🬃","🬄","🬅","🬆","🬇","🬈","🬉","🬊","🬋","🬌","🬍","🬎",
"🬏","🬐","🬑","🬒","🬓","▌","🬔","🬕","🬖","🬗","🬘","🬙","🬚","🬛","🬜","🬝",
"🬞","🬟","🬠","🬡","🬢","🬣","🬤","🬥","🬦","🬧","▐","🬨","🬩","🬪","🬫","🬬",
"🬭","🬮","🬯","🬰","🬱","🬲","🬳","🬴","🬵","🬶","🬷","🬸","🬹","🬺","🬻","█"
};

void printbytes(uint8_t b[3], int j)
{
	int i;
	uint8_t c=0;
	for (i=0;i<8&&j>0;i++) {
		c|=((b[0]>>6)&2)|((b[1]>>4)&8)|((b[2]>>2)&32);
		b[0]<<=1;b[1]<<=1;b[2]<<=1;
		j--;
		if (i&1) {
			fputs(sexts[c], stdout);
			c=0;
		} else {
			c>>=1;
		}
	}
	if (i&1) {
		fputs(sexts[c], stdout);
	}
}

struct __attribute__((packed)) bmpheader {
	char signature[2];
	uint32_t filesize;
	uint16_t reserved[2];
	uint32_t pixels_offset;
};
#define HDR ((struct bmpheader *)buf)
struct __attribute__((packed)) v5header {
	uint32_t dibheadersize;
	uint32_t imgwidth;
	uint32_t imgheight;
	uint16_t nrplanes;
	uint16_t bpp;
	uint32_t compression;
	uint32_t imgsize;
};
#define V5HDR ((struct v5header *)(buf+sizeof(struct bmpheader)))

int main(int argc, char *argv[])
{
	uint8_t *buf;
	int fd;
	uint32_t size;
	uint16_t scanlinew;
	uint8_t *bmap, *pxls[3];

	fd=open(argv[1], O_RDONLY);
	if (fd<0) {
		perror(argv[1]);
		return 1;
	}
	buf=malloc(14);
	read(fd, buf, sizeof(struct bmpheader));
	if (memcmp(HDR->signature, "BM", 2)) {
		fprintf(stderr, "%s is not a BMP file\n", argv[1]);
		return 1;
	}
	size=HDR->filesize;
	buf=realloc(buf, size);
	read(fd, buf+14, size-14);
	close(fd);
	printf("image width %d height %d\n", V5HDR->imgwidth, V5HDR->imgheight);
	printf("%d bits per pixel\n", V5HDR->bpp);
	if (V5HDR->bpp != 1) {
		fprintf(stderr, "Not monochrome 1bpp BMP\n");
		return 1;
	}
	scanlinew = ((V5HDR->imgwidth+7)/8 + 3)&~3;
	printf("scan line is %d bytes long\n", scanlinew);
	bmap=buf+HDR->pixels_offset;
	{
		int i,j;
		uint8_t zero[scanlinew];
		memset(zero,0,scanlinew);
		for (j=V5HDR->imgheight;j>0;) {
			pxls[0]=bmap+(--j)*scanlinew;
			pxls[1]=j>0?bmap+(--j)*scanlinew:zero;
			pxls[2]=j>0?bmap+(--j)*scanlinew:zero;
			for (i=0;i<V5HDR->imgwidth;i+=8) {
				printbytes((uint8_t[]){pxls[0][i/8],pxls[1][i/8],pxls[2][i/8]}, V5HDR->imgwidth-i);
			}
			putchar('\n');
		}
	}
	return 0;
}
