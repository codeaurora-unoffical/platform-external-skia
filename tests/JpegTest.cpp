/*
 * Copyright 2013 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "Test.h"
#include "TestClassDef.h"
#include "SkBitmap.h"
#include "SkData.h"
#include "SkForceLinking.h"
#include "SkImageDecoder.h"
#include "SkImage.h"
#include "SkStream.h"

__SK_FORCE_IMAGE_DECODER_LINKING;

#define JPEG_TEST_WRITE_TO_FILE_FOR_DEBUGGING 0  // do not do this for
                                                 // normal unit testing.
static unsigned char goodJpegImage[] = {
0xFF, 0xD8, 0xFF, 0xE0, 0x00, 0x10, 0x4A, 0x46,
0x49, 0x46, 0x00, 0x01, 0x01, 0x01, 0x00, 0x8F,
0x00, 0x8F, 0x00, 0x00, 0xFF, 0xDB, 0x00, 0x43,
0x00, 0x05, 0x03, 0x04, 0x04, 0x04, 0x03, 0x05,
0x04, 0x04, 0x04, 0x05, 0x05, 0x05, 0x06, 0x07,
0x0C, 0x08, 0x07, 0x07, 0x07, 0x07, 0x0F, 0x0B,
0x0B, 0x09, 0x0C, 0x11, 0x0F, 0x12, 0x12, 0x11,
0x0F, 0x11, 0x11, 0x13, 0x16, 0x1C, 0x17, 0x13,
0x14, 0x1A, 0x15, 0x11, 0x11, 0x18, 0x21, 0x18,
0x1A, 0x1D, 0x1D, 0x1F, 0x1F, 0x1F, 0x13, 0x17,
0x22, 0x24, 0x22, 0x1E, 0x24, 0x1C, 0x1E, 0x1F,
0x1E, 0xFF, 0xDB, 0x00, 0x43, 0x01, 0x05, 0x05,
0x05, 0x07, 0x06, 0x07, 0x0E, 0x08, 0x08, 0x0E,
0x1E, 0x14, 0x11, 0x14, 0x1E, 0x1E, 0x1E, 0x1E,
0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E,
0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E,
0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E,
0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E,
0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E,
0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0xFF, 0xC0,
0x00, 0x11, 0x08, 0x00, 0x80, 0x00, 0x80, 0x03,
0x01, 0x22, 0x00, 0x02, 0x11, 0x01, 0x03, 0x11,
0x01, 0xFF, 0xC4, 0x00, 0x18, 0x00, 0x01, 0x01,
0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00,
0x08, 0x06, 0x05, 0xFF, 0xC4, 0x00, 0x4C, 0x10,
0x00, 0x00, 0x01, 0x07, 0x08, 0x05, 0x08, 0x05,
0x0A, 0x03, 0x09, 0x01, 0x00, 0x00, 0x00, 0x00,
0x00, 0x01, 0x02, 0x03, 0x04, 0x06, 0x07, 0x11,
0x05, 0x08, 0x12, 0x13, 0x14, 0x15, 0x38, 0xB4,
0x16, 0x17, 0x21, 0x31, 0x84, 0x18, 0x22, 0x23,
0x24, 0x58, 0xA5, 0xA6, 0xD2, 0x32, 0x51, 0x56,
0x61, 0xD3, 0x28, 0x33, 0x41, 0x48, 0x67, 0x85,
0x86, 0xC3, 0xE4, 0xF0, 0x25, 0x49, 0x55, 0x09,
0x34, 0x35, 0x36, 0x53, 0x68, 0x72, 0x81, 0xA7,
0xE2, 0xFF, 0xC4, 0x00, 0x14, 0x01, 0x01, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF,
0xC4, 0x00, 0x14, 0x11, 0x01, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xDA, 0x00,
0x0C, 0x03, 0x01, 0x00, 0x02, 0x11, 0x03, 0x11,
0x00, 0x3F, 0x00, 0xD9, 0x62, 0x10, 0x80, 0x40,
0x65, 0xED, 0x62, 0x75, 0xC8, 0x7D, 0xFF, 0x00,
0x92, 0x30, 0x33, 0x01, 0x97, 0xB5, 0x89, 0xD7,
0x21, 0xF7, 0xFE, 0x48, 0xC0, 0x0C, 0xC2, 0x10,
0x80, 0x40, 0x66, 0x64, 0xB8, 0x62, 0x64, 0x78,
0xDC, 0xEA, 0x70, 0xCC, 0x06, 0x66, 0x4B, 0x86,
0x26, 0x47, 0x8D, 0xCE, 0xA7, 0x00, 0xCC, 0x21,
0x08, 0x04, 0x31, 0x9F, 0xF2, 0xC5, 0xFD, 0xFF,
0x00, 0x5A, 0x1B, 0x30, 0x63, 0x3F, 0xE5, 0x8B,
0xFB, 0xFE, 0xB4, 0x03, 0x66, 0x01, 0x99, 0x92,
0xE1, 0x89, 0x91, 0xE3, 0x73, 0xA9, 0xC3, 0x30,
0x19, 0x99, 0x2E, 0x18, 0x99, 0x1E, 0x37, 0x3A,
0x9C, 0x03, 0x30, 0x84, 0x33, 0x33, 0x92, 0x55,
0x7E, 0xCF, 0x29, 0xD8, 0x49, 0x0D, 0xAE, 0xBD,
0xAE, 0xAB, 0xC6, 0xBB, 0xAA, 0x68, 0x92, 0x92,
0x6A, 0xBA, 0xB4, 0xE9, 0x11, 0x7A, 0x7C, 0xD8,
0xC6, 0x84, 0x77, 0x12, 0x11, 0x87, 0xBC, 0x07,
0x67, 0xAC, 0x47, 0xED, 0xD9, 0xD3, 0xC6, 0xAA,
0x5E, 0x51, 0x6B, 0x11, 0xFB, 0x76, 0x74, 0xF1,
0xAA, 0x97, 0x94, 0x33, 0x08, 0x00, 0xCE, 0xB1,
0x1F, 0xB7, 0x67, 0x4F, 0x1A, 0xA9, 0x79, 0x41,
0x9B, 0xC4, 0x6C, 0xDE, 0xC2, 0xCB, 0xF6, 0x75,
0x92, 0x84, 0xA0, 0xE5, 0xEC, 0x12, 0xB2, 0x9D,
0xEF, 0x76, 0xC9, 0xBA, 0x50, 0xAA, 0x92, 0xF1,
0xA6, 0xAA, 0x69, 0x12, 0xF4, 0xA4, 0x36, 0x8A,
0x2A, 0xB3, 0x60, 0x77, 0x3A, 0x34, 0xA3, 0x02,
0x6D, 0x1A, 0xC8, 0x0C, 0xBD, 0xAC, 0x4E, 0xB9,
0x0F, 0xBF, 0xF2, 0x46, 0x00, 0xB5, 0x88, 0xFD,
0xBB, 0x3A, 0x78, 0xD5, 0x4B, 0xCA, 0x2D, 0x62,
0x3F, 0x6E, 0xCE, 0x9E, 0x35, 0x52, 0xF2, 0x86,
0x61, 0x00, 0x19, 0xD6, 0x23, 0xF6, 0xEC, 0xE9,
0xE3, 0x55, 0x2F, 0x28, 0x33, 0x9A, 0xE3, 0x66,
0xF6, 0x24, 0x97, 0x12, 0xCE, 0xC9, 0xEC, 0xCB,
0x97, 0xD2, 0x49, 0x25, 0x15, 0xAA, 0xCF, 0x29,
0x69, 0x42, 0xAA, 0xA5, 0x7C, 0x56, 0x92, 0x94,
0xEE, 0x88, 0xF3, 0x4A, 0x71, 0xB4, 0x4E, 0x29,
0xC6, 0xED, 0xDF, 0x46, 0x3B, 0x8A, 0x35, 0x90,
0x19, 0x99, 0x2E, 0x18, 0x99, 0x1E, 0x37, 0x3A,
0x9C, 0x01, 0x9B, 0xE4, 0x79, 0x73, 0x93, 0x59,
0x69, 0xD9, 0x36, 0x65, 0x99, 0x62, 0x34, 0x1E,
0x56, 0x95, 0xAD, 0x96, 0x75, 0x7B, 0xD6, 0x4F,
0x94, 0x6F, 0x1A, 0xA3, 0x0C, 0x3C, 0xEE, 0x71,
0xE6, 0x51, 0x45, 0x56, 0x6D, 0x22, 0xED, 0x29,
0x29, 0x53, 0xFA, 0x4A, 0x41, 0xE2, 0xFC, 0xBB,
0x3F, 0x77, 0x28, 0x66, 0x7B, 0x58, 0x9D, 0x72,
0x1F, 0x7F, 0xE4, 0x8C, 0x0C, 0xC0, 0x31, 0x9F,
0xCB, 0xB3, 0xF7, 0x72, 0x8F, 0x19, 0xB6, 0x76,
0x8F, 0x61, 0x8B, 0x99, 0xDA, 0xDA, 0x16, 0x99,
0xB7, 0xB0, 0x49, 0x2A, 0x74, 0x2D, 0x0C, 0x9D,
0xD4, 0xAA, 0x92, 0x85, 0x39, 0x40, 0xD2, 0x9B,
0xD7, 0x0C, 0x3C, 0xA7, 0x16, 0x27, 0x1C, 0x6A,
0x5D, 0x91, 0xDF, 0x43, 0x70, 0xDC, 0xA2, 0x01,
0x8C, 0xF5, 0xC1, 0xFE, 0xF1, 0x3F, 0xF3, 0x4F,
0xFE, 0x07, 0xB5, 0x35, 0xC6, 0x31, 0xEC, 0x4A,
0xCE, 0x25, 0x9D, 0x94, 0x19, 0x97, 0xD1, 0xA3,
0x72, 0x4A, 0x5B, 0x55, 0x9E, 0x4D, 0xD1, 0x75,
0x55, 0xBA, 0x88, 0x2D, 0x25, 0x21, 0xDD, 0x29,
0xE7, 0x10, 0xE3, 0xA9, 0x1C, 0x43, 0x8E, 0xDB,
0xBA, 0x94, 0x37, 0x10, 0x6B, 0x21, 0x00, 0x19,
0xD5, 0xDB, 0xF6, 0xED, 0x17, 0xE0, 0xA5, 0x2F,
0x30, 0x33, 0x9A, 0xE3, 0x18, 0xF6, 0x25, 0x67,
0x12, 0xCE, 0xCA, 0x0C, 0xCB, 0xE8, 0xD1, 0xB9,
0x25, 0x2D, 0xAA, 0xCF, 0x26, 0xE8, 0xBA, 0xAA,
0xDD, 0x44, 0x16, 0x92, 0x90, 0xEE, 0x94, 0xF3,
0x88, 0x71, 0xD4, 0x8E, 0x21, 0xC7, 0x6D, 0xDD,
0x4A, 0x1B, 0x88, 0x35, 0x90, 0x19, 0x99, 0x2E,
0x18, 0x99, 0x1E, 0x37, 0x3A, 0x9C, 0x03, 0x30,
0x80, 0x04, 0xDB, 0x99, 0x69, 0x09, 0x8B, 0x7E,
0xCF, 0x8D, 0x99, 0x66, 0x54, 0x6C, 0x12, 0x4A,
0x9D, 0xC7, 0x67, 0x57, 0xAD, 0x3D, 0x25, 0x0A,
0x6A, 0xA9, 0x4F, 0x3B, 0x9C, 0x79, 0x4A, 0x71,
0x62, 0x71, 0xC7, 0x17, 0x69, 0x4B, 0xBF, 0xD4,
0x1F, 0xC0, 0x43, 0x8C, 0x79, 0xAE, 0xB5, 0x84,
0x79, 0x57, 0x7E, 0x9A, 0xC8, 0x57, 0xAD, 0xDD,
0x5B, 0x64, 0xEB, 0x69, 0xD0, 0xD5, 0xD6, 0x50,
0xA7, 0xF3, 0x47, 0x9B, 0x18, 0xD0, 0x33, 0x7C,
0x61, 0x0D, 0x9F, 0x48, 0xEC, 0xC0, 0x03, 0x12,
0xFB, 0x5E, 0xC3, 0x68, 0xCC, 0x2A, 0x34, 0xCC,
0xCB, 0x83, 0xB7, 0xC9, 0x2B, 0x94, 0xEC, 0xEB,
0x1A, 0x5E, 0xAA, 0x8E, 0x9D, 0x03, 0xCE, 0x30,
0xEE, 0x69, 0xE8, 0xC8, 0x71, 0x20, 0x71, 0xA7,
0x13, 0x69, 0x09, 0xBB, 0xD4, 0x03, 0xD9, 0xE4,
0xB8, 0xE2, 0x7D, 0x86, 0xEF, 0x65, 0xDF, 0x8C,
0x2E, 0x4B, 0x8E, 0x27, 0xD8, 0x6E, 0xF6, 0x5D,
0xF8, 0xC2, 0xD6, 0x23, 0xF6, 0xEC, 0xE9, 0xE3,
0x55, 0x2F, 0x28, 0xB5, 0x88, 0xFD, 0xBB, 0x3A,
0x78, 0xD5, 0x4B, 0xCA, 0x02, 0xE4, 0xB8, 0xE2,
0x7D, 0x86, 0xEF, 0x65, 0xDF, 0x8C, 0x0C, 0xE6,
0xB8, 0xE1, 0x1D, 0x3B, 0x68, 0xE2, 0x59, 0xD6,
0x99, 0xA6, 0x65, 0x2D, 0xF2, 0xB2, 0xE5, 0xAA,
0xD0, 0xB1, 0x78, 0x2D, 0x23, 0xA7, 0x41, 0x69,
0x29, 0x86, 0xF3, 0x4C, 0x48, 0x43, 0x49, 0x03,
0x4D, 0x34, 0x9B, 0x08, 0x4D, 0xDE, 0xB0, 0x99,
0xAC, 0x47, 0xED, 0xD9, 0xD3, 0xC6, 0xAA, 0x5E,
0x50, 0x67, 0x35, 0xC6, 0xCD, 0xEC, 0x49, 0x2E,
0x25, 0x9D, 0x93, 0xD9, 0x97, 0x2F, 0xA4, 0x92,
0x4A, 0x2B, 0x55, 0x9E, 0x52, 0xD2, 0x85, 0x55,
0x4A, 0xF8, 0xAD, 0x25, 0x29, 0xDD, 0x11, 0xE6,
0x94, 0xE3, 0x68, 0x9C, 0x53, 0x8D, 0xDB, 0xBE,
0x8C, 0x77, 0x14, 0x04, 0xF1, 0x1C, 0x23, 0xA7,
0x92, 0x5F, 0xB3, 0xAC, 0x66, 0x64, 0xF6, 0x52,
0xA6, 0x49, 0x97, 0xAF, 0x7B, 0xC9, 0x5E, 0xF0,
0x5A, 0x3A, 0xBE, 0xA1, 0x54, 0xD3, 0xD1, 0x73,
0x8A, 0x90, 0xA7, 0x1B, 0x44, 0xE2, 0x94, 0xBC,
0xD2, 0x92, 0x3F, 0x4C, 0x48, 0x13, 0x39, 0x2E,
0x38, 0x9F, 0x61, 0xBB, 0xD9, 0x77, 0xE3, 0x01,
0x97, 0xF4, 0xF7, 0x1B, 0xB6, 0x51, 0xE7, 0xBB,
0x76, 0xD5, 0xB5, 0x74, 0xB7, 0x15, 0xCD, 0x7A,
0x59, 0x15, 0x34, 0x89, 0x02, 0xCD, 0xBA, 0xB9,
0x02, 0x34, 0x47, 0xF3, 0xD1, 0x18, 0x5A, 0xBA,
0x14, 0x8C, 0x2E, 0xD2, 0x16, 0x95, 0x28, 0x12,
0x10, 0x29, 0x46, 0xCC, 0x00, 0x33, 0xC9, 0x71,
0xC4, 0xFB, 0x0D, 0xDE, 0xCB, 0xBF, 0x18, 0x5C,
0x97, 0x1C, 0x4F, 0xB0, 0xDD, 0xEC, 0xBB, 0xF1,
0x83, 0x30, 0x80, 0x0C, 0xF2, 0x5C, 0x71, 0x3E,
0xC3, 0x77, 0xB2, 0xEF, 0xC6, 0x17, 0x25, 0xC7,
0x13, 0xEC, 0x37, 0x7B, 0x2E, 0xFC, 0x60, 0xCC,
0x20, 0x03, 0x3C, 0x97, 0x1C, 0x4F, 0xB0, 0xDD,
0xEC, 0xBB, 0xF1, 0x81, 0x9C, 0xD7, 0x1C, 0x23,
0xA7, 0x6D, 0x1C, 0x4B, 0x3A, 0xD3, 0x34, 0xCC,
0xA5, 0xBE, 0x56, 0x5C, 0xB5, 0x5A, 0x16, 0x2F,
0x05, 0xA4, 0x74, 0xE8, 0x2D, 0x25, 0x30, 0xDE,
0x69, 0x89, 0x08, 0x69, 0x20, 0x69, 0xA6, 0x93,
0x61, 0x09, 0xBB, 0xD6, 0x35, 0x90, 0x19, 0x99,
0x2E, 0x18, 0x99, 0x1E, 0x37, 0x3A, 0x9C, 0x07,
0x8D, 0x36, 0xE6, 0xA6, 0x42, 0x6D, 0x1F, 0xB3,
0xE3, 0x69, 0x99, 0x95, 0xEB, 0x7C, 0x92, 0xB9,
0x71, 0xD9, 0xD6, 0x2A, 0x8F, 0x47, 0x4E, 0x82,
0xAA, 0x53, 0x0E, 0xE6, 0x9E, 0x42, 0x1C, 0x48,
0x1C, 0x69, 0xC4, 0xDA, 0x42, 0x6E, 0xF5, 0x07,
0xF1, 0x08, 0x00, 0xCB, 0x40, 0xF7, 0x1B, 0xBD,
0x67, 0xB4, 0xEC, 0x53, 0x14, 0xE9, 0x74, 0xAB,
0x47, 0x2C, 0x96, 0xB5, 0xBD, 0x22, 0x40, 0xA5,
0xFD, 0xE1, 0x01, 0x12, 0x99, 0xCC, 0x4A, 0x67,
0xFC, 0xC9, 0xB0, 0xA5, 0xF4, 0x62, 0x58, 0x44,
0x84, 0x06, 0x73, 0x5C, 0x6C, 0xDE, 0xC4, 0x92,
0xE2, 0x59, 0xD9, 0x3D, 0x99, 0x72, 0xFA, 0x49,
0x24, 0xA2, 0xB5, 0x59, 0xE5, 0x2D, 0x28, 0x55,
0x54, 0xAF, 0x8A, 0xD2, 0x52, 0x9D, 0xD1, 0x1E,
0x69, 0x4E, 0x36, 0x89, 0xC5, 0x38, 0xDD, 0xBB,
0xE8, 0xC7, 0x71, 0x42, 0x63, 0xA5, 0xC4, 0xEB,
0xEF, 0xFB, 0x83, 0x24, 0x78, 0xA6, 0x4B, 0x86,
0x26, 0x47, 0x8D, 0xCE, 0xA7, 0x01, 0x6B, 0x11,
0xFB, 0x76, 0x74, 0xF1, 0xAA, 0x97, 0x94, 0x5A,
0xC4, 0x7E, 0xDD, 0x9D, 0x3C, 0x6A, 0xA5, 0xE5,
0x0C, 0xC2, 0x00, 0x33, 0xAC, 0x47, 0xED, 0xD9,
0xD3, 0xC6, 0xAA, 0x5E, 0x50, 0x67, 0x35, 0xC6,
0xCD, 0xEC, 0x49, 0x2E, 0x25, 0x9D, 0x93, 0xD9,
0x97, 0x2F, 0xA4, 0x92, 0x4A, 0x2B, 0x55, 0x9E,
0x52, 0xD2, 0x85, 0x55, 0x4A, 0xF8, 0xAD, 0x25,
0x29, 0xDD, 0x11, 0xE6, 0x94, 0xE3, 0x68, 0x9C,
0x53, 0x8D, 0xDB, 0xBE, 0x8C, 0x77, 0x14, 0x6B,
0x20, 0x33, 0x32, 0x5C, 0x31, 0x32, 0x3C, 0x6E,
0x75, 0x38, 0x0C, 0xCD, 0x3E, 0x76, 0x89, 0xBB,
0x97, 0xF4, 0x3B, 0x4D, 0x5D, 0xD6, 0x86, 0xD4,
0x5B, 0xAC, 0x9F, 0xC6, 0x90, 0x2F, 0xDA, 0xA9,
0x59, 0xE9, 0xFC, 0xD1, 0x09, 0x42, 0x8C, 0x0C,
0xDF, 0xBE, 0x9E, 0xCD, 0xC5, 0x1A, 0x67, 0x58,
0x8F, 0xDB, 0xB3, 0xA7, 0x8D, 0x54, 0xBC, 0xA3,
0x8C, 0xFE, 0xD0, 0x76, 0x16, 0xFF, 0x00, 0x76,
0x0A, 0xAD, 0xAD, 0xE9, 0x66, 0xD1, 0x5A, 0x7D,
0x52, 0xCF, 0x4E, 0xD5, 0x6A, 0x4E, 0xAC, 0x8B,
0xD3, 0xA4, 0x4A, 0x14, 0x61, 0x1D, 0xC7, 0x47,
0x76, 0xCD, 0xE2, 0x7D, 0xAA, 0xAF, 0xD9, 0xDA,
0xBB, 0x09, 0x5D, 0xB5, 0xD7, 0xB5, 0xEB, 0x77,
0x54, 0xF5, 0x4D, 0x12, 0x52, 0x43, 0x59, 0x58,
0x9D, 0x1A, 0x2F, 0x4F, 0x9D, 0x08, 0x53, 0x8E,
0xE2, 0xC6, 0x10, 0xF7, 0x80, 0xEC, 0xF5, 0x88,
0xFD, 0xBB, 0x3A, 0x78, 0xD5, 0x4B, 0xCA, 0x2D,
0x62, 0x3F, 0x6E, 0xCE, 0x9E, 0x35, 0x52, 0xF2,
0x8C, 0x67, 0xCA, 0x8D, 0xFB, 0x7B, 0x73, 0xDD,
0x2A, 0x5F, 0x04, 0x5C, 0xA8, 0xDF, 0xB7, 0xB7,
0x3D, 0xD2, 0xA5, 0xF0, 0x40, 0x6C, 0xCD, 0x62,
0x3F, 0x6E, 0xCE, 0x9E, 0x35, 0x52, 0xF2, 0x8B,
0x58, 0x8F, 0xDB, 0xB3, 0xA7, 0x8D, 0x54, 0xBC,
0xA3, 0x33, 0x3B, 0x27, 0xA5, 0x3B, 0x17, 0x95,
0x78, 0x68, 0x54, 0xBB, 0x7A, 0xDD, 0xD5, 0x56,
0xBE, 0xA9, 0x25, 0xA1, 0xAB, 0xAC, 0xA7, 0x43,
0xE7, 0x4C, 0x36, 0x31, 0xA0, 0x7E, 0xE8, 0xC2,
0x1B, 0x7E, 0x81, 0xD9, 0xFC, 0xBB, 0x3F, 0x77,
0x28, 0x06, 0x6D, 0x62, 0x3F, 0x6E, 0xCE, 0x9E,
0x35, 0x52, 0xF2, 0x83, 0x39, 0xAE, 0x36, 0x6F,
0x62, 0x49, 0x71, 0x2C, 0xEC, 0x9E, 0xCC, 0xB9,
0x7D, 0x24, 0x92, 0x51, 0x5A, 0xAC, 0xF2, 0x96,
0x94, 0x2A, 0xAA, 0x57, 0xC5, 0x69, 0x29, 0x4E,
0xE8, 0x8F, 0x34, 0xA7, 0x1B, 0x44, 0xE2, 0x9C,
0x6E, 0xDD, 0xF4, 0x63, 0xB8, 0xA3, 0xC5, 0xF9,
0x76, 0x7E, 0xEE, 0x51, 0xC6, 0x39, 0x2E, 0x56,
0x3A, 0xB0, 0x92, 0x35, 0x69, 0xFE, 0x53, 0xE9,
0xAC, 0x1F, 0xE1, 0x7F, 0xEB, 0xA4, 0xAC, 0xF9,
0xFE, 0x93, 0xE7, 0x2B, 0x3D, 0x2F, 0xFA, 0xD9,
0x00, 0x1B, 0xFC, 0x42, 0x10, 0x0C, 0x9A, 0xD4,
0xBE, 0x39, 0x09, 0xCF, 0xBF, 0x67, 0xD5, 0x28,
0x4A, 0x08, 0x6D, 0xF2, 0xB2, 0xE5, 0xC3, 0x76,
0xC9, 0xB4, 0x8F, 0x47, 0x6B, 0xA0, 0xAA, 0x42,
0x25, 0xE9, 0x48, 0x8C, 0xF3, 0x4C, 0xA0, 0x6A,
0x42, 0x1D, 0xCE, 0x84, 0x61, 0x02, 0x6D, 0xDC,
0x64, 0xE4, 0xA7, 0x5B, 0xAB, 0x57, 0x61, 0x24,
0x31, 0x5A, 0x05, 0x7A, 0xDD, 0xD5, 0xDD, 0x6E,
0xF7, 0xA9, 0xAC, 0xAC, 0x4E, 0x91, 0x2F, 0xA1,
0x52, 0x74, 0x21, 0x4E, 0x1B, 0xCB, 0x18, 0x47,
0xDC, 0x34, 0xCC, 0xF6, 0xB0, 0xC4, 0xD7, 0x70,
0x59, 0xD4, 0x02, 0x99, 0x2E, 0x18, 0x99, 0x1E,
0x37, 0x3A, 0x9C, 0x00, 0xCF, 0x2E, 0x7F, 0xB2,
0xEE, 0xFF, 0x00, 0xFD, 0x38, 0xB9, 0x73, 0xFD,
0x97, 0x77, 0xFF, 0x00, 0xE9, 0xC6, 0xCC, 0x10,
0x0C, 0x67, 0xCB, 0x9F, 0xEC, 0xBB, 0xBF, 0xFF,
0x00, 0x4E, 0x38, 0xC7, 0x25, 0x3A, 0xDD, 0x5A,
0xBB, 0x09, 0x21, 0x8A, 0xD0, 0x2B, 0xD6, 0xEE,
0xAE, 0xEB, 0x77, 0xBD, 0x4D, 0x65, 0x62, 0x74,
0x89, 0x7D, 0x0A, 0x93, 0xA1, 0x0A, 0x70, 0xDE,
0x58, 0xC2, 0x3E, 0xE1, 0xBF, 0xC0, 0xCC, 0xC9,
0x70, 0xC4, 0xC8, 0xF1, 0xB9, 0xD4, 0xE0, 0x33,
0x33, 0xED, 0x9D, 0x6E, 0xB2, 0x9D, 0x84, 0xAE,
0xC5, 0x68, 0x15, 0xD5, 0x78, 0xD4, 0xF5, 0xBB,
0xDE, 0xBA, 0xAE, 0xAD, 0x3A, 0x34, 0xBE, 0x85,
0x49, 0xB1, 0x8D, 0x08, 0x6F, 0x24, 0x23, 0x1F,
0x70, 0x9F, 0x6C, 0xEB, 0x75, 0x94, 0xEC, 0x25,
0x76, 0x2B, 0x40, 0xAE, 0xAB, 0xC6, 0xA7, 0xAD,
0xDE, 0xF5, 0xD5, 0x75, 0x69, 0xD1, 0xA5, 0xF4,
0x2A, 0x4D, 0x8C, 0x68, 0x43, 0x79, 0x21, 0x18,
0xFB, 0x86, 0x99, 0x9E, 0xD6, 0x18, 0x9A, 0xEE,
0x0B, 0x3A, 0x80, 0x53, 0xDA, 0xC3, 0x13, 0x5D,
0xC1, 0x67, 0x50, 0x00, 0xCC, 0xCE, 0x4A, 0x75,
0xBA, 0xB5, 0x76, 0x12, 0x43, 0x15, 0xA0, 0x57,
0xAD, 0xDD, 0x5D, 0xD6, 0xEF, 0x7A, 0x9A, 0xCA,
0xC4, 0xE9, 0x12, 0xFA, 0x15, 0x27, 0x42, 0x14,
0xE1, 0xBC, 0xB1, 0x84, 0x7D, 0xC3, 0xB3, 0xE5,
0xCF, 0xF6, 0x5D, 0xDF, 0xFF, 0x00, 0xA7, 0x0C,
0xD3, 0x25, 0xC3, 0x13, 0x23, 0xC6, 0xE7, 0x53,
0x86, 0x60, 0x18, 0x01, 0x92, 0x9D, 0x6D, 0xC0,
0xF3, 0xDB, 0x76, 0xD7, 0x40, 0xAD, 0x3A, 0x55,
0x60, 0xEA, 0x97, 0xBD, 0x0B, 0x2D, 0x95, 0x01,
0x51, 0x7A, 0x75, 0x25, 0xA7, 0x4A, 0x31, 0xDC,
0x6C, 0x37, 0x6D, 0xDE, 0x3B, 0x3E, 0x5C, 0xFF,
0x00, 0x65, 0xDD, 0xFF, 0x00, 0xFA, 0x70, 0xCC,
0xE9, 0x71, 0x3A, 0xFB, 0xFE, 0xE0, 0xC9, 0x1E,
0x19, 0x80, 0x63, 0x3E, 0x5C, 0xFF, 0x00, 0x65,
0xDD, 0xFF, 0x00, 0xFA, 0x71, 0xC6, 0x39, 0x29,
0xD6, 0xEA, 0xD5, 0xD8, 0x49, 0x0C, 0x56, 0x81,
0x5E, 0xB7, 0x75, 0x77, 0x5B, 0xBD, 0xEA, 0x6B,
0x2B, 0x13, 0xA4, 0x4B, 0xE8, 0x54, 0x9D, 0x08,
0x53, 0x86, 0xF2, 0xC6, 0x11, 0xF7, 0x0D, 0xFE,
0x06, 0x66, 0x4B, 0x86, 0x26, 0x47, 0x8D, 0xCE,
0xA7, 0x00, 0xCC, 0x21, 0x08, 0x00, 0xCC, 0xF6,
0xB0, 0xC4, 0xD7, 0x70, 0x59, 0xD4, 0x02, 0x99,
0x2E, 0x18, 0x99, 0x1E, 0x37, 0x3A, 0x9C, 0x53,
0xDA, 0xC3, 0x13, 0x5D, 0xC1, 0x67, 0x50, 0x0A,
0x64, 0xB8, 0x62, 0x64, 0x78, 0xDC, 0xEA, 0x70,
0x0C, 0xC2, 0x10, 0x80, 0x40, 0x66, 0x64, 0xB8,
0x62, 0x64, 0x78, 0xDC, 0xEA, 0x70, 0xCC, 0x06,
0x66, 0x4B, 0x86, 0x26, 0x47, 0x8D, 0xCE, 0xA7,
0x01, 0x4F, 0x6B, 0x0C, 0x4D, 0x77, 0x05, 0x9D,
0x40, 0x29, 0xED, 0x61, 0x89, 0xAE, 0xE0, 0xB3,
0xA8, 0x05, 0x3D, 0xAC, 0x31, 0x35, 0xDC, 0x16,
0x75, 0x00, 0xA7, 0xB5, 0x86, 0x26, 0xBB, 0x82,
0xCE, 0xA0, 0x01, 0x4C, 0x97, 0x0C, 0x4C, 0x8F,
0x1B, 0x9D, 0x4E, 0x19, 0x86, 0x4D, 0x9A, 0xE3,
0xFB, 0x74, 0xEC, 0x5B, 0x89, 0x67, 0x59, 0x96,
0x99, 0xAB, 0xB0, 0x4A, 0xCA, 0x76, 0xAB, 0x42,
0xBD, 0xDE, 0xB4, 0x92, 0x85, 0x35, 0xA4, 0xA7,
0x9B, 0xCE, 0x31, 0x19, 0x4D, 0x2C, 0x4D, 0x38,
0xD2, 0xEC, 0x29, 0x77, 0xFA, 0xC2, 0x67, 0x2A,
0x37, 0x13, 0xED, 0xCF, 0x74, 0xAE, 0xFC, 0x10,
0x03, 0x33, 0x80, 0xFA, 0xCE, 0xFE, 0x13, 0xFC,
0xB0, 0xCD, 0x32, 0x5C, 0x31, 0x32, 0x3C, 0x6E,
0x75, 0x38, 0x00, 0x79, 0x2D, 0x4C, 0x84, 0xDA,
0x33, 0x13, 0x91, 0x69, 0x99, 0x95, 0xEB, 0x7C,
0x92, 0xB9, 0xA2, 0xF6, 0x75, 0x8A, 0xA3, 0xD1,
0xD3, 0xA0, 0x79, 0xA6, 0x1D, 0xCD, 0x3C, 0x84,
0x38, 0x90, 0x38, 0xD3, 0x89, 0xB4, 0x84, 0xDD,
0xEA, 0x0F, 0xF3, 0x25, 0xC3, 0x13, 0x23, 0xC6,
0xE7, 0x53, 0x80, 0x66, 0x03, 0x33, 0x25, 0xC3,
0x13, 0x23, 0xC6, 0xE7, 0x53, 0x86, 0x60, 0x33,
0x32, 0x5C, 0x31, 0x32, 0x3C, 0x6E, 0x75, 0x38,
0x06, 0x61, 0x08, 0x40, 0x06, 0x67, 0xB5, 0x86,
0x26, 0xBB, 0x82, 0xCE, 0xA0, 0x14, 0xC9, 0x70,
0xC4, 0xC8, 0xF1, 0xB9, 0xD4, 0xE2, 0x9E, 0xD6,
0x18, 0x9A, 0xEE, 0x0B, 0x3A, 0x80, 0x53, 0x25,
0xC3, 0x13, 0x23, 0xC6, 0xE7, 0x53, 0x80, 0x66,
0x10, 0x84, 0x02, 0x03, 0x33, 0x25, 0xC3, 0x13,
0x23, 0xC6, 0xE7, 0x53, 0x86, 0x60, 0x33, 0x32,
0x5C, 0x31, 0x32, 0x3C, 0x6E, 0x75, 0x38, 0x0A,
0x7B, 0x58, 0x62, 0x6B, 0xB8, 0x2C, 0xEA, 0x01,
0x4F, 0x6B, 0x0C, 0x4D, 0x77, 0x05, 0x9D, 0x40,
0x29, 0xED, 0x61, 0x89, 0xAE, 0xE0, 0xB3, 0xA8,
0x05, 0x3D, 0xAC, 0x31, 0x35, 0xDC, 0x16, 0x75,
0x00, 0x06, 0x61, 0x08, 0x40, 0x31, 0x9C, 0xEB,
0x65, 0x86, 0xEE, 0x5F, 0xD7, 0x2C, 0x93, 0xA6,
0x36, 0x66, 0x4D, 0x95, 0xB8, 0xFF, 0x00, 0x82,
0xDD, 0x88, 0x0F, 0xB5, 0x5A, 0xAA, 0x4E, 0xF9,
0xF8, 0x11, 0x21, 0x94, 0x52, 0x12, 0x9E, 0xF3,
0xA3, 0xBB, 0x61, 0x07, 0xB5, 0x35, 0xC6, 0x31,
0xEC, 0x4A, 0xCE, 0x25, 0x9D, 0x94, 0x19, 0x97,
0xD1, 0xA3, 0x72, 0x4A, 0x5B, 0x55, 0x9E, 0x4D,
0xD1, 0x75, 0x55, 0xBA, 0x88, 0x2D, 0x25, 0x21,
0xDD, 0x29, 0xE7, 0x10, 0xE3, 0xA9, 0x1C, 0x43,
0x8E, 0xDB, 0xBA, 0x94, 0x37, 0x10, 0x78, 0xB3,
0x80, 0xFA, 0xCE, 0xFE, 0x13, 0xFC, 0xB0, 0xCD,
0x32, 0x5C, 0x31, 0x32, 0x3C, 0x6E, 0x75, 0x38,
0x0B, 0x57, 0x6F, 0xDB, 0xB4, 0x5F, 0x82, 0x94,
0xBC, 0xC0, 0xCE, 0x6B, 0x8C, 0x63, 0xD8, 0x95,
0x9C, 0x4B, 0x3B, 0x28, 0x33, 0x2F, 0xA3, 0x46,
0xE4, 0x94, 0xB6, 0xAB, 0x3C, 0x9B, 0xA2, 0xEA,
0xAB, 0x75, 0x10, 0x5A, 0x4A, 0x43, 0xBA, 0x53,
0xCE, 0x21, 0xC7, 0x52, 0x38, 0x87, 0x1D, 0xB7,
0x75, 0x28, 0x6E, 0x20, 0xD6, 0x40, 0x66, 0x64,
0xB8, 0x62, 0x64, 0x78, 0xDC, 0xEA, 0x70, 0x16,
0xB1, 0x1F, 0xB7, 0x67, 0x4F, 0x1A, 0xA9, 0x79,
0x45, 0xAC, 0x47, 0xED, 0xD9, 0xD3, 0xC6, 0xAA,
0x5E, 0x50, 0xCC, 0x20, 0x19, 0x36, 0x74, 0x6D,
0x9B, 0xD8, 0x95, 0x9C, 0x4B, 0x45, 0x27, 0xB4,
0xCE, 0x5F, 0x46, 0xE4, 0x94, 0xB6, 0x5B, 0x44,
0xA5, 0xA5, 0x0A, 0xAB, 0x75, 0x10, 0x5A, 0x44,
0x53, 0x7A, 0x23, 0x0D, 0x21, 0xC7, 0x52, 0x38,
0x86, 0x9B, 0xB3, 0x75, 0x28, 0xEE, 0x20, 0xA6,
0xB8, 0xD9, 0xBD, 0x89, 0x25, 0xC4, 0xB3, 0xB2,
0x7B, 0x32, 0xE5, 0xF4, 0x92, 0x49, 0x45, 0x6A,
0xB3, 0xCA, 0x5A, 0x50, 0xAA, 0xA9, 0x5F, 0x15,
0xA4, 0xA5, 0x3B, 0xA2, 0x3C, 0xD2, 0x9C, 0x6D,
0x13, 0x8A, 0x71, 0xBB, 0x77, 0xD1, 0x8E, 0xE2,
0x84, 0xC9, 0xED, 0x61, 0x89, 0xAE, 0xE0, 0xB3,
0xA8, 0x05, 0x32, 0x5C, 0x31, 0x32, 0x3C, 0x6E,
0x75, 0x38, 0x0B, 0x58, 0x8F, 0xDB, 0xB3, 0xA7,
0x8D, 0x54, 0xBC, 0xA2, 0xD6, 0x23, 0xF6, 0xEC,
0xE9, 0xE3, 0x55, 0x2F, 0x28, 0x66, 0x10, 0x01,
0x9D, 0x62, 0x3F, 0x6E, 0xCE, 0x9E, 0x35, 0x52,
0xF2, 0x8F, 0x6A, 0x6B, 0x8C, 0xB4, 0xBA, 0xC5,
0xB8, 0x96, 0x75, 0x99, 0x69, 0x94, 0x6C, 0x12,
0xB2, 0x9D, 0xAA, 0xD0, 0xAF, 0x5A, 0x62, 0x4A,
0x14, 0xD6, 0x92, 0x9E, 0x6F, 0x38, 0xC2, 0x94,
0xD2, 0xC4, 0xD3, 0x8D, 0x2E, 0xC2, 0x97, 0x7F,
0xAC, 0x26, 0x08, 0x00, 0xCC, 0xF6, 0xB0, 0xC4,
0xD7, 0x70, 0x59, 0xD4, 0x02, 0x9E, 0xD6, 0x18,
0x9A, 0xEE, 0x0B, 0x3A, 0x80, 0x53, 0xDA, 0xC3,
0x13, 0x5D, 0xC1, 0x67, 0x50, 0x0A, 0x7B, 0x58,
0x62, 0x6B, 0xB8, 0x2C, 0xEA, 0x00, 0x0C, 0xC2,
0x10, 0x80, 0x63, 0x39, 0xC0, 0x7D, 0x67, 0x7F,
0x09, 0xFE, 0x58, 0x66, 0x99, 0x2E, 0x18, 0x99,
0x1E, 0x37, 0x3A, 0x9C, 0x0C, 0xCE, 0x03, 0xEB,
0x3B, 0xF8, 0x4F, 0xF2, 0xC3, 0x34, 0xC9, 0x70,
0xC4, 0xC8, 0xF1, 0xB9, 0xD4, 0xE0, 0x19, 0x80,
0xCC, 0xC9, 0x70, 0xC4, 0xC8, 0xF1, 0xB9, 0xD4,
0xE1, 0x98, 0x0C, 0xCC, 0x97, 0x0C, 0x4C, 0x8F,
0x1B, 0x9D, 0x4E, 0x03, 0xFF, 0xD9};
static const int goodJpegImageWidth = 128;
static const int goodJpegImageHeight = 128;

// https://code.google.com/p/android/issues/detail?id=42382
// https://code.google.com/p/android/issues/detail?id=9064
// https://code.google.com/p/skia/issues/detail?id=1649

/**
  This test will test the ability of the SkImageDecoder to deal with
  Jpeg files which have been mangled somehow.  We want to display as
  much of the jpeg as possible.
*/
DEF_TEST(Jpeg, reporter) {
    size_t len = sizeof(goodJpegImage) / 2;
    // I am explicitly not putting the entire image into the
    // DecodeMemory.  This simulates a network error.

    SkBitmap bm8888;
    bool imageDecodeSuccess = SkImageDecoder::DecodeMemory(
        static_cast<void *>(goodJpegImage), len, &bm8888);
    REPORTER_ASSERT(reporter, imageDecodeSuccess);
    REPORTER_ASSERT(reporter, bm8888.width() == goodJpegImageWidth);
    REPORTER_ASSERT(reporter, bm8888.height() == goodJpegImageHeight);
    REPORTER_ASSERT(reporter, !(bm8888.empty()));

    // Pick a few pixels and verify that their colors match the colors
    // we expect (given the original image).
    REPORTER_ASSERT(reporter, bm8888.getColor(7, 9) == 0xffffffff);
    REPORTER_ASSERT(reporter, bm8888.getColor(28, 3) == 0xff000000);
    REPORTER_ASSERT(reporter, bm8888.getColor(27, 34) == 0xffffffff);
    REPORTER_ASSERT(reporter, bm8888.getColor(71, 18) == 0xff000000);

    // This is the fill color
    REPORTER_ASSERT(reporter, bm8888.getColor(127, 127) == SK_ColorWHITE);

    #if JPEG_TEST_WRITE_TO_FILE_FOR_DEBUGGING
    // Check to see that the resulting bitmap is nice
    bool writeSuccess = (!(bm8888.empty())) && SkImageEncoder::EncodeFile(
        "HalfOfAJpeg.png", bm8888, SkImageEncoder::kPNG_Type, 100);
    SkASSERT(writeSuccess);
    #endif
}
