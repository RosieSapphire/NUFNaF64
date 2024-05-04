#include "engine/random.h"

static const u16 rand_array[0x100] = {
	62332,
	61863,
	15390,
	35219,
	46092,
	9473,
	39814,
	36652,
	5214,
	15086,
	54918,
	58266,
	6707,
	49394,
	44873,
	309,
	3252,
	43870,
	11199,
	32795,
	482,
	8517,
	16078,
	61839,
	30056,
	47198,
	29014,
	39078,
	1569,
	49559,
	14082,
	17716,
	35727,
	37405,
	40222,
	1922,
	22369,
	37673,
	7411,
	14794,
	11143,
	5142,
	4513,
	53221,
	17764,
	41387,
	25398,
	38327,
	11859,
	53560,
	27895,
	64027,
	22236,
	23332,
	28665,
	21669,
	30476,
	551,
	34109,
	65089,
	19349,
	56955,
	5249,
	34995,
	5253,
	59471,
	33994,
	50044,
	63063,
	62053,
	21306,
	16703,
	16706,
	21073,
	27142,
	12977,
	47006,
	37977,
	43867,
	37228,
	27796,
	32046,
	56898,
	30900,
	37667,
	49007,
	59822,
	18260,
	8509,
	47862,
	24052,
	15488,
	4428,
	34082,
	19292,
	42614,
	39484,
	30708,
	9572,
	3800,
	14430,
	45153,
	3002,
	28838,
	32992,
	18881,
	47207,
	4179,
	63590,
	30661,
	35390,
	44854,
	28587,
	1711,
	44670,
	64060,
	3005,
	18216,
	26203,
	5630,
	48498,
	27200,
	38,
	59443,
	24009,
	25950,
	55146,
	43736,
	34895,
	41061,
	44400,
	33047,
	38537,
	59645,
	59852,
	57726,
	27484,
	17611,
	44393,
	63493,
	29996,
	49645,
	56043,
	40991,
	64917,
	26086,
	61198,
	38517,
	32467,
	10576,
	61535,
	23339,
	64941,
	53405,
	5178,
	19383,
	10453,
	28311,
	15307,
	41077,
	36993,
	63722,
	39160,
	38140,
	37968,
	58841,
	29821,
	50413,
	43395,
	2312,
	53078,
	14719,
	57990,
	58710,
	56602,
	5113,
	5331,
	54964,
	60897,
	44541,
	41585,
	29586,
	48710,
	16298,
	16545,
	62943,
	36175,
	23369,
	50200,
	48162,
	51951,
	63486,
	49970,
	23618,
	42626,
	59997,
	375,
	50899,
	21881,
	52387,
	18888,
	4195,
	54281,
	28715,
	32998,
	40206,
	64203,
	55703,
	2691,
	55043,
	62721,
	31810,
	22120,
	19169,
	23665,
	26240,
	63017,
	14916,
	24483,
	40548,
	20496,
	50962,
	6866,
	35099,
	54473,
	14086,
	41771,
	6732,
	63291,
	46063,
	10827,
	34379,
	51836,
	59081,
	22928,
	13351,
	31379,
	52054,
	7275,
	57000,
	1688,
	6104,
	19576,
	9846,
	59440,
	872,
	25678,
	27513,
	13884,
	5047,
	26806,
	32343,
	53419,
	1869,
	18358,
	6325,
};

static u8 rand_head;

u16 random(void)
{
	return rand_array[rand_head++];
}
