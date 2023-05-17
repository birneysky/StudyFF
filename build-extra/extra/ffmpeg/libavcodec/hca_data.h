/*
 * This file is part of FFmpeg.
 *
 * FFmpeg is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * FFmpeg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with FFmpeg; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */


#ifndef AVCODEC_HCA_DATA_H
#define AVCODEC_HCA_DATA_H

#include <stdint.h>

static const uint8_t max_bits_table[] = {
    0, 2, 3, 3, 4, 4, 4, 4, 5, 6, 7, 8, 9, 10, 11, 12,
};

static const uint8_t quant_spectrum_bits[] =
{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    1,1,2,2,0,0,0,0,0,0,0,0,0,0,0,0,
    2,2,2,2,2,2,3,3,0,0,0,0,0,0,0,0,
    2,2,3,3,3,3,3,3,0,0,0,0,0,0,0,0,
    3,3,3,3,3,3,3,3,3,3,3,3,3,3,4,4,
    3,3,3,3,3,3,3,3,3,3,4,4,4,4,4,4,
    3,3,3,3,3,3,4,4,4,4,4,4,4,4,4,4,
    3,3,4,4,4,4,4,4,4,4,4,4,4,4,4,4,
};

static const int8_t quant_spectrum_value[] =
{
    +0,+0,+0,+0,+0,+0,+0,+0,+0,+0,+0,+0,+0,+0,+0,+0,
    +0,+0,+1,-1,+0,+0,+0,+0,+0,+0,+0,+0,+0,+0,+0,+0,
    +0,+0,+1,+1,-1,-1,+2,-2,+0,+0,+0,+0,+0,+0,+0,+0,
    +0,+0,+1,-1,+2,-2,+3,-3,+0,+0,+0,+0,+0,+0,+0,+0,
    +0,+0,+1,+1,-1,-1,+2,+2,-2,-2,+3,+3,-3,-3,+4,-4,
    +0,+0,+1,+1,-1,-1,+2,+2,-2,-2,+3,-3,+4,-4,+5,-5,
    +0,+0,+1,+1,-1,-1,+2,-2,+3,-3,+4,-4,+5,-5,+6,-6,
    +0,+0,+1,-1,+2,-2,+3,-3,+4,-4,+5,-5,+6,-6,+7,-7,
};

static const uint8_t scale_table[] =
{
    15, 14, 14, 14, 14, 14, 14, 13, 13,
    13, 13, 13, 13, 12, 12, 12, 12,
    12, 12, 11, 11, 11, 11, 11, 11,
    10, 10, 10, 10, 10, 10, 10,  9,
    9,  9,  9,  9,  9,  8,  8,  8,
    8,  8,  8,  7,  6,  6,  5,  4,
    4,  4,  3,  3,  3,  2,  2,  2,
    2,  1,
};

static const float window[128] =
{
    0.000690534, 0.00197623, 0.00367386, 0.00572424, 0.0080967, 0.0107732, 0.0137425, 0.0169979,
    0.0205353, 0.0243529, 0.0284505, 0.0328291, 0.0374906, 0.0424379, 0.0476744, 0.0532043,
    0.0590321, 0.0651629, 0.071602, 0.0783552, 0.0854285, 0.092828, 0.10056, 0.108631,
    0.117048, 0.125817, 0.134944, 0.144437, 0.1543, 0.164539, 0.175161, 0.186169,
    0.197569, 0.209363, 0.221555, 0.234145, 0.247136, 0.260526, 0.274313, 0.288493,
    0.303062, 0.318012, 0.333333, 0.349015, 0.365044, 0.381403, 0.398073, 0.415034,
    0.43226, 0.449725, 0.4674, 0.485251, 0.503245, 0.521344, 0.539509, 0.557698,
    0.575869, 0.593978, 0.611981, 0.629831, 0.647486, 0.6649, 0.682031, 0.698838,
    0.71528, 0.731323, 0.746932, 0.762077, 0.776732, 0.790873, 0.804481, 0.817542,
    0.830044, 0.84198, 0.853347, 0.864144, 0.874375, 0.884046, 0.893167, 0.901749,
    0.909806, 0.917354, 0.924409, 0.93099, 0.937117, 0.942809, 0.948087, 0.952971,
    0.957482, 0.961641, 0.965467, 0.968981, 0.972202, 0.975148, 0.977838, 0.980289,
    0.982518, 0.98454, 0.986371, 0.988024, 0.989514, 0.990853, 0.992053, 0.993126,
    0.994082, 0.994931, 0.995682, 0.996344, 0.996926, 0.997433, 0.997875, 0.998256,
    0.998584, 0.998863, 0.999099, 0.999297, 0.999461, 0.999595, 0.999703, 0.999789,
    0.999856, 0.999906, 0.999942, 0.999967, 0.999984, 0.999993, 0.999998, 1.0,
};

