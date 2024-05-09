#include <string.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define DEBUG 0

#include "textype.h"

static unsigned short tex_type;

static inline unsigned short u16flipendian(const unsigned short in)
{
	return (((in & 0xFF) << 8) | ((in & 0xFF00) >> 8));
}

static void rgba16_convert_and_write(const char *inpath, const char *outpath)
{
	/* convert */
	int w, h, c;
	stbi_uc *pixels_in = stbi_load(inpath, &w, &h, &c, 4);
	unsigned short *pxls = malloc(w * h * sizeof(*pxls));

	for (int i = 0; i < w * h; i++)
	{
		unsigned char ro = pixels_in[i * c + 0] >> 3;
		unsigned char go = pixels_in[i * c + 1] >> 3;
		unsigned char bo = pixels_in[i * c + 2] >> 3;
		unsigned char ao = pixels_in[i * c + 3] >> 7;

		pxls[i] = u16flipendian((ro << 11) |
				(go << 6) | (bo << 1) | (ao & 1));
	}

	stbi_image_free(pixels_in);

	/* write */
	FILE *file = fopen(outpath, "wb");

	if (file)
	{
		fclose(file);
		remove(outpath);
		file = fopen(outpath, "wb");
	}

	unsigned short type_out = u16flipendian(tex_type);
	unsigned short width_out = u16flipendian(w);
	unsigned short height_out = u16flipendian(h);

	fwrite(&type_out, sizeof(type_out), 1, file);
	fwrite(&width_out, sizeof(width_out), 1, file);
	fwrite(&height_out, sizeof(height_out), 1, file);
	fwrite(pxls, sizeof(*pxls), w * h, file);
	fclose(file);
}

static void ci8_convert_and_write(const char *inpath, const char *outpath)
{
	/* convert */
	int w, h, c;
	stbi_uc *pixels_in = stbi_load(inpath, &w, &h, &c, 4);

	if (!pixels_in)
	{
		fprintf(stderr, "Failed to load '%s' into STBImage.\n", inpath);
		exit(EXIT_FAILURE);
	}

	unsigned short *pixels_conv = malloc(w * h * sizeof(*pixels_conv));
	unsigned char tlut_size = 0;
	unsigned short *tlut = malloc(0);
	unsigned char *indis = malloc(w * h);

	for (int i = 0; i < w * h; i++)
	{
		unsigned char ro = pixels_in[i * c + 0] >> 3;
		unsigned char go = pixels_in[i * c + 1] >> 3;
		unsigned char bo = pixels_in[i * c + 2] >> 3;
		unsigned char ao = pixels_in[i * c + 3] >> 7;

		pixels_conv[i] = u16flipendian((ro << 11) |
				(go << 6) | (bo << 1) | (ao << 0));

		if (!i)
			goto add_new;

		int tlut_size_last = tlut_size;
		int tlut_found_color = 0;

		for (int j = 0; j < tlut_size_last; j++)
		{
			if (pixels_conv[i] == tlut[j])
			{
				tlut_found_color = 1;
				indis[i] = j;
				break;
			}
		}

#if DEBUG
		printf("%d (%dx%d): 0x%.4X ", i,
		       i % w, i / w, pixels_conv[i]);
#endif

		if (tlut_found_color)
		{
#if DEBUG
			printf("\n");
#endif
			continue;
		}

#if DEBUG
		printf("NEW COLOR! (found %d)\n", tlut_size + 1);
#endif

add_new:
		tlut = realloc(tlut, ++tlut_size * sizeof(*tlut));
		if (!tlut_size)
		{
			fprintf(stderr,
				"'%s' ERROR: tlut_size rolled over 0xFF at "
				"'%dx%d (%.2f%%)'\n", outpath,
				i % w, i / w, ((float)i / (w * h)) * 100);
			exit(EXIT_FAILURE);
		}

		tlut[tlut_size - 1] = pixels_conv[i];
		indis[i] = tlut_size - 1;
	}

#if DEBUG
	printf("'%s': tlut_size=%d dimensions=(%dx%d)\n",
	       outpath, tlut_size, w, h);
#endif

	stbi_image_free(pixels_in);
	free(pixels_conv);

	/* write */
	unsigned short type_out = u16flipendian(tex_type);
	unsigned short width_out = u16flipendian(w);
	unsigned short height_out = u16flipendian(h);
	unsigned short tlut_size_out = u16flipendian((unsigned short)tlut_size);
	FILE *file = fopen(outpath, "wb");

	if (file)
	{
		fclose(file);
		remove(outpath);
		file = fopen(outpath, "wb");
	}

	fwrite(&type_out, sizeof(type_out), 1, file);
	fwrite(&width_out, sizeof(width_out), 1, file);
	fwrite(&height_out, sizeof(height_out), 1, file);
	fwrite(&tlut_size_out, sizeof(tlut_size_out), 1, file);
	fwrite(tlut, sizeof(*tlut), tlut_size, file);
	fwrite(indis, sizeof(*indis), w * h, file);

	free(indis);
	free(tlut);
}

int main(int argc, char **argv)
{
	/* make sure input is valid */
	if (argc != 3)
	{
		fprintf(stderr, "Usage: %s <input.png> <output.bin>\n",
			argv[0]);
		exit(EXIT_FAILURE);
	}

	/* stbi_set_flip_vertically_on_load(1); */

	/* check format */
	const char *str = NULL;

	for (int i = 0; i < TEXTYPE_CNT; i++)
	{
		static const char *textype_strs[TEXTYPE_CNT] = {
			"rgba16", "ci8",
		};

		if (!(str = strstr(argv[1], textype_strs[i])))
			continue;

		tex_type = i;
		break;
	}

	if (!str)
	{
		fprintf(stderr,
			"'%s': Couldn't determine texture type\n", argv[1]);
		exit(EXIT_FAILURE);
	}

	/*
	 * TODO: Replace this with a function pointer lookup table
	 */
	switch (tex_type)
	{
	case TEXTYPE_RGBA16:
		rgba16_convert_and_write(argv[1], argv[2]);
		return (0);

	
	case TEXTYPE_CI8:
		ci8_convert_and_write(argv[1], argv[2]);
		return (0);

	default:
		fprintf(stderr, "'%s' is not any "
			"recognized texture format\n", argv[1]);
		exit(EXIT_FAILURE);
	}

	return (1);
}
