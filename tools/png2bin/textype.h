#ifndef _PNG2BIN_TEXTYPE_H_
#define _PNG2BIN_TEXTYPE_H_

enum
{
	TEXTYPE_RGBA16 = 0,
	TEXTYPE_CI8,
	TEXTYPE_CNT,
};

const char *textype_strs[TEXTYPE_CNT] = {
	"rgba16",
	"ci8",
};

#endif /* _PNG2BIN_TEXTYPE_H_ */