static const float intensity_ratio_table[] =
{
    2.0, 1.85714, 1.71429, 1.57143, 1.42857, 1.28571, 1.14286, 1.0,
    0.857143, 0.714286, 0.571429, 0.428571, 0.285714, 0.142857, 0.0, 0.0,
};

static const float scale_conversion_table[] =
{
    0, 0, 1.87066e-08, 2.49253e-08, 3.32113e-08, 4.42518e-08, 5.89626e-08, 7.85637e-08, 1.04681e-07,
    1.3948e-07, 1.85848e-07, 2.4763e-07, 3.2995e-07, 4.39636e-07, 5.85785e-07, 7.80519e-07, 1.03999e-06,
    1.38572e-06, 1.84637e-06, 2.46017e-06, 3.27801e-06, 4.36772e-06, 5.8197e-06, 7.75435e-06, 1.03321e-05,
    1.37669e-05, 1.83435e-05, 2.44414e-05, 3.25665e-05, 4.33927e-05, 5.78179e-05, 7.70384e-05, 0.000102648,
    0.000136772, 0.00018224, 0.000242822, 0.000323544, 0.000431101, 0.000574413, 0.000765366, 0.0010198,
    0.00135881, 0.00181053, 0.0024124, 0.00321437, 0.00428293, 0.00570671, 0.00760381, 0.0101316,
    0.0134996, 0.0179873, 0.0239669, 0.0319343, 0.0425503, 0.0566954, 0.0755428, 0.100656,
    0.134117, 0.178702, 0.238108, 0.317263, 0.422731, 0.563261, 0.750507,
    1.0, 1.33243, 1.77538, 2.36557, 3.15196, 4.19978, 5.59592, 7.45618,
    9.93486, 13.2375, 17.6381, 23.5016, 31.3143, 41.7242, 55.5947, 74.0762,
    98.7015, 131.513, 175.232, 233.485, 311.103, 414.524, 552.326, 735.937,
    980.586, 1306.56, 1740.91, 2319.64, 3090.77, 4118.24, 5487.28, 7311.43,
    9741.98, 12980.5, 17295.7, 23045.3, 30706.4, 40914.2, 54515.4, 72638,
    96785.3, 128960, 171830, 228952, 305064, 406477, 541603, 721649,
    961548, 1.2812e+06, 1.70711e+06, 2.27461e+06, 3.03076e+06, 4.03829e+06, 5.38075e+06, 7.16948e+06,
    9.55285e+06, 1.27285e+07, 1.69599e+07, 2.25979e+07, 3.01102e+07, 4.01198e+07, 5.3457e+07, 0,
};

static const int scale_conv_bias = 64;

static const float dequantizer_scaling_table[] =
{
    1.58838e-07, 2.11641e-07, 2.81998e-07, 3.75743e-07, 5.00652e-07, 6.67085e-07, 8.88846e-07, 1.18433e-06,
    1.57804e-06, 2.10263e-06, 2.80161e-06, 3.73296e-06, 4.97391e-06, 6.6274e-06, 8.83057e-06, 1.17661e-05,
    1.56776e-05, 2.08893e-05, 2.78336e-05, 3.70864e-05, 4.94151e-05, 6.58423e-05, 8.77305e-05, 0.000116895,
    0.000155755, 0.000207533, 0.000276523, 0.000368448, 0.000490933, 0.000654135, 0.00087159, 0.00116134,
    0.0015474, 0.00206181, 0.00274722, 0.00366048, 0.00487735, 0.00649874, 0.00865913, 0.0115377,
    0.0153732, 0.0204838, 0.0272932, 0.0363664, 0.0484558, 0.0645641, 0.0860272, 0.114626,
    0.152731, 0.203503, 0.271155, 0.361295, 0.481401, 0.641435, 0.854669, 1.13879,
    1.51736, 2.02178, 2.69388, 3.58942, 4.78266, 6.37257, 8.49102, 11.3137,
};

static const float quant_step_size[] =
{
    0.0, 0.666667, 0.4, 0.285714, 0.222222, 0.181818, 0.153846, 0.133333, 0.0645161,
    0.031746, 0.015748, 0.00784314, 0.00391389, 0.00195503, 0.00097704, 0.000488401,
};

static const uint8_t ath_base_curve[656] =
{
    0x78,0x5F,0x56,0x51,0x4E,0x4C,0x4B,0x49,0x48,0x48,0x47,0x46,0x46,0x45,0x45,0x45,
    0x44,0x44,0x44,0x44,0x43,0x43,0x43,0x43,0x43,0x43,0x42,0x42,0x42,0x42,0x42,0x42,
    0x42,0x42,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x40,0x40,0x40,0x40,
    0x40,0x40,0x40,0x40,0x40,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,
    0x3F,0x3F,0x3F,0x3E,0x3E,0x3E,0x3E,0x3E,0x3E,0x3D,0x3D,0x3D,0x3D,0x3D,0x3D,0x3D,
    0x3C,0x3C,0x3C,0x3C,0x3C,0x3C,0x3C,0x3C,0x3B,0x3B,0x3B,0x3B,0x3B,0x3B,0x3B,0x3B,
    0x3B,0x3B,0x3B,0x3B,0x3B,0x3B,0x3B,0x3B,0x3B,0x3B,0x3B,0x3B,0x3B,0x3B,0x3B,0x3B,
    0x3B,0x3B,0x3B,0x3B,0x3B,0x3B,0x3B,0x3B,0x3C,0x3C,0x3C,0x3C,0x3C,0x3C,0x3C,0x3C,
    0x3D,0x3D,0x3D,0x3D,0x3D,0x3D,0x3D,0x3D,0x3E,0x3E,0x3E,0x3E,0x3E,0x3E,0x3E,0x3F,
    0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,
    0x3F,0x3F,0x3F,0x3F,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,
    0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x41,0x41,0x41,0x41,0x41,0x41,0x41,
    0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,
    0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x42,
    0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x43,0x43,0x43,
    0x43,0x43,0x43,0x43,0x43,0x43,0x43,0x43,0x43,0x43,0x43,0x43,0x43,0x43,0x44,0x44,
    0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x45,0x45,0x45,0x45,
    0x45,0x45,0x45,0x45,0x45,0x45,0x45,0x45,0x46,0x46,0x46,0x46,0x46,0x46,0x46,0x46,
    0x46,0x46,0x47,0x47,0x47,0x47,0x47,0x47,0x47,0x47,0x47,0x47,0x48,0x48,0x48,0x48,
    0x48,0x48,0x48,0x48,0x49,0x49,0x49,0x49,0x49,0x49,0x49,0x49,0x4A,0x4A,0x4A,0x4A,
    0x4A,0x4A,0x4A,0x4A,0x4B,0x4B,0x4B,0x4B,0x4B,0x4B,0x4B,0x4C,0x4C,0x4C,0x4C,0x4C,
    0x4C,0x4D,0x4D,0x4D,0x4D,0x4D,0x4D,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4F,0x4F,0x4F,
    0x4F,0x4F,0x4F,0x50,0x50,0x50,0x50,0x50,0x51,0x51,0x51,0x51,0x51,0x52,0x52,0x52,
    0x52,0x52,0x53,0x53,0x53,0x53,0x54,0x54,0x54,0x54,0x54,0x55,0x55,0x55,0x55,0x56,
    0x56,0x56,0x56,0x57,0x57,0x57,0x57,0x57,0x58,0x58,0x58,0x59,0x59,0x59,0x59,0x5A,
    0x5A,0x5A,0x5A,0x5B,0x5B,0x5B,0x5B,0x5C,0x5C,0x5C,0x5D,0x5D,0x5D,0x5D,0x5E,0x5E,
    0x5E,0x5F,0x5F,0x5F,0x60,0x60,0x60,0x61,0x61,0x61,0x61,0x62,0x62,0x62,0x63,0x63,
    0x63,0x64,0x64,0x64,0x65,0x65,0x66,0x66,0x66,0x67,0x67,0x67,0x68,0x68,0x68,0x69,
    0x69,0x6A,0x6A,0x6A,0x6B,0x6B,0x6B,0x6C,0x6C,0x6D,0x6D,0x6D,0x6E,0x6E,0x6F,0x6F,
    0x70,0x70,0x70,0x71,0x71,0x72,0x72,0x73,0x73,0x73,0x74,0x74,0x75,0x75,0x76,0x76,
    0x77,0x77,0x78,0x78,0x78,0x79,0x79,0x7A,0x7A,0x7B,0x7B,0x7C,0x7C,0x7D,0x7D,0x7E,
    0x7E,0x7F,0x7F,0x80,0x80,0x81,0x81,0x82,0x83,0x83,0x84,0x84,0x85,0x85,0x86,0x86,
    0x87,0x88,0x88,0x89,0x89,0x8A,0x8A,0x8B,0x8C,0x8C,0x8D,0x8D,0x8E,0x8F,0x8F,0x90,
    0x90,0x91,0x92,0x92,0x93,0x94,0x94,0x95,0x95,0x96,0x97,0x97,0x98,0x99,0x99,0x9A,
    0x9B,0x9B,0x9C,0x9D,0x9D,0x9E,0x9F,0xA0,0xA0,0xA1,0xA2,0xA2,0xA3,0xA4,0xA5,0xA5,
    0xA6,0xA7,0xA7,0xA8,0xA9,0xAA,0xAA,0xAB,0xAC,0xAD,0xAE,0xAE,0xAF,0xB0,0xB1,0xB1,
    0xB2,0xB3,0xB4,0xB5,0xB6,0xB6,0xB7,0xB8,0xB9,0xBA,0xBA,0xBB,0xBC,0xBD,0xBE,0xBF,
    0xC0,0xC1,0xC1,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7,0xC8,0xC9,0xC9,0xCA,0xCB,0xCC,0xCD,
    0xCE,0xCF,0xD0,0xD1,0xD2,0xD3,0xD4,0xD5,0xD6,0xD7,0xD8,0xD9,0xDA,0xDB,0xDC,0xDD,
    0xDE,0xDF,0xE0,0xE1,0xE2,0xE3,0xE4,0xE5,0xE6,0xE7,0xE8,0xE9,0xEA,0xEB,0xED,0xEE,
    0xEF,0xF0,0xF1,0xF2,0xF3,0xF4,0xF5,0xF7,0xF8,0xF9,0xFA,0xFB,0xFC,0xFD,0xFF,0xFF,
};

#endif /* AVCODEC_HCA_DATA_H */
